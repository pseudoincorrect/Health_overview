#ifndef __GEN_FIFO_H__
#define __GEN_FIFO_H__

#include <stdint.h>
#include <stdlib.h>
#include "sdk_common.h"

/**@brief   A FIFO instance structure.
 * @details Keeps track of which bytes to read and write next.
 *          Also, it keeps the information about which memory is allocated for the buffer
 *          and its size. This structure must be initialized by gen_fifo_init() before use.
 */
typedef struct
{
    void *             p_buf;           // Pointer to FIFO buffer memory.
    uint32_t           size_element;		// Size of an element
    uint16_t           buf_size_mask;   // Read/write index mask. Also used for size checking.
    volatile uint32_t  read_pos;        // Next read position in the FIFO buffer.
    volatile uint32_t  write_pos;       // Next write position in the FIFO buffer.
} gen_fifo_t;


uint32_t gen_fifo_length(gen_fifo_t * p_fifo);

/**@brief Function for initializing the FIFO.
 *
 * @param[out] p_fifo   FIFO object.
 * @param[in]  p_buf    FIFO buffer for storing data. The buffer size must be a power of two.
 * @param[in]  buf_size Size of the FIFO buffer provided. This size must be a power of two.
 *
 * @retval     NRF_SUCCESS              If initialization was successful.
 * @retval     NRF_ERROR_NULL           If a NULL pointer is provided as buffer.
 * @retval     NRF_ERROR_INVALID_LENGTH If size of buffer provided is not a power of two.
 */
uint32_t gen_fifo_init( gen_fifo_t * p_fifo, void * p_buf,
                        uint16_t buf_size, uint32_t element_size, uint32_t buff_sizeof);

/**@brief Function for adding an element to the FIFO.
 *
 * @param[in]  p_fifo   Pointer to the FIFO.
 * @param[in]  byte     Data byte to add to the FIFO.
 *
 * @retval     NRF_SUCCESS              If an element has been successfully added to the FIFO.
 * @retval     NRF_ERROR_NO_MEM         If the FIFO is full.
 */
uint32_t gen_fifo_write(gen_fifo_t * p_fifo, void* p_src_data);

/**@brief Function for getting the next element from the FIFO.
 *
 * @param[in]  p_fifo   Pointer to the FIFO.
 * @param[out] p_byte   Byte fetched from the FIFO.
 *
 * @retval     NRF_SUCCESS              If an element was returned.
 * @retval     NRF_ERROR_NOT_FOUND      If there are no more elements in the queue.
 */
uint32_t gen_fifo_read(gen_fifo_t * p_fifo, void* p_dest_data);

/**@brief Function for looking at an element in the FIFO, without consuming it.
 *
 * @param[in]  p_fifo   Pointer to the FIFO.
 * @param[in]  index    Which element to look at. The lower the index, the earlier it was put.
 * @param[out] p_byte   Byte fetched from the FIFO.
 *
 * @retval     NRF_SUCCESS              If an element was returned.
 * @retval     NRF_ERROR_NOT_FOUND      If there are no more elements in the queue, or the index was
 *                                      too large.
 */
uint32_t gen_fifo_read_only(gen_fifo_t * p_fifo, uint16_t index, void * p_dest_data);

/**@brief Function for flushing the FIFO.
 *
 * @param[in]  p_fifo   Pointer to the FIFO.
 *
 * @retval     NRF_SUCCESS              If the FIFO was flushed successfully.
 */
uint32_t gen_fifo_flush(gen_fifo_t * p_fifo);


#endif
