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
            ++(*size);
            scan(path, results, size);
        }
    }

    closedir(dir);
}

// this is a big one
void archive_doc(Archive* arc) {
    char **results;
    results = malloc(_MAX_RESULTS * sizeof(char *));
    size_t num_results = 0;
    scan(arc->wt_path, results, &num_results);

    uint8_t **hashes;
    hashes = malloc(num_results);

    // Iterate over results to write blobs and collect hashes
    int i;
    for (i=0; i<num_results; ++i) {
        // Compression source
        const char* source_path = results[i];
        FILE *src_file = fopen(source_path, "rb");
        assert(src_file != NULL);

        // Get file size
        fseek(src_file, 0, SEEK_END);
        long file_size = ftell(src_file);
        rewind(src_file);

        // Read file
        uint8_t* ostream = malloc(file_size);
        fread(ostream, 1, file_size, src_file);
        fclose(src_file);

        // Hash contents, get path
        uint8_t hash[32];
        message_to_hash(hash, ostream, file_size);
        free(ostream);

        char dest_path[86];
        char dest_fpath[94];
        hash_to_path(dest_fpath, dest_path, arc, hash);

        // Create file at path
        int err = mkdir(dest_path, S_IRWXU);
        if (err != 0 && errno != EEXIST) {
            perror("Error: dest folder");
        }
        src_file = fopen(source_path, "rb");
        FILE *dst_file = fopen(dest_fpath, "wb+");
        assert(dst_file != NULL);

        int deflate_success = def(src_file, dst_file, 8);
        zerr(deflate_success);
        if (deflate_success != 0) {
            perror("Error compression");

        }

        fclose(src_file);
        fclose(dst_file);

        // Store hash
        printf(source_path);
        printf("\n");
        printf(dest_fpath);
        printf("\n");
        free(results[i]);
    }

    // commmit sudoku
    
    assert(i == num_results);

    free(results);
    free(hashes);
}

// FILE* deest_file = fopen(dest_fpath, "rb");
// assert(deest_file != NULL);

// FILE* new_file = fopen("/Users/colindavis/Code/yeet", "wb+");
// assert(new_file != NULL);

// int inflate_success = inf(deest_file, new_file);
// zerr(inflate_success);
// if (inflate_success != 0) {
//     perror("Error decompression");
// }

// fclose(deest_file);
// fclose(new_file);