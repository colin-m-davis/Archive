#include "ItemReadWrite.h"
#include "zlib.h"
#include "sha-256.h"
#include "Hash.h"

#include <zlib.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum kind { Commit, Tree, Blob, Tag }; // in Object.h: typedef enum object Object
struct item {
    Kind kind;
    char *data;
};

void item_write(uint8_t *out, Archive *arc, Item *item) {
    // Bytef *istream = serialize(item);
    Bytef *istream = (Bytef*)item->data;
    uLongf data_size = strlen(item->data)+1;
    uLongf destLen = compressBound(data_size);
    Bytef *ostream = malloc(destLen);
    int res = compress(ostream + sizeof(uLongf), &destLen, istream, data_size);
    *((uLongf *)ostream) = data_size;

    // Generate hash
    message_to_hash(out, item->data);

    // Construct path from hash
    char dir_path[256];
    char file_path[256]; 
    hash_to_path(file_path, dir_path, arc, out);  
    _mkdir(dir_path); 

    // Open file (create if it does not yet exist)
    FILE *item_file = fopen(file_path, "wb+");
    assert(item_file != NULL);

    // Write item data into file
    fwrite(ostream, 1, destLen + sizeof(uLongf), item_file);
    fclose(item_file);
}

void item_read(Item *out, Archive *arc, uint8_t *hash) {
    // Represent hash in a string of hex values
    char dir_path[256];
    char file_path[256]; 
    hash_to_path(file_path, dir_path, arc, hash);

    // Open file
    FILE *item_file = fopen(file_path, "rb");
    assert(item_file != NULL);

    // Get file size
    fseek(item_file, 0, SEEK_END);
    long raw_size = ftell(item_file);
    rewind(item_file);

    // Read file contents into ostream
    u_char *ostream = malloc(raw_size);
    fread(ostream, 1, raw_size, item_file);
    uLongf uncompressed_size = *((uLongf *) ostream);
    Bytef *uncompressed = malloc(uncompressed_size);
    Bytef *compressed = ostream + sizeof(uLongf);

    // Uncompress ostream to item data
    int res = uncompress(uncompressed, &uncompressed_size, compressed, raw_size);
    fclose(item_file);
}