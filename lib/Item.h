#ifndef ITEM_H_
#define ITEM_H_

#include <stdint.h>

typedef struct item Item;
typedef enum kind Kind;
void hash_to_path(char* out, const Archive* arc, const uint8_t* hash);
void item_write(Item* item, uint8_t* hash, const char* data);
void item_read(Item* out, Archive* arc, uint8_t* hash);
void commit_serialize(char** out, const Item* item);
void tree_serialize(char** out, const Item* item);
void blob_serialize(char** out, const Item* item);
void tag_serialize(char** out, const Item* item);
void item_serialize(char** out, const Item* item);

#endif // ITEM_H_