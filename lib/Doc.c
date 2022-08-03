#include "Archive.h"
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <errno.h> // errno
#include "Hash.h"
#include <stdlib.h>

#include "zpipe.c"

#define _MAX_RESULTS 1024

void scan(const char *base_path, char **results, size_t *size) {
    if (*size >= _MAX_RESULTS * sizeof(char *)) { return; }
    // TODO: chunks
    char path[256];
    struct dirent *dp;
    DIR *dir = opendir(base_path);
    if (!dir) { return; }
    // BFS
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0 && strcmp(dp->d_name, ".arc") != 0) { // avoid certain dirs
            results[*size] = malloc(256 * sizeof(char));
            snprintf(results[*size], 128, "%s/%s", base_path, dp->d_name);
            snprintf(path, 128, "%s/%s", base_path, dp->d_name);
            ++(*size);
            scan(path, results, size);
        }
    }

    closedir(dir);
}

// this is a big one
void archive_doc(Archive* arc) {
    // Open document buffer

    char **results;
    results = malloc(_MAX_RESULTS * sizeof(char *));

    size_t num_results = 0;
    scan(arc->wt_path, results, &num_results);

    char document[64 + (352 * num_results)];
    
    // Iterate over results to write blobs and collect hashes
    int i;
    int docptr = 0; // index to print next filepath at 
    for (i=0; i<num_results; ++i) {
        const char* src_path = results[i];

        struct stat src_path_stat;
        stat(src_path, &src_path_stat);
        if (!S_ISREG(src_path_stat.st_mode)) { continue; } // ignore non-file results

        // Compression source
        FILE *src_file = fopen(src_path, "rb");
        assert(src_file != NULL);

        // Get file size
        fseek(src_file, 0, SEEK_END);
        long file_size = ftell(src_file);
        rewind(src_file);

        // Read file
        uint8_t* ostream = malloc(file_size);
        fread(ostream, 1, file_size, src_file);
        fclose(src_file);

        // Hash contents, get path, store in hashes array
        uint8_t hash[32];
        message_to_hash(hash, ostream, file_size);
        free(ostream);

        char dest_path[86];
        char dest_fpath[94];
        hash_to_path(dest_fpath, dest_path, arc, hash);

        fprintf(stderr, "%s\n", src_path);
        // Create file at path
        int err = mkdir(dest_path, S_IRWXU);
        if (err != 0 && errno != EEXIST) {
            perror("Error: dest folder");
        }
        src_file = fopen(src_path, "rb");

        if (access(dest_fpath, F_OK) == 0) {
            // File unmodified since last time, no need to overwrite
            fprintf(stderr, "%s\n", "Match");
        } else {
            // Compress and write file
            fprintf(stderr, "%s\n", "No match");
            FILE *dst_file = fopen(dest_fpath, "wb+");
            assert(dst_file != NULL);

            int deflate_success = def(src_file, dst_file, 8);
            if (deflate_success != 0) {
                perror("Error: compression");
            }
        }

        // Store hash
        size_t src_len = strlen(src_path);
        size_t dst_len = strlen(dest_fpath);

        snprintf(document + docptr, src_len+1, "%s", src_path);
        docptr += src_len + 1;
        snprintf(document + docptr, dst_len+1, "%s", dest_fpath);
        docptr += dst_len + 1;
        free(results[i]);
    }
    
    assert(i == num_results);
    printf(document);
    printf("\n");
    free(results);
}

// Test decompression to random file

// fclose(src_file);
// fclose(dst_file);

// FILE* deest_file = fopen(dest_fpath, "rb");
// assert(deest_file != NULL);

// FILE* new_file = fopen("path/to/test/file", "wb+");
// assert(new_file != NULL);

// int inflate_success = inf(deest_file, new_file);
// if (inflate_success != 0) {
//     perror("Error decompression");
// }

// fclose(deest_file);
// fclose(new_file);

// int x = 10000;
