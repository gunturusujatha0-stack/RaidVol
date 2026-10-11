#include "disk.h"
#include <stdio.h>

const char *disk_names[NUM_DISKS] = {
    "disks/disk1.img",
    "disks/disk2.img",
    "disks/disk3.img"
};

int disk_exists(int disk)
{
    if (disk < 0 || disk >= NUM_DISKS)
        return 0;

    FILE *fp = fopen(disk_names[disk], "rb");

    if (fp == NULL)
        return 0;

    fclose(fp);
    return 1;
}

int disk_read(int disk, uint64_t block_number,
              void *buffer, size_t size)
{
    if (disk < 0 || disk >= NUM_DISKS ||
        buffer == NULL || size == 0 ||
        block_number > (uint64_t)(INT64_MAX / BLOCK_SIZE))
        return -1;

    FILE *fp = fopen(disk_names[disk], "rb");

    if (fp == NULL)
        return -1;

    uint64_t offset = block_number * BLOCK_SIZE;

    if (fseeko(fp, (off_t)offset, SEEK_SET) != 0)
    {
        fclose(fp);
        return -1;
    }

    size_t bytes = fread(buffer, 1, size, fp);
    fclose(fp);

    return bytes == size ? 0 : -1;
}

int disk_write(int disk, uint64_t block_number,
               const void *buffer, size_t size)
{
    if (disk < 0 || disk >= NUM_DISKS ||
        buffer == NULL || size == 0 ||
        block_number > (uint64_t)(INT64_MAX / BLOCK_SIZE))
        return -1;

    FILE *fp = fopen(disk_names[disk], "r+b");

    if (fp == NULL)
        return -1;

    uint64_t offset = block_number * BLOCK_SIZE;

    if (fseeko(fp, (off_t)offset, SEEK_SET) != 0)
    {
        fclose(fp);
        return -1;
    }

    size_t bytes = fwrite(buffer, 1, size, fp);
    int result = (bytes == size && fflush(fp) == 0) ? 0 : -1;

    fclose(fp);
    return result;
}
