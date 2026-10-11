
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "raid.h"

int main(void)
{
    char written[BLOCK_SIZE] = {0};
    char read_back[BLOCK_SIZE] = {0};
    char sample = 'A';
    int failures = 0;
    int total = 8;

    printf("\n========== RAIDVOL AUTOMATED TESTS ==========\n");

    /* Test 1: Write */
    strcpy(written, "RaidVol automated test");
    if (raid_write(0, written, BLOCK_SIZE) == 0)
        printf("PASS: Write operation\n");
    else { printf("FAIL: Write operation\n"); failures++; }

    /* Test 2: Read and compare */
    if (raid_read(0, read_back, BLOCK_SIZE) == 0 &&
        memcmp(written, read_back, BLOCK_SIZE) == 0)
        printf("PASS: Read and compare operation\n");
    else { printf("FAIL: Read and compare operation\n"); failures++; }

    /* Test 3: Logical block 1 -> Disk 2 */
    memset(written, 0, sizeof(written));
    memset(read_back, 0, sizeof(read_back));
    strcpy(written, "Logical block 1 test");
    if (raid_write(1, written, BLOCK_SIZE) == 0 &&
        raid_read(1, read_back, BLOCK_SIZE) == 0 &&
        memcmp(written, read_back, BLOCK_SIZE) == 0)
        printf("PASS: Second logical block operation\n");
    else { printf("FAIL: Second logical block operation\n"); failures++; }

    /* Test 4: Reject NULL write buffer */
    if (raid_write(0, NULL, BLOCK_SIZE) != 0)
        printf("PASS: Invalid write buffer rejected\n");
    else { printf("FAIL: Invalid write buffer accepted\n"); failures++; }

    /* Test 5: Reject NULL read buffer */
    if (raid_read(0, NULL, BLOCK_SIZE) != 0)
        printf("PASS: Invalid read buffer rejected\n");
    else { printf("FAIL: Invalid read buffer accepted\n"); failures++; }

    /* Test 6: Reject zero-size write */
    if (raid_write(0, &sample, 0) != 0)
        printf("PASS: Zero-size write rejected\n");
    else { printf("FAIL: Zero-size write accepted\n"); failures++; }

    /* Test 7: Logical block 2 -> Disk 3 */
    memset(written, 0, sizeof(written));
    memset(read_back, 0, sizeof(read_back));
    strcpy(written, "Logical block 2 test");
    if (raid_write(2, written, BLOCK_SIZE) == 0 &&
        raid_read(2, read_back, BLOCK_SIZE) == 0 &&
        memcmp(written, read_back, BLOCK_SIZE) == 0)
        printf("PASS: Third logical block operation\n");
    else { printf("FAIL: Third logical block operation\n"); failures++; }

    /* Test 8: Logical block 3 -> Disk 1, physical block 1 */
    memset(written, 0, sizeof(written));
    memset(read_back, 0, sizeof(read_back));
    strcpy(written, "Physical block 1 test");
    if (raid_write(3, written, BLOCK_SIZE) == 0 &&
        raid_read(3, read_back, BLOCK_SIZE) == 0 &&
        memcmp(written, read_back, BLOCK_SIZE) == 0)
        printf("PASS: Physical block 1 operation\n");
    else { printf("FAIL: Physical block 1 operation\n"); failures++; }

    printf("---------------------------------------------\n");
    printf("Total tests: %d\n", total);
    printf("Passed: %d\n", total - failures);
    printf("Failed: %d\n", failures);
    printf("RESULT: %s\n",
           failures == 0 ? "ALL TESTS PASSED" : "TESTS FAILED");
    printf("=============================================\n");

    return failures == 0 ? 0 : 1;
}
