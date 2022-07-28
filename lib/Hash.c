#include "Hash.h"
#include "sha-256.c"

#include <stdio.h>

// Get SHA-256 hash of message as uint8 array
void message_to_hash(uint8_t* out, const char* message) {
    calc_sha_256(out, message, strlen(message)); // see sha-256.h
}

// Path for folder and file from hash in Archive
void hash_to_path(char* out_file, char* out_dir, const Archive* arc, const uint8_t* hash) {
    // Represent hash as hex string
    char hash_str[65];
    int offset = 0;
    for (int i = 0; i < 32; i++) {
        offset += snprintf(hash_str + offset, 65-(offset), "%.2x", hash[i]);
    }

    // Folder name is first 2 chars
    char first_two[3];
    snprintf(first_two, 3, "%s", hash_str);

    // File name is the subsequent 30 chars
    char rest[31];
    snprintf(rest, 31, "%s", hash_str+2);

    // Construct file path
    snprintf(out_dir, 256, "%s/%s/%s", arc->arc_path, "items", first_two);
    snprintf(out_file, 256, "%s/%s/%s/%s", arc->arc_path, "items", first_two, rest);
}