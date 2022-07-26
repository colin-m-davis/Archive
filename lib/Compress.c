#include "Compress.h"
#include "Repo.h"

#include <string.h>
#include <zlib.h>

enum object { Commit, Tree, Blob, Tag }; // in Compress.h: typedef enum object Object

void object_read(Repo* repo, char* hash) {
    // Get first two characters of hash
    char* first = "";
    first[0] = hash[0];
    first[1] = hash[1];
    first[2] = 0;
    // Get the rest of the characters
    char* rest = hash + 2;

    int x = 5;
    printf("Hahhaa");
}

void object_serialize(Object obj) {
    switch(obj) {
        case Commit:
            break;
        case Tree:
            break;
        case Blob:
            break;
        case Tag:
            break;
    }
}
