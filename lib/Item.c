#include "Item.h"
#include "zlib.h"
#include "sha-256.h"
#include "Hash.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <zlib.h>

enum kind { Commit, Tree, Blob, Tag }; // in Object.h: typedef enum object Object
struct item {
    Kind kind;
    char* data;
};

void item_write(uint8_t* out, Archive* arc, Item* item) {
    long data_size = strlen(item->data);
    uLong comp_bound = compressBound(data_size);
    char compressed[11 + comp_bound];
    snprintf(compressed, 10, "%lu", data_size);
    compress((u_char*)compressed+10, &comp_bound, (u_char*)item->data, data_size);

    // generate hash
    uint8_t hash[32];
    message_to_hash(hash, compressed);

    // get file_path from hash
    char file_path[256]; 
    hash_to_path(file_path, arc, hash);   

    FILE* item_file = fopen(file_path, "w+");
    if (item_file == NULL) {
        fprintf(stderr, "could not write item file.");
        exit(1);
    }

    fwrite(compressed, data_size+11, 1, item_file);
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
    long raw_size = ftell(item_file);
    rewind(item_file);

    // Allocate memory for char buffer
    char raw_buf[raw_size+1];

    // Read file contents into buffer and terminate it with '\0'
    fread(raw_buf, raw_size, 1, item_file);
    raw_buf[raw_size] = 0;

    // Get size of original data
    // Maximum # digits for ulong in dec representation is 10
    char og_size_buf[11];
    snprintf(og_size_buf, 10, "%s", raw_buf);
    u_long og_size = strtoul(og_size_buf, NULL, 10);

    // Uncompress contents of buffer
    char og_buf[og_size];
    uncompress((u_char*)og_buf, &og_size, (u_char*)raw_buf+10, raw_size);

    printf(og_buf);
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