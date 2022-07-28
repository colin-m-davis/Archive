#include <unistd.h> // getcwd
#include <stdio.h>

#include "Archive.c"
#include "Item.c"
#include "Hash.c"
#include <zlib.h>

int main(void) {
    Archive* bark = archive_init("/Users/colindavis/code/projects/yar");
    Item okay;
    okay.data = "here i am, a lonely item in an empty archive.";
    uint8_t hash[32];
    item_write(hash, bark, &okay);
    Item* shlocked = malloc(sizeof(Item));
    item_read(shlocked, bark, hash);
    free(bark->arc_path);
    free(bark->wt_path);
    free(bark);
    return 0;
}

