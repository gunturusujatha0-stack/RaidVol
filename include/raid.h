#ifndef RAID_H
#define RAID_H

#include <stdint.h>
#include <stddef.h>

#include "disk.h"

int raid_write(uint64_t logical_block,
               const void *buffer, size_t size);

int raid_read(uint64_t logical_block,
              void *buffer, size_t size);

int raid_status(void);

#endif
