#ifndef INTERVIEW_TASK1_FLASHDRIVER_H
#define INTERVIEW_TASK1_FLASHDRIVER_H

#include <stdint.h>

#define PAGE_SIZE 2048U

typedef struct S_Array_type
{
    uint8_t* data; ///< Pointer to data
    uint32_t size; ///< Length of data
} S_Array_t;

typedef enum ErrorCode_type
{
    SUCCESS, ///< Operation finished with success
    PENDING, ///< Some operation is pending in flashdriver, cannot run any other. This may be returned only from API
///< and not callback
    INVALID_ARGS, ///< Invalid arguments passed to interface. This may be returned only from API and not callback
    ERROR ///< Generic, unrecoverable error
} ErrorCode_t;

/**
* @brief Callback used to report status of operation.
* @attention Such callback is always called on another context than Your code executes
*/
typedef void (*FlashDriverCallback)(ErrorCode_t code);

/**
* @brief Initialize module async
* @param callback Called when operation finished resulting in appropriate error code.
*/
void FlashDriver_Init(FlashDriverCallback callback);

/**
* @brief Write data to flash.
* The writing starts always on beginning of given page.
* @param page_number Page number You write to.
* @param data Data to be written
* @param callback Called on operation finish
*/
ErrorCode_t FlashDriver_Write(uint32_t page_number, S_Array_t data, FlashDriverCallback callback);

/**
* Please implement small module that uses above interface (You shall not implement given interface, but use it in Your module as given),
* providing to the user's application possibility of writing data to given flash
* region, using linear addressing mode (0x0 - X and not page numbers). The module shall have at least:
* - initialization function
* - write function
* - other needed with description
*
* The module shall be synchronous from the user perspective ( blocking ). Please provide small example of such module usage.
* Please keep it simple.
* You may use C11.
*/

#endif //INTERVIEW_TASK1_FLASHDRIVER_H
