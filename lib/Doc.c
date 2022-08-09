#include "Archive.h"
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <errno.h> // errno
#include "Hash.h"
#include <stdlib.h>
#include <time.h>

#include "zpipe.c"


void scan(const char *base_path, char **results, size_t *size) {
    // TODO: chunks
    char path[256];
    struct dirent *dp;
    DIR *dir = opendir(base_path);
    if (!dir) { return; }
    // BFS
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0 && strcmp(dp->d_name, ".arc") != 0) { // TODO: ignore from list
            snprintf(path, 256, "%s/%s", base_path, dp->d_name);
            results[*size] = strdup(path);
            ++(*size);
            if (*size >= 800) { return; }
            scan(path, results, size);
        }
    }

    closedir(dir);
}

void archive_doc(Archive* arc, char* message) {
    // Open document buffer
    char **results;
    results = malloc(1024 * sizeof(char *));

    size_t num_results = 0;
    scan(arc->wt_path, results, &num_results);

    char *document = malloc(64 + 512 * num_results);
    if (document == NULL) {
        fprintf(stderr, "Failed to allocate enough memory for document.");
        exit(1);
    }
    snprintf(document, 33, "%s\n", message);
    snprintf(document+33, 33, "%u\n", (unsigned)time(NULL));
    // Iterate over results to write blobs and collect hashes
    int i;
    int docptr = 67; // index to print next filepath at 
    for (i=0; i<num_results; ++i) {
        const char* src_path = results[i];

        struct stat src_path_stat;
        stat(src_path, &src_path_stat);
        if (!S_ISREG(src_path_stat.st_mode)) { continue; } // ignore non-file results

        // Compression source
        FILE *src_file = fopen(src_path, "rb");

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

        char dest_fpath[66];
        char dest_path[5];
        hash_to_path(dest_fpath, dest_path, arc, hash);

        char *full_dest_fpath = malloc(4096);
        char *full_dest_path = malloc(4096);

        snprintf(full_dest_fpath, 4096, "%s/logs/%s", arc->arc_path, dest_fpath);
        snprintf(full_dest_path, 4096, "%s/logs/%s", arc->arc_path, dest_path);

        fprintf(stderr, "%s\n", src_path);
        // Create file at path
        _mkdir(full_dest_path);
        src_file = fopen(src_path, "rb");

        if (access(full_dest_fpath, F_OK) == 0) {
            // File unmodified since last time, no need to overwrite
            fprintf(stderr, "%s\n", "Match");
        } else {
            // Compress and write file
            fprintf(stderr, "%s\n", "No match");
            FILE *dst_file = fopen(full_dest_fpath, "wb+");
            assert(dst_file != NULL);

            int deflate_success = def(src_file, dst_file, 8);
            if (deflate_success != 0) {
                perror("Error: compression");
            }
        }

        free(full_dest_fpath);
        free(full_dest_path);

        // Store hash
        size_t src_len = strlen(src_path);
        size_t dst_len = strlen(dest_fpath);

        snprintf(document + docptr, src_len+1, "%s", src_path);
        docptr += src_len;
        snprintf(document + docptr, 2, "%s", "\n");
        docptr++;
        snprintf(document + docptr, dst_len+1, "%s", dest_fpath);    
        docptr += dst_len;
        snprintf(document + docptr, 2, "%s", "\n");
        docptr++;
    }
    uint8_t doc_hash[64];
    message_to_hash(doc_hash, (uint8_t*)document, docptr);
    char* temp = "./.arc/temp";
    FILE *tmp_file = fopen(temp, "wb+");
    fwrite(document, 1, docptr, tmp_file);
    fclose(tmp_file);

    tmp_file = fopen(temp, "rb");
    fclose(tmp_file);
    
    assert(i == num_results);
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

// int x = 10000
