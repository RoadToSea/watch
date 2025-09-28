#ifndef __MRING_BUFFER_H__
#define __MRING_BUFFER_H__

#include "module_config.h"

#if USE_MRING_BUFFER == 1

#include <stdint.h>

/*** 
 * @description: 
 * @param:  buffer: 缓冲区 
 *          size: 缓冲区大小
 *          write_index: 写指针,始终指向下一个要写入的位置
 *          read_index:  读指针,始终指向下一个要读取的位置
 * @return {*}
 */
struct mring_buffer
{
    uint8_t *buffer;
    uint32_t size;
    uint32_t write_index;
    uint32_t read_index;
};
typedef struct mring_buffer mring_buffer_t;


mring_buffer_t* rb_alloc( uint32_t size);

uint32_t rb_get_remain(mring_buffer_t *rb);

uint32_t rb_get_used(mring_buffer_t *rb);

uint32_t rb_write(mring_buffer_t *rb, const uint8_t *data, uint32_t size);

uint32_t rb_read(mring_buffer_t *rb, uint8_t *data, uint32_t size);

void rb_clear(mring_buffer_t *rb);



#endif

#endif // __RING_BUFFER_H__
