#include "Repo.h"

#include <stdlib.h>
#include <unistd.h> // getcwd
#include <sys/stat.h> // mkdir
#include <assert.h> // assert

struct repo {
    char* wt_path;
    char* arc_path; // worktree/.arc
};

// relative path to absolute path
// assumes from > to
// caller is responsible for allocating memory
void get_path(const char* from, const char* to, char buf[256]) {
    snprintf(buf, 256, "%s%s%s", from, "/", to);
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
            mkdir(buf, S_IRWXU);
            *p = '/';
        }
    mkdir(buf, S_IRWXU);
}

void repo_dir(Repo* repo, const char* rpath) {
    char apath[256];
    get_path(repo->arc_path, rpath, apath);
    _mkdir(apath);
}

Repo* repo_init(const char* path) {
    // Calculate archive path
    char tmp_arc_path[256];
    get_path(path, ".arc", tmp_arc_path);

    // If repository already exists at path return NULL
    struct stat s;
    int err = stat(tmp_arc_path, &s);
    if (err == 0) {
        if(S_ISDIR(s.st_mode)) {
            fprintf(stderr, "could not initialize archive: .arc directory already exists in path");
            exit(1);
        }
    }

    // Allocate space and initialize members
    Repo* new_repo = malloc(sizeof(Repo));
    new_repo->wt_path = malloc(sizeof(char*));
    memcpy(new_repo->wt_path, path, strlen(path));
    new_repo->arc_path = malloc(sizeof(char*));
    memcpy(new_repo->arc_path, tmp_arc_path, strlen(tmp_arc_path));

    // Shorthand for new repo's members
    char* wt = new_repo->wt_path;
    char* arc = new_repo->arc_path;

    repo_dir(new_repo, "branches");
    repo_dir(new_repo, "objects");
    repo_dir(new_repo, "refs/tags");
    repo_dir(new_repo, "refs/heads");
    
    return new_repo;
}




// TODO: Delete repo prevent memory leak B)