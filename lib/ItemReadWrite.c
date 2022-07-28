#include "Item.h"
#include "zlib.h"
#include "sha-256.h"
#include "Hash.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum kind { Commit, Tree, Blob, Tag }; // in Object.h: typedef enum object Object
struct item {
    Kind kind;
    char* data;
};

void item_write(uint8_t* out, Archive* arc, Item* item) {
    long data_size = strlen(item->data);
    // Generate hash
    message_to_hash(out, item->data);

    // Construct path from hash
    char dir_path[256];
    char file_path[256]; 
    hash_to_path(file_path, dir_path, arc, out);  
    _mkdir(dir_path); 

    // Open file (create if it does not yet exist)
    FILE* item_file = fopen(file_path, "w+");
    if (item_file == NULL) {
        fprintf(stderr, "could not write item file.");
        exit(1);
    }

    // Write item data into file
    fwrite(item->data, 1, data_size, item_file);
    fclose(item_file);
}

void item_read(Item* out, Archive* arc, uint8_t* hash) {
    // Represent hash in a string of hex values
    char dir_path[256];
    char file_path[256]; 
    hash_to_path(file_path, dir_path, arc, hash);

    // Open file
    FILE* item_file = fopen(file_path, "rb");
    if (item_file == NULL) {
        fprintf(stderr, "could not read item file.");
        exit(1);
    }

    // Get file size
    fseek(item_file, 0, SEEK_END);
    long raw_size = ftell(item_file);
    rewind(item_file);

    // Read file contents into item data
    fread(out->data, 1, raw_size, item_file);
    fclose(item_file);
}

// void item_serialize(char** out, const Item* item) {
//     switch(item->kind) {
//         case Commit:
//             break;
//         case Tree:
//             break;
//         case Blob:
//             blob_serialize(out, item);
//             break;
//         case Tag:
//             break;
//     }
// }

// void commit_serialize(char** out, const Item* item) {

// }

// void commit_deserialize(Item* item, const char* in) {

// }

// void blob_serialize(char** out, const Item* item) {
//     *out = strndup(item->data, strlen(item->data));
// }

// void blob_deserialize(Item* item, const char* in) {
//     item->data =strndup(in, strlen(in));
// }



// Get existing repo from path (in case parent dir move)