#ifndef INTERVIEW_TASK1_FLASHUTIL_H
#define INTERVIEW_TASK1_FLASHUTIL_H

#include <stdbool.h>
#include <stddef.h>
#include "flashdriver.h"

/**
 * Callback type to handle an unrecoverable (fatal) error.
 */
typedef void (*FlashUtil_FatalErrorHandler)();

typedef struct FlashUtil_TransferType
{
    uint32_t offset; /** Region offset */
    uint32_t size; /** Transfer size */
    uint8_t* data; /** Transfer data */
} FlashUtilTransfer;

/**
 * Initialize flash memory.
 *
 * @return Status of the operation
 */
ErrorCode_t FlashUtil_Initialize(void);

/**
 * Write data to flash memory.
 *
 * @param transfer Pointer to transfer definition
 * @return Status of the operation
 */
ErrorCode_t FlashUtil_Write(const FlashUtilTransfer* transfer);

#endif //INTERVIEW_TASK1_FLASHUTIL_H
