/*
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-08-29 20:25:32
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-09-02 20:47:06
 * @FilePath: \watch\Lwatch\Module\sw_iic\sw_iic.c
 * @Description: 
 * 版权声明 保留文件所有权利 
 * Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */
#include "sw_iic.h"
#include "delay.h"
#include <stdio.h>

#if USE_SW_IIC == 1


/************************************************************************** 
                                 defines  
************************************************************************** */

#define HIGH   1    
#define LOW    0
#define ACK    0
#define NACK   1
#define WRITE  0
#define READ   1
#define GPIO_CTRL(port,pin,level)     HAL_GPIO_WritePin(port,pin,level)
#define GPIO_READ(port,pin)           HAL_GPIO_ReadPin(port,pin)
#define DELAY_US(tick_us)             mdelay_us(tick_us)




static void sw_iic_sda_out(board_iic_t* iic_handler,uint8_t level)
{
    if(level == HIGH)
    {
        GPIO_CTRL(iic_handler->sda.port,iic_handler->sda.pin,GPIO_PIN_SET);
    }
    else{
        GPIO_CTRL(iic_handler->sda.port,iic_handler->sda.pin,GPIO_PIN_RESET);
    }
}

static uint8_t sw_iic_sda_in(board_iic_t* iic_handler)
{
    return GPIO_READ(iic_handler->sda.port,iic_handler->sda.pin);
}

static void sw_iic_scl_out(board_iic_t* iic_handler,uint8_t level)
{
    if(level == HIGH)
    {
        GPIO_CTRL(iic_handler->scl.port,iic_handler->scl.pin,GPIO_PIN_SET);
    }
    else{
        GPIO_CTRL(iic_handler->scl.port,iic_handler->scl.pin,GPIO_PIN_RESET);
    }
}



/**
 * @description: 软件iic开始信号
 * @param {board_iic_t*} iic_handler
 * @return {*}
 */
static void sw_iic_start(board_iic_t* iic_handler)
{
    sw_iic_sda_out(iic_handler,HIGH);
    DELAY_US(6);
    sw_iic_scl_out(iic_handler,HIGH);
    DELAY_US(6);
    sw_iic_sda_out(iic_handler,LOW);
    DELAY_US(6);
    sw_iic_scl_out(iic_handler,LOW);
    DELAY_US(6);
}

/**
 * @description: 软件iic停止信号
 * @param {board_iic_t*} iic_handler
 * @return {*}
 */
static void sw_iic_stop(board_iic_t* iic_handler)
{
    sw_iic_scl_out(iic_handler,LOW);
    DELAY_US(6);
    sw_iic_sda_out(iic_handler,LOW);
    DELAY_US(6);
    sw_iic_scl_out(iic_handler,HIGH);
    DELAY_US(6);
    sw_iic_sda_out(iic_handler,HIGH);
    DELAY_US(6);
    
}

/**
 * @description: 软件iic接收从机应答
 * @param {board_iic_t*} iic_handler
 * @return {uint8_t} 1:应答成功 0:应答失败
 */
uint8_t sw_iic_recv_ack(board_iic_t* iic_handler)
{
    uint8_t err_time = 5;

    sw_iic_sda_out(iic_handler,HIGH); //主机释放sda
    sw_iic_scl_out(iic_handler,HIGH);
    DELAY_US(6);
    
    while(sw_iic_sda_in(iic_handler) == HIGH)
    {
        DELAY_US(6);
        if(err_time-- == 0)
        {
            sw_iic_stop(iic_handler);
            return 0; //接收不到从机应答
        }
    }

    sw_iic_scl_out(iic_handler,LOW);
    DELAY_US(6);
    return 1; //从机应答成功

}

/**
 * @description: 主机发送ack
 * @param {board_iic_t*} iic_handler
 * @param {uint8_t} ack: ACK:应答 NACK:非应答
 * @return {*}
 */
void sw_iic_send_ack(board_iic_t* iic_handler,uint8_t ack)
{
    sw_iic_sda_out(iic_handler,ack);
    sw_iic_scl_out(iic_handler,HIGH);
    DELAY_US(6);
    sw_iic_scl_out(iic_handler,LOW);
    DELAY_US(6);
}

/**
 * @description: 软件iic发送一个字节
 * @param {board_iic_t*} iic_handler
 * @param {uint8_t} byte
 * @return {*}
 */
static void sw_iic_send_byte(board_iic_t* iic_handler,uint8_t byte)
{
    for(int8_t i=7;i>=0;i--)
    {
        sw_iic_scl_out(iic_handler,LOW);
        DELAY_US(6);
        sw_iic_sda_out(iic_handler,(byte>>i) & 0x01);//高位先行
        DELAY_US(6);
        sw_iic_scl_out(iic_handler,HIGH);
        DELAY_US(6);
    }

    sw_iic_scl_out(iic_handler,LOW);
    DELAY_US(6);
}

/**
 * @description: 软件iic接收一个字节
 * @param {board_iic_t*} iic_handler
 * @param {uint8_t*} byte
 * @return {*}
 */
