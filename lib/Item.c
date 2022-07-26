#include "Item.h"

#include "Archive.h"
#include "zlib.h"
#include "sha-256.c"

#include <string.h>
#include <zlib.h>

enum item_kind { Commit, Tree, Blob, Tag }; // in Object.h: typedef enum object Object
struct item {
    ItemKind kind;
    Archive* owner;
    char* data;
};


void item_write(Item item, uint8_t hash[32], const char* data) {
    char* serialized;
    item_serialize(serialized, item);

    // TODO
}

void item_read(Archive* arc, char* hash) {
    // Get first 2 characters of hash
    char first_two[2];
    memcpy(first_two, hash, 2);
    // Get the rest of the characters (32-2=30)
    char* rest = hash + 2;

    char rpath[10] = "objects";
    strncat(rpath, first_two, 2);
    char fullpath[256];
    get_path(arc->arc_path, "objects", fullpath);

    // TODO
}

// Get SHA-256 hash of message as uint8 array
void get_hash(uint8_t* out, const char* message) {
    calc_sha_256(out, message, strlen(message)); // see sha-256.h
}

void item_serialize(char* out, Item item) {
    switch(item.kind) {
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
