#ifndef ITEMREADWRITE_H_
#define ITEMREADWRITE_H_

#include "Archive.h"
#include <stdint.h>

typedef struct item Item;
typedef enum kind Kind;
void item_write(uint8_t* out, Archive* arc, Item* item);
void item_read(Item* out, Archive* arc, uint8_t* hash);

#endif // ITEMREADWRITE_H_