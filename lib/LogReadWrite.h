#ifndef LOGREADWRITE_H_
#define LOGREADWRITE_H_

#include "Archive.h"
#include <stdint.h>
#include <stdlib.h>

void log_write(uint8_t* out, Archive* arc, const uint8_t *data, size_t data_size);
uint8_t* log_read(size_t *data_size_out, Archive* arc, uint8_t* hash);

#endif // logREADWRITE_H_