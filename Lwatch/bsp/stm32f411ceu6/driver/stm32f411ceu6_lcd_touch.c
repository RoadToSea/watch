/*
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-08-29 20:14:32
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-09-02 17:24:21
 * @FilePath: \watch\Lwatch\bsp\stm32f411ceu6\driver\stm32f411ceu6_lcd_touch.c
 * @Description: 
 * 版权声明 保留文件所有权利 
 * Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */
#include "drv_lcd_touch.h"
#include "sw_iic.h"
#include "stm32f4xx_hal.h"

#if USE_LCD_TOUCH == 1

/************************************************************************** 
                                 defines  
************************************************************************** */
#define LCD_TOUCH_ADDR      0x15

#define CHIP_ID_REG         0xA7

board_iic_t lcd_iic = 
{
    {
        .port = GPIOB,.pin = GPIO_PIN_4,.mode = GPIO_MODE_OUTPUT_OD
    },
    {
        .port = GPIOB,.pin = GPIO_PIN_6,.mode = GPIO_MODE_OUTPUT_PP
    }
};


int drv_lcd_touch_board_init(int id)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    HAL_GPIO_WritePin(lcd_iic.sda.port, lcd_iic.sda.pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(lcd_iic.scl.port, lcd_iic.scl.pin, GPIO_PIN_RESET);

    //lcd iic sda 配置
    GPIO_InitStruct.Pin = lcd_iic.sda.pin;
    GPIO_InitStruct.Mode = lcd_iic.sda.mode;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(lcd_iic.sda.port, &GPIO_InitStruct);

    //lcd iic scl 配置
    GPIO_InitStruct.Pin = lcd_iic.scl.pin;
    GPIO_InitStruct.Mode = lcd_iic.scl.mode;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(lcd_iic.scl.port, &GPIO_InitStruct);
		
    return DRV_OK;
}

int drv_lcd_touch_board_get_chip_id(int id,uint8_t *chip_id)
{
    return sw_iic_read_one_byte(&lcd_iic,LCD_TOUCH_ADDR,CHIP_ID_REG,chip_id);
}


#endif
