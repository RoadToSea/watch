/*
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-09-10 20:17:38
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-09-14 17:37:22
 * @FilePath: \watch\Lwatch\Module\mring_buffer\mring_buffer.c
 * @Description: 
 * 版权声明 保留文件所有权利 
 * Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */
#include "mring_buffer.h"
#include <string.h>
#include <stdlib.h>

#if USE_MRING_BUFFER  == 1

mring_buffer_t* rb_alloc(uint32_t size)
{
    mring_buffer_t* rb = (mring_buffer_t*)malloc(sizeof(mring_buffer_t));
    if(rb == NULL)  return NULL;
    uint8_t* buffer = (uint8_t*)malloc(size+1);
    if(buffer == NULL) {free(rb); return NULL;}
    
    rb->buffer = buffer;
    rb->size   = size+1;
    rb->read_index = 0;
    rb->write_index = 0;

    return rb;
}


/**
 * @description: get the used space of ring buffer
 * @param {mring_buffer_t} *rb
 * @return {uint32_t}  the used space size
 */
uint32_t rb_get_used(mring_buffer_t *rb)
{
    if(rb->write_index < rb->read_index)
    {
        return rb->size - rb->read_index + rb->write_index;
    }
    else 
        return rb->write_index - rb->read_index ;
}

/**
 * @description: get the free space of ring buffer
 * @param {mring_buffer_t} *rb
 * @return {uint32_t}  the free space size
 */
uint32_t rb_get_remain(mring_buffer_t *rb)
{
    return rb->size-rb_get_used(rb)-1;
}

uint32_t rb_write(mring_buffer_t *rb,const uint8_t *data, uint32_t size)
{
 
    size = rb_get_remain(rb) > size ? size : rb_get_remain(rb);  //max write size can be written
    if(size == 0) return 0;

    uint32_t has_written = 0;
    if(rb->write_index > rb->read_index)      
    {
        if(size <= rb->size-rb->write_index)  //if no overflow
        {
            memcpy(rb->buffer+rb->write_index,data,size); 
            has_written = size;
        }
        else  
        {
            has_written = rb->size - rb->write_index;
            memcpy(rb->buffer+rb->write_index,data,has_written);
            memcpy(rb->buffer,data+has_written,size-has_written);
        }
    }
    else 
    {
        memcpy(rb->buffer+rb->write_index,data,size);
    }

    rb->write_index = (rb->write_index + size)%rb->size ;
    return size;
}

uint32_t rb_read(mring_buffer_t *rb, uint8_t *data, uint32_t size)
{
    size = rb_get_used(rb) > size ? size : rb_get_used(rb);   //max size can be read
    if(size == 0) return 0;

    uint32_t has_read = 0;
    if(rb->write_index < rb->read_index)
    {
        if(size <=rb->size-rb->read_index)    //if no overflow
        {
            memcpy(data,rb->buffer+rb->read_index,size);
            has_read = size;
        }
        else 
        {
            has_read = rb->size - rb->read_index;
            memcpy(data,rb->buffer+rb->read_index,has_read);
            memcpy(data+has_read,rb->buffer,size-has_read);
        }
    }
    else
    {
        memcpy(data,rb->buffer+rb->read_index,size);
    }

    rb->read_index = (rb->read_index + size)%rb->size ;

    return size;
}

void rb_clear(mring_buffer_t *rb)
{
    rb->read_index = 0;
    rb->write_index = 0;

}


#endif

