#ifndef INTERVIEW_TASK1_FLASHUTIL_H
#define INTERVIEW_TASK1_FLASHUTIL_H

#include <stdbool.h>
#include <stddef.h>
#include "flashdriver.h"

/**
 * Callback type to handle an unrecoverable error.
 */
typedef void (*FlashUtil_FatalErrorHandler)();

/**
 * Flash transfer definition.
 */
typedef struct FlashUtil_TransferType
{
    uint32_t offset; /** Region offset */
    uint32_t size; /** Transfer size */
    uint8_t* data; /** Transfer data */
} FlashUtil_Transfer;

/**
 * Initialize flash memory.
 * @param error_handler Optional error handler to handle unrecoverable errors.
 * @return Status of the operation
 */
ErrorCode_t FlashUtil_Initialize(FlashUtil_FatalErrorHandler error_handler);

/**
 * Write data to flash memory.
 *
 * @param transfer Pointer to transfer definition
 * @return Status of the operation
 */
ErrorCode_t FlashUtil_Write(const FlashUtil_Transfer* transfer);

#endif //INTERVIEW_TASK1_FLASHUTIL_H
