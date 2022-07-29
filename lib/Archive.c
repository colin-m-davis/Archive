#include "Archive.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // strlen
#include <unistd.h> // getcwd
#include <sys/stat.h> // mkdir
#include <assert.h> // assert
#include <errno.h> // errno

// relative path to absolute path
// assumes from > to
// caller is responsible for allocating memory
void get_path(const char* from, const char* to, char* buf) {
    snprintf(buf, 256, "%s/%s", from, to);
}

// $ mkdir -p dir
// credit to http://nion.modprobe.de/blog/archives/357-Recursive-directory-creation.html
void _mkdir(const char* dir) {
    char buf[256];
    char *p = NULL;

    snprintf(buf, sizeof(buf), "%s", dir);
    size_t len = strlen(buf);
    if (buf[len - 1] == '/')
        buf[len - 1] = 0;
    for (p = buf + 1; *p; p++)
        if (*p == '/') {
            *p = 0;
            if (mkdir(buf, S_IRWXU) == -1) {
                if (errno != EEXIST) {
                    perror("could not make directory");
                    exit(1);
                }
            }
            *p = '/';
        }
    mkdir(buf, S_IRWXU);
}

void archive_mkdir(Archive* archive, const char* rpath) {
    char apath[256];
    get_path(archive->arc_path, rpath, apath);
    _mkdir(apath);
}

Archive* archive_get(const char* path) {
    char check_arc_path[256];
    get_path(path, ".arc", check_arc_path);

    struct stat s;
    int err = stat(check_arc_path, &s);
    if (err == 0) {
        if(S_ISDIR(s.st_mode)) {
            Archive* got_arc = malloc(sizeof(Archive));
            got_arc->wt_path = strdup(path);
            got_arc->arc_path = strdup(check_arc_path);
            return got_arc;
        }
    }
    return NULL;
}

Archive* archive_init(const char* path) {
    // char path[256];
    // getcwd(path, 256);
    // Calculate archive path

    Archive* check = archive_get(path);
    if (check != NULL) {
        fprintf(stderr, "detected existing archive in path\n");
        return check;
    }

    char tmp_arc_path[256];
    get_path(path, ".arc", tmp_arc_path);

    // Allocate space and initialize members
    Archive* new_arc = malloc(sizeof(Archive));
    new_arc->wt_path = strdup(path);
    // memcpy(new_arc->wt_path, path, strlen(path));
    new_arc->arc_path = strdup(tmp_arc_path);
    // memcpy(new_arc->arc_path, tmp_arc_path, strlen(tmp_arc_path));

    // Create directories in ./.arc
    archive_mkdir(new_arc, "branches");
    archive_mkdir(new_arc, "logs");
    archive_mkdir(new_arc, "refs/tags");
    archive_mkdir(new_arc, "refs/heads");
    
    return new_arc;
}




// TODO: Delete archive prevent memory leak B)