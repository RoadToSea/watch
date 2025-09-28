/*
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-09-14 16:10:31
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-09-14 16:10:38
 * @FilePath: \watch\Lwatch\driver\drv_serial.c
 * @Description: 
 * 版权声明 保留文件所有权利 
 * Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */
#include "drv_serial.h"

#if USE_SERIAL == 1

drv_ops_t drv_serial_ops = {0};

drv_serial_t dev_serial[] = {
    {
        .drv_name = "dev_bluetooth_serial",
        .id = 0,
        .status = DRV_STATUS_UNINIT,
        .ops = &drv_serial_ops,
        .priv_data = NULL
    }
};

 WEAK_FUNC int drv_serial_board_init(int id)
{
    return DRV_ERR_NOSUPPORT;
}

WEAK_FUNC int drv_serial_board_open(int id)
{
    return DRV_ERR_NOSUPPORT;
}

WEAK_FUNC int drv_serial_board_close(int id)
{
    return DRV_ERR_NOSUPPORT;
}

WEAK_FUNC int drv_serial_board_read(int id, unsigned int addr, void* buf, unsigned int size)
{
    return DRV_ERR_NOSUPPORT;
}

WEAK_FUNC int drv_serial_board_write(int id, unsigned int addr, const void* buf, unsigned int size)
{
    return DRV_ERR_NOSUPPORT;
}

int drv_serial_init(drv_if_t* drv)
{
    return drv_serial_board_init(drv->id);
}

int drv_serial_open(drv_if_t* drv,int status)
{
    return drv_serial_board_open(drv->id);
}

int drv_serial_close(drv_if_t* drv,int status)
{
    return drv_serial_board_close(drv->id);
}

int drv_serial_read(drv_if_t* drv,unsigned int addr,void* send,unsigned int size)
{
    return drv_serial_board_read(drv->id,addr,send,size);
}

int drv_serial_write(drv_if_t* drv,unsigned int addr,const void* recv,unsigned int size)
{
    return drv_serial_board_write(drv->id,addr,recv,size);
}

int drv_serial_ctrl(drv_if_t* drv,int cmd, void* user_data)
{
    switch (cmd)
    {

    default:
        break;
    }

    return DRV_OK;
}

void serial_init(void)
{
    drv_serial_ops.init = drv_serial_init;
    drv_serial_ops.open = drv_serial_open;
    drv_serial_ops.close = drv_serial_close;
    drv_serial_ops.read = drv_serial_read;
    drv_serial_ops.write = drv_serial_write;
    drv_serial_ops.ctrl = drv_serial_ctrl;

    for(int i=0;i<sizeof(dev_serial)/sizeof(drv_serial_t);i++)
    {
        drv_add(&dev_serial[i]);
    }

    printf("serial init ok\n");
}

DRIVER_REG(serial_init);

#endif
