/*
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-09-24 15:41:34
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-09-26 22:23:05
 * @FilePath: \watch\Lwatch\bsp\stm32f411ceu6\driver\stm32f411ceu6_spl06.c
 * @Description: spl06_01气压传感器,使用IIC接口通信,可以读取气压和温度数据
 * 目前问题: 使用低功耗模式数据采集正常,使用正常和运动模式采集,传感器始终未准备好数据,但是数据采集自开机后一段时间波动后正常
 * 版权声明 保留文件所有权利 
 * Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */
#include "drv_spl06.h"
#include "sw_iic.h"
#include "stm32f4xx_hal.h"
#include "spl06_reg.h"
#include "delay.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

/**************************************************************** */
/*                      define                                    */
/**************************************************************** */

/**************************************************************** */
/*                      typedef                                   */
/**************************************************************** */
typedef struct 
{
    int32_t c00;
    int32_t c10;
    int16_t c20;
    int16_t c30;
    int16_t c01;
    int16_t c11;
    int16_t c21;
    int16_t c0;   
    int16_t c1;
}spl06_compensate_t;


/**************************************************************** */
/*                      variable                                  */
/**************************************************************** */
board_iic_t spl06_iic = 
{
    {
        .port = GPIOB,.pin = GPIO_PIN_13,.mode = GPIO_MODE_OUTPUT_OD
    },
    {
        .port = GPIOB,.pin = GPIO_PIN_14,.mode = GPIO_MODE_OUTPUT_PP
    }
};

spl06_compensate_t spl06_compensate={0};
static float KP = 0.0f;
static float KT = 0.0f;

static int drv_spl06_board_set_mode(spl06_mode_t mode);
static int drv_spl06_board_read_calibration(spl06_compensate_t* compensate);

/**
 * @description: 初始化io口
 *               SPL06初始化流程:
 *               1. 选择运行模式
 *               2. 选择测量频率和采样精度
 *               3. 读取校准数据
 * @param {int} id
 * @return {*}
 */
int drv_spl06_board_init(int id)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    uint8_t available = 0;
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    HAL_GPIO_WritePin(spl06_iic.sda.port, spl06_iic.sda.pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(spl06_iic.scl.port, spl06_iic.scl.pin, GPIO_PIN_SET);

    //sensor iic sda 配置
    GPIO_InitStruct.Pin = spl06_iic.sda.pin;
    GPIO_InitStruct.Mode = spl06_iic.sda.mode;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(spl06_iic.sda.port, &GPIO_InitStruct);

    //sensor iic scl 配置
    GPIO_InitStruct.Pin = spl06_iic.scl.pin;
    GPIO_InitStruct.Mode = spl06_iic.scl.mode;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(spl06_iic.scl.port, &GPIO_InitStruct);

    //延时是必须的,否则iic通讯会失败
    mdelay_ms(10);
    if(drv_spl06_board_read_id(id,&available)!= DRV_OK)
        return DRV_ERR_IO;
    if(available!=SPL06_ID)
        return DRV_ERR_NODEV;
    if(sw_iic_write_one_byte(&spl06_iic,SPL06_ADDR,SPL06_RESET,0x09)!= DRV_OK)
        return DRV_ERR_IO;
    mdelay_ms(10);

    if(drv_spl06_board_set_mode(SPL06_LOW_POWER)!= DRV_OK)
        return DRV_ERR_IO;

    //等待传感器准备就绪
    while ((available & ((1<<7)|(1<<6))) != ((1<<7)|(1<<6)))
    {
        if(sw_iic_read_one_byte(&spl06_iic, SPL06_ADDR, SPL06_MEAS_CFG, &available) != DRV_OK)
            return DRV_ERR_IO;
        printf("meas cfg:%d\n", available);
        mdelay_ms(1);
    }


    //获取校准值
    if(drv_spl06_board_read_calibration(&spl06_compensate)!= DRV_OK)
        return DRV_ERR_IO;

    return DRV_OK;
}

