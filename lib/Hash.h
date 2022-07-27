#ifndef HASH_H_
#define HASH_H_

#include "Archive.h"
#include "sha-256.h"
#include <stdint.h> // uint8-t

void message_to_hash(uint8_t* out, const char* message);
void hash_to_path(char* out, const Archive* arc, const uint8_t* hash);

#endif // HASH_H_