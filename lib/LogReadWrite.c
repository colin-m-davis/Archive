#include "LogReadWrite.h"
#include "zlib.h"
#include "sha-256.h"
#include "Hash.c"

#include <zlib.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void log_write(uint8_t *out, Archive *arc, char *data, size_t data_size) {
    // Bytef *istream = serialize(log);
    Bytef *istream = (Bytef*)data;
    uLongf destLen = compressBound(data_size);
    Bytef *ostream = malloc(destLen);
    int res = compress(ostream + sizeof(uLongf), &destLen, istream, data_size);
    *((uLongf *)ostream) = data_size;

    // Generate hash
    message_to_hash(out, data);

    // Construct path from hash
    char dir_path[256];
    char file_path[256]; 
    hash_to_path(file_path, dir_path, arc, out);  
    _mkdir(dir_path); 

    // Open file (create if it does not yet exist)
    FILE *log_file = fopen(file_path, "wb+");
    assert(log_file != NULL);

    // Write log data into file
    fwrite(ostream, 1, destLen + sizeof(uLongf), log_file);
    fclose(log_file);
    free(ostream);
}

void log_read(uint8_t *uncompressed, size_t *data_size_out, Archive *arc, uint8_t *hash) {
    // Represent hash in a string of hex values
    char dir_path[256];
    char file_path[256]; 
    hash_to_path(file_path, dir_path, arc, hash);

    // Open file
    FILE *log_file = fopen(file_path, "rb");
    assert(log_file != NULL);

    // Get file size
    fseek(log_file, 0, SEEK_END);
    long raw_size = ftell(log_file);
    rewind(log_file);

    // Read file contents into ostream
    u_char *ostream = malloc(raw_size);
    fread(ostream, 1, raw_size, log_file);
    *data_size_out = *((uLongf *) ostream);
    *uncompressed = malloc(*data_size_out);
    Bytef *compressed = ostream + sizeof(uLongf);

    // Uncompress ostream to log data
    int res = uncompress(uncompressed, data_size_out, compressed, raw_size);
    fclose(log_file);
    free(ostream);
    free(uncompressed);
}