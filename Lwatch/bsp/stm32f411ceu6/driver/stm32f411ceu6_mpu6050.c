/*
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-09-02 15:13:26
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-09-02 20:14:39
 * @FilePath: \watch\Lwatch\bsp\stm32f411ceu6\driver\stm32f411ceu6_mpu6050.c
 * @Description: stm32f411ceu6 mpu6050底层驱动
 * 版权声明 保留文件所有权利 
 * Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */

 #include "drv_mpu6050.h"
 #include "sw_iic.h"
 #include "stm32f4xx_hal.h"
 #include "mpu6050_reg.h"




 board_iic_t mpu6050_iic = 
{
    {
        .port = GPIOB,.pin = GPIO_PIN_13,.mode = GPIO_MODE_OUTPUT_OD
    },
    {
        .port = GPIOB,.pin = GPIO_PIN_14,.mode = GPIO_MODE_OUTPUT_PP
    }
};

int drv_mpu6050_board_init(int id)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    HAL_GPIO_WritePin(mpu6050_iic.sda.port, mpu6050_iic.sda.pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(mpu6050_iic.scl.port, mpu6050_iic.scl.pin, GPIO_PIN_SET);

    //sensor iic sda 配置
    GPIO_InitStruct.Pin = mpu6050_iic.sda.pin;
    GPIO_InitStruct.Mode = mpu6050_iic.sda.mode;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(mpu6050_iic.sda.port, &GPIO_InitStruct);

    //sensor iic scl 配置
    GPIO_InitStruct.Pin = mpu6050_iic.scl.pin;
    GPIO_InitStruct.Mode = mpu6050_iic.scl.mode;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(mpu6050_iic.scl.port, &GPIO_InitStruct);
		
    
    // sw_iic_write_one_byte(&mpu6050_iic,MPU_ADDR,MPU_MOTION_DET_REG,0x01);    //set the acceleration threshold is (LSB*2)mg
    // sw_iic_write_one_byte(&mpu6050_iic,MPU_ADDR,MPU_MOTION_DUR_REG,0x01);    //Acceleration detection time is ()ms 
    // sw_iic_write_one_byte(&mpu6050_iic,MPU_ADDR,MPU_INTBP_CFG_REG,0X90);     //INT Pin active low level, reset until 50us
    // sw_iic_write_one_byte(&mpu6050_iic,MPU_ADDR,MPU_INT_EN_REG,0x40);        //enable INT
    return DRV_OK;
}

int drv_mpu6050_board_get_chip_id(int id,uint8_t* chip_id)
{
    return sw_iic_read_one_byte(&mpu6050_iic,MPU_ADDR,MPU_DEVICE_ID_REG,chip_id);
}

