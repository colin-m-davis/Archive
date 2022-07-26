#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // getcwd
#include <sys/stat.h> // mkdir
#include <string.h> // strncat
#include <errno.h> // ENOENT
#include <assert.h>

#include "Repo.c"

int main(int argc, char* argv[]) {
    char cwd[256];
    getcwd(cwd, 256);
    Repo * reap = repo_init(cwd);
    return 0;
}

