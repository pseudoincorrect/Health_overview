#include "gen_fifo.h"
#include "SEGGER_RTT.h"
// gen_fifo for general data type fifo
// p_dest_data for pointer to destination data (destination data to be written from fifo)
// p_src_data  for pointer to source data (source data to be copied to fifo)



/**@brief Put one byte to the FIFO. */
static __INLINE void gen_fifo_put(gen_fifo_t * p_fifo, void* p_src_data)
{
    memcpy( (p_fifo->p_buf) + ((p_fifo->write_pos & p_fifo->buf_size_mask) * p_fifo->size_element),
            p_src_data,
            p_fifo->size_element);

    p_fifo->write_pos++;
}


/**@brief Look at one byte in the FIFO. */
static __INLINE void gen_fifo_peek(gen_fifo_t * p_fifo, uint16_t index, void* p_dest_data)
{
    memcpy( p_dest_data,
            (p_fifo->p_buf + (((p_fifo->read_pos + index) & p_fifo->buf_size_mask) * p_fifo->size_element)),
            p_fifo->size_element);
}


/**@brief Get one byte from the FIFO. */
static __INLINE void gen_fifo_get(gen_fifo_t * p_fifo, void* p_dest_data)
{
    gen_fifo_peek(p_fifo, 0, p_dest_data);
    p_fifo->read_pos++;
}


uint32_t gen_fifo_length(gen_fifo_t * p_fifo)
{
    uint32_t tmp = p_fifo->read_pos;
    return p_fifo->write_pos - tmp;
}


uint32_t gen_fifo_init( gen_fifo_t * p_fifo, void * p_buf,
                        uint16_t buf_size, uint32_t element_size, uint32_t buff_sizeof)
{
    // Check buffer for null pointer.
    if (p_buf == NULL)
    {
        return NRF_ERROR_NULL;
    }

    // Check that the buffer size is a power of two, for index intentional wrapping/overflow.
    if (!IS_POWER_OF_TWO(buf_size))
    {
        return NRF_ERROR_INVALID_LENGTH;
    }

    if (buff_sizeof != (buf_size * element_size))
    {
        return NRF_ERROR_INVALID_LENGTH;
    }

    p_fifo->p_buf         = p_buf;
    p_fifo->buf_size_mask = buf_size - 1;
    p_fifo->read_pos      = 0;
    p_fifo->write_pos     = 0;
    p_fifo->size_element  = element_size;

    return NRF_SUCCESS;
}


uint32_t gen_fifo_write(gen_fifo_t * p_fifo, void* p_src_data)
{
    if (gen_fifo_length(p_fifo) <= p_fifo->buf_size_mask)
    {
        gen_fifo_put(p_fifo, p_src_data);
        return NRF_SUCCESS;
    }

    return NRF_ERROR_NO_MEM;
}


uint32_t gen_fifo_read(gen_fifo_t * p_fifo, void* p_dest_data)
{
    if (gen_fifo_length(p_fifo) != 0)
    {
        gen_fifo_get(p_fifo, p_dest_data);
        return NRF_SUCCESS;
    }
    return NRF_ERROR_NOT_FOUND;
}


uint32_t gen_fifo_read_only(gen_fifo_t * p_fifo, uint16_t index, void * p_dest_data)
{
    if (gen_fifo_length(p_fifo) > index)
    {
        gen_fifo_peek(p_fifo, index, p_dest_data);
        return NRF_SUCCESS;
    }
    return NRF_ERROR_NOT_FOUND;
}


uint32_t gen_fifo_flush(gen_fifo_t * p_fifo)
{
    p_fifo->read_pos = p_fifo->write_pos;
    return NRF_SUCCESS;
}

