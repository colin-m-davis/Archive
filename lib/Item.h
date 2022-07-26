#ifndef ITEM_H_
#define ITEM_H_

#include <stdint.h>

typedef struct item Item;
typedef enum item_kind ItemKind;
void item_write(Item item, uint8_t hash[32], const char* data);
void item_read(Archive* arc, char* hash);
void get_hash(uint8_t* out, const char* message);
void item_serialize(char* out, Item item);

#endif // ITEM_H_