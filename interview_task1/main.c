#include <stdio.h>
#include <stdlib.h>
#include "flashutil.h"

#define TRANSFER1_SIZE 10
#define TRANSFER1_OFFSET 0
static uint8_t transfer1_data[TRANSFER1_SIZE];
static FlashUtil_Transfer transfer1 = {.offset = TRANSFER1_OFFSET, .size = TRANSFER1_SIZE, .data = &transfer1_data[0]};

#define TRANSFER2_SIZE 4099
#define TRANSFER2_OFFSET 4096
static uint8_t transfer2_data[TRANSFER1_SIZE];
static FlashUtil_Transfer transfer2 = {.offset = TRANSFER2_OFFSET, .size = TRANSFER2_SIZE, .data = &transfer2_data[0]};

/* Invalid */
#define TRANSFER3_SIZE 1000
#define TRANSFER3_OFFSET 1234 /* not aligned to PAGE size */
static uint8_t transfer3_data[TRANSFER3_SIZE];
static FlashUtil_Transfer transfer3 = {.offset = TRANSFER3_OFFSET, .size = TRANSFER3_SIZE, .data = &transfer3_data[0]};

static void ErrorHandler(void)
{
    printf("Fatal error!\n");
    exit(EXIT_FAILURE);
}

int main() {
    ErrorCode_t code = FlashUtil_Initialize(&ErrorHandler);
    if (code != SUCCESS) {
        printf("Cannot initialize FLASH subsystem!\n");
        return EXIT_FAILURE;
    }

    code = FlashUtil_Write(&transfer1);
    if (code != SUCCESS) {
        printf("Cannot write to FLASH!\n");
        return EXIT_FAILURE;
    }

    code = FlashUtil_Write(&transfer2);
    if (code != SUCCESS) {
        printf("Cannot write to FLASH!\n");
        return EXIT_FAILURE;
    }

    code = FlashUtil_Write(&transfer3);
    if (code != SUCCESS) {
        printf("Cannot write to FLASH!\n");
        return EXIT_FAILURE;
    }

    printf("Done!\n");
    return EXIT_SUCCESS;
}