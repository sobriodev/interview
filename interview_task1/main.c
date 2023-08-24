#include <stdio.h>
#include <stdlib.h>
#include "flashutil.h"

#define TRANSFER1_SIZE 10
#define TRANSFER1_OFFSET 0
static uint8_t transfer1_data[TRANSFER1_SIZE];
static FlashUtilTransfer transfer1 = {.offset = TRANSFER1_OFFSET, .size = TRANSFER1_SIZE, .data = &transfer1_data[0]};

#define TRANSFER2_SIZE 4099
#define TRANSFER2_OFFSET 4096
static uint8_t transfer2_data[TRANSFER1_SIZE];
static FlashUtilTransfer transfer2 = {.offset = TRANSFER2_OFFSET, .size = TRANSFER2_SIZE, .data = &transfer2_data[0]};

int main() {
    ErrorCode_t code = FlashUtil_Initialize();
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

    printf("Done!\n");
    return EXIT_SUCCESS;
}

//void FlashDriver_Init(FlashDriverCallback callback)
//{
//
//}
//
//ErrorCode_t FlashDriver_Write(uint32_t page_number, S_Array_t data, FlashDriverCallback callback)
//{
//    printf("Transfer: page[%u], len: %u, data = %p\n", page_number, data.size, data.data);
//    return SUCCESS;
//}