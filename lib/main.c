#include <unistd.h> // getcwd
#include <stdio.h>

#include "Archive.c"
#include "Item.c"
#include "Hash.c"

int main(void) {
    Archive* bark = archive_init("/Users/colindavis/code/projects/yar");
    Item* okay = malloc(sizeof(Item));
    okay->data = "here i am, a lonely item in an empty archive.";
    uint8_t hash[32];
    item_write(hash, bark, okay);
    return 0;
}

