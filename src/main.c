#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "raid.h"

#define MAX_DATA 4096

static void clear_input(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
}

static void show_menu(void)
{
    printf("\n========== RAIDVOL ==========\n");
    printf("1. Create / Check RAID Volume\n");
    printf("2. Write Data\n");
    printf("3. Read Data\n");
    printf("4. Show Disk Status\n");
    printf("5. Exit\n");
    printf("=============================\n");
    printf("Enter choice: ");
}

static void write_data(void)
{
    char data[MAX_DATA];

    printf("Enter data (maximum %d characters): ", MAX_DATA);
    clear_input();

    if (fgets(data, sizeof(data), stdin) == NULL)
    {
        printf("Input error.\n");
        return;
    }

    size_t length = strcspn(data, "\n");
    data[length] = '\0';

    if (length == 0)
    {
        printf("No data entered.\n");
        return;
    }

    /* Write data in chunks of at most BLOCK_SIZE bytes.
       Each chunk is stored in one logical RAID block. */
    size_t offset = 0;
    uint64_t logical_block = 0;

    while (offset < length)
    {
        size_t chunk = length - offset;

        if (chunk > BLOCK_SIZE)
            chunk = BLOCK_SIZE;

        char buffer[BLOCK_SIZE] = {0};
        memcpy(buffer, data + offset, chunk);

        if (raid_write(logical_block, buffer, BLOCK_SIZE) != 0)
        {
            printf("Write failed.\n");
            return;
        }

        offset += chunk;
        logical_block++;
    }

    FILE *meta = fopen("metadata.txt", "w");
    if (meta == NULL)
    {
        printf("Warning: Could not save metadata.\n");
        return;
    }

    fprintf(meta, "%zu\n", length);
    fclose(meta);

    printf("Data written successfully (%zu bytes).\n", length);
}

static void read_data(void)
{
    FILE *meta = fopen("metadata.txt", "r");
    if (meta == NULL)
    {
        printf("No saved data found. Write data first.\n");
        return;
    }

    size_t length;
    if (fscanf(meta, "%zu", &length) != 1 ||
        length == 0 || length >= MAX_DATA)
    {
        fclose(meta);
        printf("Invalid metadata.\n");
        return;
    }
    fclose(meta);

    char result[MAX_DATA] = {0};
    size_t offset = 0;
    uint64_t logical_block = 0;

    while (offset < length)
    {
        char buffer[BLOCK_SIZE];

        if (raid_read(logical_block, buffer, BLOCK_SIZE) != 0)
        {
            printf("Read failed.\n");
            return;
        }

        size_t chunk = length - offset;
        if (chunk > BLOCK_SIZE)
            chunk = BLOCK_SIZE;

        memcpy(result + offset, buffer, chunk);
        offset += chunk;
        logical_block++;
    }

    result[length] = '\0';

    printf("\n========== READ DATA ==========\n");
    printf("%s\n", result);
    printf("===============================\n");
}

int main(void)
{
    int choice;

    while (1)
    {
        show_menu();

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Enter a number from 1 to 5.\n");
            clear_input();
            continue;
        }

        switch (choice)
        {
            case 1:
                if (raid_status() == 0)
                    printf("All disks are available.\n");
                else
                    printf("RAID 0 cannot operate normally with a missing disk.\n");
                break;

            case 2:
                if (raid_status() == 0)
                    write_data();
                else
                    printf("Cannot write: one or more disks are unavailable.\n");
                break;

            case 3:
                if (raid_status() == 0)
                    read_data();
                else
                    printf("Cannot read: one or more disks are unavailable.\n");
                break;

            case 4:
                raid_status();
                break;

            case 5:
                printf("Exiting RaidVol.\n");
                return 0;

            default:
                printf("Invalid choice. Select 1-5.\n");
        }
    }
}
