#include <unistd.h> // getcwd
#include <stdio.h>

#include "LogReadWrite.c"
#include "Doc.c"
#include "Archive.c"


int main(void) {
    Archive* arc = archive_init(".");
    archive_doc(arc);
}