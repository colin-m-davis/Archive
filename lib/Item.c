#include "Item.h"

#include "Archive.h"
#include "zlib.h"
#include "sha-256.c"

#include <string.h>
#include <zlib.h>

enum kind { Commit, Tree, Blob, Tag }; // in Object.h: typedef enum object Object
struct item {
    Kind kind;
    Archive* owner;
    char* data;
};

// Get SHA-256 hash of message as uint8 array
void message_to_hash(uint8_t* out, const char* message) {
    calc_sha_256(out, message, strlen(message)); // see sha-256.h
}

// For a standard 32-byte hash, produces a 65-char output including the final '\0'
void hash_to_path(char* out, const Archive* arc, const uint8_t* hash) {
    // Represent hash as hex string
    char hash_str[65];
    char* hsp = hash_str;
    for (int i = 0; i < 32; i++) {
        hsp += sprintf(hash_str, "%.2x", hash[i]);
    }

    // Folder name is first 2 chars
    char first_two[3];
    strncpy(first_two, hash_str, 2);

    // File name is the subsequent 30 chars
    char rest[31];
    strncpy(rest, hash_str + 2, 30);

    // Construct file path
    snprintf(out, 256, "%s/%s/%s/%s", arc->arc_path, "items", first_two, rest);
}

void item_write(Item* item, uint8_t* hash, const char* data) {
    char* serialized;
    item_serialize(serialized, item);

    // TODO: a lot

    // insert original filesize as long at beginning of buffer. makes reading later easier!
}

void item_read(Item* out, Archive* arc, uint8_t* hash) {
    // Represent hash in a string of hex values
    char file_path[256]; 
    hash_to_path(file_path, arc, hash);


    // Open file at file path
    FILE* item_file = fopen(file_path, "rb");
    if (item_file == NULL) {
        fprintf(stderr, "could not read item file.");
        exit(1);
    }

    // Get file size
    fseek(item_file, 0, SEEK_END);
    size_t raw_size = ftell(item_file);
    rewind(item_file);

    // Allocate memory for char buffer
    char* raw_buf;
    raw_buf = malloc((raw_size + 1) * sizeof(*raw_buf));

    // Read file contents into buffer and terminate it with '\0'
    fread(raw_buf, raw_size, 1, item_file);
    raw_buf[raw_size] = 0;

    // Get size of original data

    
    // Uncompress contents of buffer
    char* buf;
    size_t size = 
    buf = malloc(size * sizeof(*buf));
    free(raw_buf);

    // Parse them

    //
}


void item_serialize(char** out, const Item* item) {
    switch(item->kind) {
        case Commit:
            break;
        case Tree:
            break;
        case Blob:
            blob_serialize(out, item);
            break;
        case Tag:
            break;
    }
}

void commit_serialize(char** out, const Item* item) {

}

void commit_deserialize(Item* item, const char* in) {

}

void blob_serialize(char** out, const Item* item) {
    *out = strndup(item->data, strlen(item->data));
}

void blob_deserialize(Item* item, const char* in) {
    item->data =strndup(in, strlen(in));
}



// Get existing repo from path (in case parent dir move)