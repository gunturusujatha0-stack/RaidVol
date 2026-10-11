#ifndef DISK_H
#define DISK_H

#include <stddef.h>
#include <stdint.h>

#define NUM_DISKS 3
#define BLOCK_SIZE 512

extern const char *disk_names[NUM_DISKS];

int disk_exists(int disk);
int disk_read(int disk, uint64_t block_number,
              void *buffer, size_t size);
int disk_write(int disk, uint64_t block_number,
               const void *buffer, size_t size);

#endif
