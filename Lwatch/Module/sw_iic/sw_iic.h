

#ifndef __SW_IIC_H
#define __SW_IIC_H

#include "module_config.h"
#include "stm32f4xx_hal.h"

#if USE_SW_IIC == 1

/************************************************************************** 
                                 typedef  
************************************************************************** */

typedef struct 
{
    GPIO_TypeDef* port;
    uint16_t      pin;
    uint32_t      mode;
}board_iic_gpio_t;

typedef struct 
{
    board_iic_gpio_t sda;
    board_iic_gpio_t scl;
}board_iic_t;

int sw_iic_write_one_byte(board_iic_t* iic_handler,uint8_t slave_addr,uint8_t reg_addr,uint8_t data);
int sw_iic_write_multi_byte(board_iic_t* iic_handler,uint8_t slave_addr,uint8_t reg_addr,uint8_t* data,uint32_t len);
int sw_iic_read_one_byte(board_iic_t* iic_handler,uint8_t slave_addr,uint8_t reg_addr,uint8_t* data);
int sw_iic_read_multi_byte(board_iic_t* iic_handler,uint8_t slave_addr,uint8_t reg_addr,uint8_t* data,uint32_t len);


#endif


#endif /* __SW_IIC_H */
