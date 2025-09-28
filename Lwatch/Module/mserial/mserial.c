/*
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-09-12 11:21:06
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-09-21 22:26:39
 * @FilePath: \watch\Lwatch\Module\mserial\mserial.c
 * @Description: 高速串口框架
 * 版权声明 保留文件所有权利 
 * Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */

#include "mserial.h"
#include <stdlib.h>
#include <string.h>

#if USE_MSERIAL == 1


mserial_t* mserial_init(uint32_t buffer_size,
                        mserial_stop_rx_dma_func stop_rx_dma,
                        mserial_start_rx_dma_func start_rx_dma,
                        mserial_get_rx_remain_func get_count,
                        mserial_start_tx_dma_func start_tx_dma)
{
    mserial_t* serial_handle = NULL;
    uint8_t* dma_buffer = NULL;
    uint8_t* tx_buf = NULL;
    mring_buffer_t* rx_buf = NULL;

    serial_handle = malloc(sizeof(mserial_t));
    if(serial_handle == NULL)  goto clean;


	dma_buffer = malloc(buffer_size);
    if(dma_buffer == NULL)  goto clean;

    tx_buf = malloc(buffer_size);
    if(tx_buf == NULL)  goto clean;

    rx_buf = rb_alloc(buffer_size);
    if(rx_buf == NULL)  goto clean;


    if(stop_rx_dma == NULL || start_rx_dma == NULL || get_count == NULL || start_tx_dma == NULL)
        goto clean;

    serial_handle->dma_buffer = dma_buffer;
    serial_handle->buffer_size = buffer_size;
    serial_handle->last_copy_index = 0;
    serial_handle->tx_buffer = tx_buf;
    serial_handle->rx_buffer = rx_buf;
    serial_handle->rx_loss_flag = 0;
    serial_handle->tx_busy_flag = 0;
    serial_handle->stop_rx_dma = stop_rx_dma;
    serial_handle->start_rx_dma = start_rx_dma;
    serial_handle->get_rx_remain = get_count;
    serial_handle->start_tx_dma = start_tx_dma;

    serial_handle->start_rx_dma(serial_handle->dma_buffer, serial_handle->buffer_size);
    return serial_handle;

clean:
    if(serial_handle) free(serial_handle);
    if(dma_buffer)    free(dma_buffer);
    if(tx_buf)        free(tx_buf);
    if(rx_buf)        free(rx_buf);
    return NULL;
}

/**
 * @description: 串口DMA半满回调处理函数
 * @param {mserial_t*} serial_handle
 * @return {*}
 */
void mserial_rx_dma_half_cb(mserial_t* serial_handle )
{
    serial_handle->stop_rx_dma();//停止DMA传输

    uint32_t count = serial_handle->buffer_size-serial_handle->get_rx_remain()-serial_handle->last_copy_index;  //计算本次需要拷贝的数据量
    rb_write(serial_handle->rx_buffer,serial_handle->dma_buffer+serial_handle->last_copy_index,count);
    serial_handle->last_copy_index = serial_handle->buffer_size - serial_handle->get_rx_remain();  //更新已拷贝数据的位置指针

    serial_handle->start_rx_dma(serial_handle->dma_buffer, serial_handle->buffer_size);    //重新启动DMA传输
}

/**
 * @description: 串口DMA传输完成回调处理函数
 * @param {mserial_t*} serial_handle
 * @return {*}
 */
void mserial_rx_dma_cplt_cb(mserial_t* serial_handle)
{
    serial_handle->stop_rx_dma();//停止DMA传输

    uint32_t count = serial_handle->buffer_size-serial_handle->last_copy_index;   //计算本次需要拷贝的数据量
    rb_write(serial_handle->rx_buffer,serial_handle->dma_buffer+serial_handle->last_copy_index,count);
    serial_handle->last_copy_index = 0;  //更新已拷贝数据的位置指针

    serial_handle->start_rx_dma(serial_handle->dma_buffer,serial_handle->buffer_size);    //重新启动DMA传输
}

/**
 * @description: 串口DMA传输空闲回调处理函数
 * @param {mserial_t*} serial_handle
 * @return {*}
 */
void mserial_rx_dma_idle_cb(mserial_t* serial_handle)
{
    serial_handle->stop_rx_dma();//停止DMA传输

    uint32_t count = serial_handle->buffer_size-serial_handle->get_rx_remain()-serial_handle->last_copy_index;  //计算本次需要拷贝的数据量
    rb_write(serial_handle->rx_buffer,serial_handle->dma_buffer+serial_handle->last_copy_index,count);
    serial_handle->last_copy_index = serial_handle->buffer_size - serial_handle->get_rx_remain();  //更新已拷贝数据的位置指针

    serial_handle->start_rx_dma(serial_handle->dma_buffer, serial_handle->buffer_size);    //重新启动DMA传输

}

/**
 * @description: 串口DMA发送传输完成回调处理函数
 * @param {mserial_t*} serial_handle
 * @return {*}
 */
void mserial_tx_dma_cplt_cb(mserial_t* serial_handle)
{
    serial_handle->tx_busy_flag = 0;  //发送成功
}

/**
 * @description: 串口发送函数
 * @param {mserial_t*} serial_handle
 * @param {uint8_t*} data
 * @param {uint32_t} size
 * @return {*}
 */
uint32_t mserial_write(mserial_t* serial_handle,const uint8_t* data,uint32_t size)
{
    if(serial_handle->tx_busy_flag == 1)  //如果发送速率过快正在丢包，则不发送数据
        return 0;

    if(size>serial_handle->buffer_size)   //如果发送的数据大于缓冲区大小，则只发送缓冲区大小的数据
        size = serial_handle->buffer_size;

    memcpy(serial_handle->tx_buffer,data,size);
    uint32_t ret = serial_handle->start_tx_dma(serial_handle->tx_buffer,size);
    serial_handle->tx_busy_flag = 1;  //发送中

    return size;
}

/**
 * @description: 串口接收函数
 * @param {mserial_t*} serial_handle
 * @param {uint8_t*} data
 * @param {uint32_t} size
 * @return {*}
 */
uint32_t mserial_read(mserial_t* serial_handle,uint8_t* data,uint32_t size)
{
    return rb_read(serial_handle->rx_buffer,data,size);
}


#endif
