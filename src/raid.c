
#include <stdio.h>
#include <pthread.h>

#include "raid.h"

/* Protect RAID read/write operations from concurrent access */
static pthread_mutex_t raid_mutex = PTHREAD_MUTEX_INITIALIZER;

int raid_write(uint64_t logical_block,
               const void *buffer, size_t size)
{
    if (buffer == NULL || size == 0 || size > BLOCK_SIZE)
        return -1;

    uint64_t physical_block = logical_block / NUM_DISKS;
    int disk = (int)(logical_block % NUM_DISKS);

    pthread_mutex_lock(&raid_mutex);

    int result = disk_write(disk, physical_block, buffer, size);

    pthread_mutex_unlock(&raid_mutex);

    return result;
}

int raid_read(uint64_t logical_block,
              void *buffer, size_t size)
{
    if (buffer == NULL || size == 0 || size > BLOCK_SIZE)
        return -1;

    uint64_t physical_block = logical_block / NUM_DISKS;
    int disk = (int)(logical_block % NUM_DISKS);

    pthread_mutex_lock(&raid_mutex);

    int result = disk_read(disk, physical_block, buffer, size);

    pthread_mutex_unlock(&raid_mutex);

    return result;
}

int raid_status(void)
{
    int online = 0;

    printf("\n===== RAIDVOL DISK STATUS =====\n");

    for (int i = 0; i < NUM_DISKS; i++)
    {
        if (disk_exists(i))
        {
            printf("Disk %d: ONLINE\n", i + 1);
            online++;
        }
        else
        {
            printf("Disk %d: OFFLINE\n", i + 1);
        }
    }

    if (online == NUM_DISKS)
    {
        printf("Volume: READY\n");
        return 0;
    }

    printf("Volume: DEGRADED — RAID 0 data may be unavailable\n");
    return -1;
}
