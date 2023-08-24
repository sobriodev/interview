#include "flashutil.h"

/**** Private types ****/

#define FLASHUTIL_REGION_LENGTH 20480U
#define FLASHUTIL_PAGE_ID(offset) ((offset) / PAGE_SIZE)

typedef struct FlashUtil_ConfigType
{
    bool initialized;
    bool corrupted;
    volatile bool driver_operation_completed;
    volatile ErrorCode_t driver_recent_code;
    FlashUtil_FatalErrorHandler error_handler;
} FlashUtil_Config;

/**** Private variables ****/

static FlashUtil_Config flash_config = {.initialized = false, .corrupted = false, .driver_operation_completed = true,
                                       .driver_recent_code = SUCCESS, .error_handler = NULL};

/**** Private functions ****/

/* Callback function to reap flash subsystem results */
static void FlashUtil_OperationCallback(ErrorCode_t code)
{
    flash_config.driver_recent_code = code;
    flash_config.driver_operation_completed = true;
}

/* Helper function to busy wait for flash operation completeness */
static inline ErrorCode_t FlashUtil_ReapStatus(void)
{
    uint32_t counter = UINT32_MAX;
    while (!flash_config.driver_operation_completed && --counter > 0) {
        asm("");
    }

    if (!counter) {
        /* Consider no response from flash driver as non-recoverable error */
        return ERROR;
    }
    return flash_config.driver_recent_code;
}

/* Get flash driver recent status with regard to user error callback */
static inline ErrorCode_t FlashUtil_GetOpCode()
{
    ErrorCode_t code = FlashUtil_ReapStatus();
    if (ERROR == code) {
        if (NULL != flash_config.error_handler) {
            flash_config.error_handler();
            /* noreturn */
        } else {
            flash_config.corrupted = true;
        }
    }
    return code;
}

/* Helper function to perform N consecutive flash writes to respective pages */
static ErrorCode_t FlashUtil_PerformTransfer(const FlashUtilTransfer* transfer)
{
    if (!transfer->size) {
        return INVALID_ARGS;
    }
    uint32_t end_addr = transfer->offset + transfer->size - 1;
    if (transfer->offset % PAGE_SIZE || /* write not aligned to PAGE_SIZE */
        end_addr >= FLASHUTIL_REGION_LENGTH)  /* too much data */
    {
        return INVALID_ARGS;
    }

    uint32_t start_page = FLASHUTIL_PAGE_ID(transfer->offset);
    uint32_t end_page = FLASHUTIL_PAGE_ID(end_addr);
    S_Array_t buffer;
    for (uint32_t page = start_page; page <= end_page; ++page) {
        uint32_t transferred_byes = (page - start_page) * PAGE_SIZE;
        buffer.data = &transfer->data[transferred_byes];
        buffer.size = transfer->size - transferred_byes;
        if (buffer.size > PAGE_SIZE) {
            buffer.size = PAGE_SIZE;
        }

        flash_config.driver_operation_completed = false;
        FlashDriver_Write(page, buffer, &FlashUtil_OperationCallback);
        ErrorCode_t code = FlashUtil_GetOpCode();
        if (SUCCESS != code) {
            return code;
        }
    }

    return SUCCESS;
}

/**** API functions ****/

ErrorCode_t FlashUtil_Initialize(void)
{
    if (flash_config.initialized && !flash_config.corrupted) {
        return SUCCESS;
    }

    flash_config.driver_operation_completed = false;
    FlashDriver_Init(&FlashUtil_OperationCallback);
    ErrorCode_t code = FlashUtil_GetOpCode();
    if (SUCCESS == code) {
        flash_config.initialized = true;
    }
    return code;
}

ErrorCode_t FlashUtil_Write(const FlashUtilTransfer* transfer)
{
    if (!flash_config.initialized) {
        /* Let's assume uninitialized flash subsystem results in INVALID_ARGS error */
        return INVALID_ARGS;
    } else if (flash_config.corrupted) {
        return ERROR;
    }
    return FlashUtil_PerformTransfer(transfer);
}