static int drv_spl06_board_set_mode(int id,spl06_mode_t mode)
{
    switch(mode)
    {
        case SPL06_STANDBY:
        {
            //配置为待机模式
            if(sw_iic_write_one_byte(&spl06_iic,SPL06_ADDR,SPL06_MEAS_CFG,STANDBY_MEAS)!= DRV_OK)
                return DRV_ERR_IO;
            return DRV_OK;
        }
        case SPL06_LOW_POWER:
        {
            //采样精度2x
            if(sw_iic_write_one_byte(&spl06_iic,SPL06_ADDR,SPL06_PRS_CFG,SPL06_PRC_2X)!= DRV_OK)
                return DRV_ERR_IO;
            //外部温度传感器,测量频率1hz,采样精度1x
            if(sw_iic_write_one_byte(&spl06_iic,SPL06_ADDR,SPL06_TMP_CFG,TMP_EXT_EXTERNAL|SPL06_RATE_1HZ|SPL06_PRC_1X)!= DRV_OK)
                return DRV_ERR_IO;
            KP = SPL06_OSR_SELECT(SPL06_PRC_2X);
            KT = SPL06_OSR_SELECT(SPL06_PRC_1X);
            break;
        }
        case SPL06_STANDARD:
        {
            //测量频率2hz,采样精度16x
            if(sw_iic_write_one_byte(&spl06_iic,SPL06_ADDR,SPL06_PRS_CFG,SPL06_RATE_2HZ|SPL06_PRC_16X)!= DRV_OK)
                return DRV_ERR_IO;
            //外部温度传感器,测量频率1hz,采样精度1x
            if(sw_iic_write_one_byte(&spl06_iic,SPL06_ADDR,SPL06_TMP_CFG,TMP_EXT_EXTERNAL|SPL06_RATE_1HZ|SPL06_PRC_1X)!= DRV_OK)
                return DRV_ERR_IO;
            //过采样超过8x需要移位
            if(sw_iic_write_one_byte(&spl06_iic,SPL06_ADDR,SPL06_CFG_REG,CFG_P_SHIFT|CFG_FIFO_EN)!= DRV_OK)
                return DRV_ERR_IO;
            KP = SPL06_OSR_SELECT(SPL06_PRC_16X);
            KT = SPL06_OSR_SELECT(SPL06_PRC_1X);
            break;
        }
        case SPL06_SPORT:
        {
            //测量频率4hz,采样精度64x
            if(sw_iic_write_one_byte(&spl06_iic,SPL06_ADDR,SPL06_PRS_CFG,SPL06_RATE_4HZ|SPL06_PRC_64X)!= DRV_OK)
                return DRV_ERR_IO;
            //外部温度传感器,测量频率8hz,采样精度1x
            if(sw_iic_write_one_byte(&spl06_iic,SPL06_ADDR,SPL06_TMP_CFG,TMP_EXT_EXTERNAL|SPL06_RATE_8HZ|SPL06_PRC_1X)!= DRV_OK)
                return DRV_ERR_IO;
            //过采样超过8x需要移位
            if(sw_iic_write_one_byte(&spl06_iic,SPL06_ADDR,SPL06_CFG_REG,CFG_P_SHIFT|CFG_FIFO_EN)!= DRV_OK)
                return DRV_ERR_IO;
            KP = SPL06_OSR_SELECT(SPL06_PRC_64X);
            KT = SPL06_OSR_SELECT(SPL06_PRC_1X);
            break;
        }
    }
    //配置为持续测量
    if(sw_iic_write_one_byte(&spl06_iic,SPL06_ADDR,SPL06_MEAS_CFG,CONTINOUS_PRES_TEMP_MEAS)!= DRV_OK)
        return DRV_ERR_IO;
    return DRV_OK;
}

/**
 * @description: 读取校准值,需要注意: c1只有12位有效值,同时c1高位和c0低位使用同一个寄存器,需要先去除c0的低位在左移
 * @param {spl06_compensate_t*} compensate
 * @return {*}
 */
static int drv_spl06_board_read_calibration(spl06_compensate_t* compensate)
{
    uint8_t buf[18] = {0};

    if(sw_iic_read_multi_byte(&spl06_iic,SPL06_ADDR,SPL06_CALIB_C0_H,buf,sizeof(buf)) != DRV_OK) 
        return DRV_ERR_IO;

    compensate->c0 = (buf[0] << 4) | (buf[1] >> 4) ;
    if(compensate->c0 & (1<<11))   compensate->c0 |= 0xF000;
    compensate->c1 = ((buf[1] & 0x0F) << 8) | buf[2];
    if (compensate->c1 & (1 << 11)) compensate->c1 |= 0xF000;
    compensate->c00 = (buf[3] << 12) | (buf[4] << 4) | (buf[5] >>4);
    if(compensate->c00 & (1<<19))   compensate->c00 |= 0xFFF00000;
    compensate->c10 = ((buf[5]&0x0F) << 16) | (buf[6] << 8) | (buf[7]);
    if(compensate->c10 & (1<<19))   compensate->c10 |= 0xFFF00000;
    compensate->c01 = (buf[8] <<8) | (buf[9]);
    compensate->c11 = (buf[10] << 8) | (buf[11]);
    compensate->c20 = (buf[12] << 8) | (buf[13]);
    compensate->c21 = (buf[14] << 8) | (buf[15]);
    compensate->c30 = (buf[16] << 8) | (buf[17]);

    return DRV_OK;
}

