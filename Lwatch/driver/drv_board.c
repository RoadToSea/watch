/*
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-08-18 12:09:16
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-08-28 16:18:44
 * @FilePath: \watch\Lwatch\driver\drv_board.c
 * @Description: 
 * 版权声明 保留文件所有权利 
 * Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */

#include "drv_board.h"


#if USE_DRV_BOARD == 1

drv_ops_t drv_board_ops = {0};

drv_board_t dev_board[] = {
    {
        .drv_name = "dev_board",
        .id = 0,
        .status = 0,
        .ops = &drv_board_ops,
        .priv_data = NULL
    }
};


WEAK_FUNC int drv_board_board_init(int id)
{
    return DRV_ERR_NOSUPPORT;
}

WEAK_FUNC int drv_board_board_open(int id)
{
    return DRV_ERR_NOSUPPORT;
}

WEAK_FUNC int drv_board_board_close(int id)
{
    return DRV_ERR_NOSUPPORT;
}

WEAK_FUNC int drv_board_board_read(int id, unsigned int addr, void* buf, unsigned int size)
{
    return DRV_ERR_NOSUPPORT;
}

WEAK_FUNC int drv_board_board_write(int id, unsigned int addr, const void* buf, unsigned int size)
{
    return DRV_ERR_NOSUPPORT;
}

int drv_board_init(drv_if_t* drv)
{
    return drv_board_board_init(drv->id);
}

int drv_board_open(drv_if_t* drv,int status)
{
    return drv_board_board_open(drv->id);
}

int drv_board_close(drv_if_t* drv,int status)
{
    return drv_board_board_close(drv->id);
}

int drv_board_read(drv_if_t* drv,unsigned int addr,void* send,unsigned int size)
{
    return drv_board_board_read(drv->id,addr,send,size);
}

int drv_board_write(drv_if_t* drv,unsigned int addr,const void* recv,unsigned int size)
{
    return drv_board_board_write(drv->id,addr,recv,size);
}

int drv_board_ctrl(drv_if_t* drv,int cmd, void* user_data)
{
    switch (cmd)
    {

    default:
        break;
    }


    return DRV_OK;
}

void board_init()
{

    drv_board_ops.init = drv_board_init;
    drv_board_ops.open = drv_board_open;
    drv_board_ops.close = drv_board_close;
    drv_board_ops.read = drv_board_read;
    drv_board_ops.write = drv_board_write;
    drv_board_ops.ctrl = drv_board_ctrl;

    for(int i=0;i<sizeof(dev_board)/sizeof(drv_board_t);i++)
    {
        drv_add(&dev_board[i]);
    }

    printf("board init\n");

}

DRIVER_REG(board_init);

#endif
