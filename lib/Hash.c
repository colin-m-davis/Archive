#include "Hash.h"
#include "sha-256.c"

#include <stdio.h>

// Get SHA-256 hash of message as uint8 array
void message_to_hash(uint8_t* out, const uint8_t* message, size_t data_size) {
    calc_sha_256(out, message, data_size); // see sha-256.h
}

// Path for folder and file from hash in Archive
void hash_to_path(char* out_file, char* out_dir, const Archive* arc, const uint8_t* hash) {
    // Represent hash as hex string
    char hash_str[65];
    int ofset = 0;
    for (int i = 0; i < 32; i++) {
        ofset += snprintf(hash_str + ofset, 65-(ofset), "%.2x", hash[i]);
    }

    // Folder name is first 2 chars
    char first_four[5];
    snprintf(first_four, 4, "%s", hash_str);

    // File name is the subsequent 30 chars
    char rest[61];
    snprintf(rest, 60, "%s", hash_str+4);

    // Construct file path
    snprintf(out_dir, 4, "%s", first_four);
    snprintf(out_file, 65, "%s/%s", first_four, rest);
}