/**
 * @description: 读取压力和温度,计算公式:
    Traw_sc = Traw/kT
    Praw_sc = Praw/kP
    Pcomp(Pa) = c00+ Praw_sc*(c10 + Praw_sc *(c20+ Praw_sc *c30)) + Traw_sc*c01 + Traw_sc*Praw_sc *(c11+Praw_sc*c21)
    Tcomp (°C) = c0*0.5 + c1*Traw_sc
 * @param {spl06_compensate_t*} cps
 * @param {float*} pressure
 * @param {float*} temperature
 * @return {*}
 */
static int drv_spl06_read_prs_temp(spl06_compensate_t* cps,float* pressure,float* temperature)
{
    uint8_t buf[3] = {0};
    volatile int32_t pressure_raw_sc = 0;
    volatile int32_t temperature_raw_sc = 0;
    if(sw_iic_read_one_byte(&spl06_iic,SPL06_ADDR,SPL06_MEAS_CFG,buf) != DRV_OK)
			return DRV_ERR_IO;
    if ( (buf[0] & ((1<<4)|(1<<5))) != ((1<<4)|(1<<5))) //如果没有传感器数据准备好, ==优先级比<<高
            return DRV_ERR_BUSY;
    
    //读取原始数据
    if(sw_iic_read_multi_byte(&spl06_iic,SPL06_ADDR,SPL06_PSR_B2,buf,sizeof(buf)) != DRV_OK)
        return DRV_ERR_IO;
    pressure_raw_sc = ((int32_t)buf[0] << 16) |
                  ((int32_t)buf[1] << 8)  |
                   (int32_t)buf[2];
    if (pressure_raw_sc & 0x00800000)   // 24位有符号数最高位
        pressure_raw_sc |= 0xFF000000;

    memset(buf,0,sizeof(buf));
    if(sw_iic_read_multi_byte(&spl06_iic,SPL06_ADDR,SPL06_TMP_B2,buf,sizeof(buf)) != DRV_OK)
        return DRV_ERR_IO;
    temperature_raw_sc = ((int32_t)buf[0] << 16) |
                  ((int32_t)buf[1] << 8)  |
                   (int32_t)buf[2];
    if (temperature_raw_sc & 0x00800000)   // 24位有符号数最高位
        temperature_raw_sc |= 0xFF000000;

    volatile float pressure_disp = pressure_raw_sc/KP;
    volatile float temperature_disp = temperature_raw_sc/KT;

    //处理数据
    *pressure = cps->c00 + pressure_disp*(cps->c10 + pressure_disp*(cps->c20 + pressure_disp*cps->c30))+temperature_disp*cps->c01+temperature_disp*pressure_disp*(cps->c11+pressure_disp*cps->c21);
    *temperature = cps->c0*0.5+cps->c1*temperature_disp;

    return DRV_OK;
}

int drv_spl06_board_read(int id, unsigned int addr, void* buf, unsigned int size)
{
    drv_spl06_data_t* tmp = (drv_spl06_data_t*) buf;
    int ret = drv_spl06_read_prs_temp(&spl06_compensate,&tmp->pressure,&tmp->temperature);
    tmp->altitude = SPL06_ALTITUDE_MUTIFY * (1 - powf(tmp->pressure/SPL06_SEA_LEVEL_PRESSURE, SPL06_ALTITUDE_POWER));  
    return ret;
}

int drv_spl06_board_read_id(int id, unsigned char* chip_id)
{
    if(sw_iic_read_one_byte(&spl06_iic,SPL06_ADDR,SPL06_ID_REG,chip_id) != DRV_OK)
        return DRV_ERR_IO;
    return DRV_OK;
}

int drv_spl06_board_close(int id)
{
    return drv_spl06_board_set_mode(id,SPL06_STANDBY);
}

int drv_spl06_board_open(int id)
{
    return drv_spl06_board_set_mode(id,SPL06_LOW_POWER);
}