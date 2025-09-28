#ifndef __MSERIAL_H
#define __MSERIAL_H

#include "module_config.h"


/**
 * 移植说明:
 * 1. 开启串口DMA,并设置成circle模式
 * 2. 在相应的中断函数中调用回调函数处理数据
 */


#if USE_MSERIAL == 1
#include "mring_buffer.h"

/*****************************************************************************/
/*                               define                                      */
/*****************************************************************************/
typedef void(*mserial_stop_rx_dma_func)(void);
typedef void(*mserial_start_rx_dma_func)(uint8_t*,uint32_t);
typedef uint32_t(*mserial_get_rx_remain_func)(void);
typedef int (*mserial_start_tx_dma_func)(uint8_t* , uint32_t );


struct mserial
{
    uint8_t*        dma_buffer;                //DMA缓冲区
    uint32_t        buffer_size;               //缓冲区大小
    uint32_t        last_copy_index;           //上次拷贝缓冲区的位置,下次拷贝从这个位置开始
    uint8_t*        tx_buffer;                 //发送缓冲区
    mring_buffer_t* rx_buffer;                 //环形接收缓冲区
    uint8_t         rx_loss_flag;              //传输速度过快,数据包丢失标志
    uint8_t         tx_busy_flag;              //发送繁忙标志
    mserial_stop_rx_dma_func stop_rx_dma;      //停止接收DMA
    mserial_start_rx_dma_func start_rx_dma;    //启动接收DMA
    mserial_get_rx_remain_func get_rx_remain;    //获取DMA接收的数据量
    mserial_start_tx_dma_func start_tx_dma;    //DMA发送

};
typedef struct mserial mserial_t;


/*****************************************************************************/
/*                               function                                    */
/*****************************************************************************/
mserial_t* mserial_init(uint32_t buffer_size,mserial_stop_rx_dma_func stop_func
                                            ,mserial_start_rx_dma_func start_func 
                                            ,mserial_get_rx_remain_func get_count_func
                                            ,mserial_start_tx_dma_func tx_dma_func);

void mserial_rx_dma_half_cb(mserial_t* serial_handle );

void mserial_rx_dma_cplt_cb(mserial_t* serial_handle);

void mserial_rx_dma_idle_cb(mserial_t* serial_handle);

void mserial_tx_dma_cplt_cb(mserial_t* serial_handle);

uint32_t mserial_write(mserial_t* serial_handle,const uint8_t* data,uint32_t size);

uint32_t mserial_read(mserial_t* serial_handle,uint8_t* data,uint32_t size);




#endif


#endif
