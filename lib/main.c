#include <unistd.h> // getcwd
#include <stdio.h>

#include "Doc.c"
#include "Archive.c"
#include "Hash.c"


int main(void) {
    Archive* arc = archive_init(".");
    archive_doc(arc);

    int x = 5;
}