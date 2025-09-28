/*
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-09-14 16:08:25
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-09-24 11:05:07
 * @FilePath: \watch\Lwatch\bsp\stm32f411ceu6\driver\stm32f411ceu6_serial.c
 * @Description: 
 * 版权声明 保留文件所有权利 
 * Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */

#include "stm32f4xx_hal.h"
#include "usart.h"
#include "drv_serial.h"


/**************************************************************************************** *
                                    defines
**************************************************************************************** */
#define BUFFER_SIZE 128

mserial_t* drv_board_serial[] = {
    0
};


void drv_bluetooth_rx_dma_stop(void)
{
    HAL_UART_DMAStop(&huart1);
}

void drv_bluetooth_rx_dma_start(uint8_t* buf, uint32_t size)
{
    HAL_UART_Receive_DMA(&huart1,buf,size);
}

uint32_t drv_bluetooth_get_rx_remain(void)
{
    return __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
}

int drv_bluetooth_tx_dma_start(uint8_t* buf, uint32_t size)
{
    int ret = DRV_ERR_IO;
    if(HAL_UART_Transmit_DMA(&huart1,buf,size) ==HAL_OK)
    {
        ret = DRV_OK;
    }
    return ret;
}

int drv_serial_board_init(int id)
{
    drv_board_serial[id] = mserial_init(BUFFER_SIZE,drv_bluetooth_rx_dma_stop,drv_bluetooth_rx_dma_start,drv_bluetooth_get_rx_remain,drv_bluetooth_tx_dma_start);

    if(drv_board_serial[id] == NULL)
        return DRV_ERR_NOINIT;
        
    //使能串口空闲中断
    __HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);
    return DRV_OK;
}

int drv_serial_board_read(int id, unsigned int addr, void* buf, unsigned int size)
{
    if(mserial_read(drv_board_serial[id],buf,size) != size)
        return DRV_ERR_IO;
    else
        return DRV_OK;
}

int drv_serial_board_write(int id, unsigned int addr, const void* buf, unsigned int size){
    if(mserial_write(drv_board_serial[id],buf,size) != size)
        return DRV_ERR_IO;
    else
        return DRV_OK;
}


/**
  * @brief This function handles USART1 global interrupt.
  */

void USART1_IRQHandler(void)
{

    HAL_UART_IRQHandler(&huart1);

    //如果空闲中断触发
    if(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE) == 1)
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);
        mserial_rx_dma_idle_cb(drv_board_serial[0]);  
    }

}
/**
  * @brief This function handles DMA2 stream2 global interrupt.
  */
void DMA2_Stream2_IRQHandler(void)
{

    if(__HAL_DMA_GET_FLAG(&hdma_usart1_rx,DMA_FLAG_HTIF2_6) == 1)  //半传输完成中断
    {
        // __HAL_DMA_CLEAR_FLAG(&hdma_usart1_rx,DMA_FLAG_HTIF2_6);
        mserial_rx_dma_half_cb(drv_board_serial[0]);
    }
  else if(__HAL_DMA_GET_FLAG(&hdma_usart1_rx,DMA_FLAG_TCIF2_6) == 1)  //传输完成中断
    {
        // __HAL_DMA_CLEAR_FLAG(&hdma_usart1_rx,DMA_FLAG_TCIF2_6);
        mserial_rx_dma_cplt_cb(drv_board_serial[0]);
    }

  HAL_DMA_IRQHandler(&hdma_usart1_rx);


}

/**
  * @brief This function handles DMA2 stream7 global interrupt.
  */
void DMA2_Stream7_IRQHandler(void)
{

  HAL_DMA_IRQHandler(&hdma_usart1_tx);

  if(__HAL_DMA_GET_FLAG(&hdma_usart1_tx,DMA_FLAG_TCIF3_7) == 1)  //传输完成中断
  {
      __HAL_DMA_CLEAR_FLAG(&hdma_usart1_tx,DMA_FLAG_TCIF3_7);
    mserial_tx_dma_cplt_cb(drv_board_serial[0]);
  }

}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        mserial_tx_dma_cplt_cb(drv_board_serial[0]);
    }
}


