
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdint.h>
#include "raid.h"

#define THREAD_COUNT 3
#define ITERATIONS 100

static void *worker(void *arg)
{
    intptr_t id = (intptr_t)arg;
    char write_buffer[BLOCK_SIZE] = {0};
    char read_buffer[BLOCK_SIZE] = {0};

    snprintf(write_buffer, sizeof(write_buffer),
             "Thread %ld RAID test", (long)id);

    for (int i = 0; i < ITERATIONS; i++)
    {
        uint64_t block = (uint64_t)id;

        if (raid_write(block, write_buffer, BLOCK_SIZE) != 0)
        {
            fprintf(stderr, "Thread %ld: write failed\n", (long)id);
            return (void *)1;
        }

        if (raid_read(block, read_buffer, BLOCK_SIZE) != 0 ||
            memcmp(write_buffer, read_buffer, BLOCK_SIZE) != 0)
        {
            fprintf(stderr, "Thread %ld: read/compare failed\n", (long)id);
            return (void *)1;
        }
    }

    printf("Thread %ld: PASS (%d iterations)\n",
           (long)id, ITERATIONS);
    return NULL;
}

int main(void)
{
    pthread_t threads[THREAD_COUNT];
    int failures = 0;

    printf("===== RAIDVOL CONCURRENCY TEST =====\n");

    for (intptr_t i = 0; i < THREAD_COUNT; i++)
    {
        if (pthread_create(&threads[i], NULL, worker, (void *)i) != 0)
        {
            perror("pthread_create");
            return 1;
        }
    }

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        void *result = NULL;

        if (pthread_join(threads[i], &result) != 0 || result != NULL)
            failures++;
    }

    printf("Result: %s\n", failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
