#include "Archive.h"
#include <dirent.h>
#include "LogReadWrite.h"
#include <sys/stat.h>
#include <stdio.h>


static char **list;

void scan(char *basePath, Archive *arc);

archive_doc(Archive* arc) {
    scan(arc->wt_path, arc);
}

void scan(char *basePath, Archive *arc)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    if (!dir) { return; }
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0 && strcmp(dp->d_name, ".arc") != 0) {
            // printf("%s\n", dp->d_name);
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            printf(path);
            printf("\n");
            
            scan(path, arc);
        }
    }

    closedir(dir);
}