static void sw_iic_receive_byte(board_iic_t* iic_handler,uint8_t* byte)
{
    *byte = 0;
    for(int8_t i=7;i>=0;i--)
    {
        sw_iic_scl_out(iic_handler,LOW);
        DELAY_US(6);
        sw_iic_scl_out(iic_handler,HIGH);
        DELAY_US(6);
        *byte |= sw_iic_sda_in(iic_handler) << i;  //scl为高电平时，进行采样
    }

    sw_iic_scl_out(iic_handler,LOW);
    DELAY_US(6);
}

/**
 * @description: 软件iic向指定地址的从设备写一个字节
 * @param {board_iic_t*} iic_handler
 * @param {uint8_t} slave_addr
 * @param {uint8_t} reg_addr
 * @param {uint8_t} data
 * @return {int} 函数执行结果
 */
int sw_iic_write_one_byte(board_iic_t* iic_handler,uint8_t slave_addr,uint8_t reg_addr,uint8_t data)
{
    sw_iic_start(iic_handler);

    sw_iic_send_byte(iic_handler,slave_addr<<1 | WRITE);
    if(sw_iic_recv_ack(iic_handler) == 0)  //如果没有找到从机，则退出iic通信
    {
        sw_iic_stop(iic_handler);
        return DRV_ERR_NODEV;
    }

    sw_iic_send_byte(iic_handler,reg_addr);
    sw_iic_recv_ack(iic_handler);

    sw_iic_send_byte(iic_handler,data);
    sw_iic_recv_ack(iic_handler);

    sw_iic_stop(iic_handler);
    return DRV_OK;
}

/**
 * @description: 软件iic向指定地址的从设备写多个字节
 * @param {board_iic_t*} iic_handler
 * @param {uint8_t} slave_addr
 * @param {uint8_t} reg_addr
 * @param {uint8_t*} data
 * @param {uint32_t} len
 * @return {int} 函数执行结果
 */
int sw_iic_write_multi_byte(board_iic_t* iic_handler,uint8_t slave_addr,uint8_t reg_addr,uint8_t* data,uint32_t len)
{
    sw_iic_start(iic_handler);

    sw_iic_send_byte(iic_handler,slave_addr<<1 | WRITE);
    if(sw_iic_recv_ack(iic_handler) == 0)  //如果没有找到从机，则退出iic通信
    {
        sw_iic_stop(iic_handler);
        return DRV_ERR_NODEV;
    }

    sw_iic_send_byte(iic_handler,reg_addr);
    sw_iic_recv_ack(iic_handler);

    for(uint32_t i = 0; i < len; i++)
    {
        sw_iic_send_byte(iic_handler,data[i]);
        sw_iic_recv_ack(iic_handler);
    }

    sw_iic_stop(iic_handler);
    return DRV_OK;
}

/**
 * @description: 软件iic从指定地址的从设备读单个字节
 * @param {board_iic_t*} iic_handler
 * @param {uint8_t} slave_addr
 * @param {uint8_t} reg_addr
 * @param {uint8_t*} data
 * @return {int} 函数执行结果
 */
int sw_iic_read_one_byte(board_iic_t* iic_handler,uint8_t slave_addr,uint8_t reg_addr,uint8_t* data)
{
    sw_iic_start(iic_handler);

    sw_iic_send_byte(iic_handler,slave_addr<< 1 | WRITE);
    if(sw_iic_recv_ack(iic_handler) == 0)  //如果没有找到从机，则退出iic通信
    {
        sw_iic_stop(iic_handler);
        return DRV_ERR_NODEV;
    }

    sw_iic_send_byte(iic_handler,reg_addr);
    sw_iic_recv_ack(iic_handler);

    //发送读命令,开始读取
    sw_iic_start(iic_handler);
    sw_iic_send_byte(iic_handler,slave_addr<<1 | READ); 
    sw_iic_recv_ack(iic_handler);

    sw_iic_receive_byte(iic_handler,data);
    sw_iic_send_ack(iic_handler,NACK);

    sw_iic_stop(iic_handler);
    return DRV_OK;
}

/**
 * @description: 软件iic从指定地址的从设备读多个字节
 * @param {board_iic_t*} iic_handler
 * @param {uint8_t} slave_addr
 * @param {uint8_t} reg_addr
 * @param {uint8_t*} data
 * @param {uint32_t} len
 * @return {int} 函数执行结果
 */
int sw_iic_read_multi_byte(board_iic_t* iic_handler,uint8_t slave_addr,uint8_t reg_addr,uint8_t* data,uint32_t len)
{
    sw_iic_start(iic_handler);

    sw_iic_send_byte(iic_handler,slave_addr<< 1 | WRITE);
    if(sw_iic_recv_ack(iic_handler) == 0)  //如果没有找到从机，则退出iic通信
    {
        sw_iic_stop(iic_handler);
        return DRV_ERR_NODEV;
    }

    sw_iic_send_byte(iic_handler,reg_addr);
    sw_iic_recv_ack(iic_handler);

    sw_iic_start(iic_handler);
    sw_iic_send_byte(iic_handler,slave_addr<<1 | READ);
    sw_iic_recv_ack(iic_handler);

    for(uint32_t i = 0;i < len -1 ;i++)
    {
        sw_iic_receive_byte(iic_handler,&data[i]);
        sw_iic_send_ack(iic_handler,ACK);
    }

    sw_iic_receive_byte(iic_handler,&data[len-1]);
    sw_iic_send_ack(iic_handler,NACK);

    sw_iic_stop(iic_handler);
    return DRV_OK;
}




#endif
