#include <unistd.h> // getcwd

#include "Repo.c"

#include "Compress.c"

int main(int argc, char* argv[]) {
    char cwd[256];
    getcwd(cwd, 256);
    Repo * reap = repo_init(cwd);
    object_read(reap, ":)HAHAHjfioggggoHAHAHAH");

    return 0;
}

