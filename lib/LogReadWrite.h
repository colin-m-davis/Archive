#ifndef LOGREADWRITE_H_
#define LOGREADWRITE_H_

#include "Archive.h"
#include <stdint.h>
#include <stdlib.h>

void log_write(uint8_t* out, Archive* arc, char *data, size_t data_size);
void log_read(uint8_t *data_out, size_t *data_size_out, Archive* arc, uint8_t* hash);

#endif // logREADWRITE_H_