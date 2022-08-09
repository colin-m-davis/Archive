#include <unistd.h> // getcwd
#include <stdio.h>

#include "Doc.c"
#include "Archive.c"
#include "Hash.c"
#include "zlib.h"

int main(void) {
    Archive* arc = archive_init(".");
    archive_doc(arc, "binitialcommit");

    int x = 5;
}