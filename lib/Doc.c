#include "Archive.h"
#include <dirent.h>
#include "LogReadWrite.h"
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

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
            snprintf(path, 256, "%s/%s", base_path, dp->d_name);
            printf(path);
            printf("\n");
            results[*size] = malloc(256 * sizeof(char));
            strcpy(results[*size], path);
            ++(*size);
            scan(path, results, size);
        }
    }

    closedir(dir);
}

// this is a big one
void archive_doc(Archive* arc) {
    char **results;
    results = malloc(_MAX_RESULTS * sizeof(char *)); // max results 1024 for now
    size_t num_results = 0;
    scan(arc->wt_path, results, &num_results);

    for (int i=0; i<num_results; ++i) {
        const char* path = results[i];
        FILE *file = fopen(path, "r");
        assert(file != NULL);

        // Get file size
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        rewind(file);

        char* ostream = malloc(file_size);
        fread(ostream, 1, file_size, file);
        fclose(file);

        uint8_t hash[32];
        uint8_t* hp = hash;
        log_write(hp, arc, ostream, file_size);

        printf(path);
        printf(":D\n\n");
        free(ostream);

        size_t ds = 0;
        uint8_t* okay = log_read(&ds, arc, hash);


        int x = 5; // :S
    }
}
