/*
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-09-24 15:38:21
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-09-26 22:26:16
 * @FilePath: \watch\Lwatch\driver\drv_spl06.c
 * @Description: 
 * 版权声明 保留文件所有权利 
 * Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */
#include "drv_spl06.h"

#if USE_SPL006 == 1

drv_ops_t drv_spl06_ops = {0};

drv_spl06_data_t drv_spl06_data={0};

drv_spl06_t dev_spl06[] = {
    {
        .drv_name = "dev_spl06",
        .id = 0,
        .status = DRV_STATUS_UNINIT,
        .ops = &drv_spl06_ops,
        .priv_data = &drv_spl06_data
    }
};

 WEAK_FUNC int drv_spl06_board_init(int id)
{
    return DRV_ERR_NOSUPPORT;
}

WEAK_FUNC int drv_spl06_board_open(int id)
{
    return DRV_ERR_NOSUPPORT;
}

WEAK_FUNC int drv_spl06_board_close(int id)
{
    return DRV_ERR_NOSUPPORT;
}

WEAK_FUNC int drv_spl06_board_read(int id, unsigned int addr, void* buf, unsigned int size)
{
    return DRV_ERR_NOSUPPORT;
}

WEAK_FUNC int drv_spl06_board_write(int id, unsigned int addr, const void* buf, unsigned int size)
{
    return DRV_ERR_NOSUPPORT;
}

WEAK_FUNC int drv_spl06_board_read_id(int id, unsigned char* chip_id)
{
    return DRV_ERR_NOSUPPORT;
}

WEAK_FUNC int drv_spl06_board_set_mode(int id,spl06_mode_t mode)
{
    return DRV_ERR_NOSUPPORT;
}

int drv_spl06_init(drv_if_t* drv)
{
    return drv_spl06_board_init(drv->id);
}

int drv_spl06_open(drv_if_t* drv,int status)
{
    return drv_spl06_board_open(drv->id);
}

int drv_spl06_close(drv_if_t* drv,int status)
{
    return drv_spl06_board_close(drv->id);
}

int drv_spl06_read(drv_if_t* drv,unsigned int addr,void* send,unsigned int size)
{
    return drv_spl06_board_read(drv->id,addr,send,size);
}

int drv_spl06_write(drv_if_t* drv,unsigned int addr,const void* recv,unsigned int size)
{
    return drv_spl06_board_write(drv->id,addr,recv,size);
}

int drv_spl06_ctrl(drv_if_t* drv,int cmd, void* user_data)
{
    int ret = DRV_ERR_NOSUPPORT;
    switch (cmd)
    {
    case DRV_SPL06_GET_CHIP_ID:
    {
        ret =  drv_spl06_board_read_id(drv->id, user_data);
        break;
    }
    break;
    case DRV_SPL06_MODE_LOW_POWER:
    {
        ret = drv_spl06_board_set_mode(drv->id,SPL06_LOW_POWER);
        break;
    }
    case DRV_SPL06_MODE_STANDARD:
    {
        ret = drv_spl06_board_set_mode(drv->id,SPL06_STANDARD);
        break;
    }    
    case DRV_SPL06_MODE_SPORT:
    {
        ret = drv_spl06_board_set_mode(drv->id,SPL06_SPORT);
        break;
    }
    default:
        break;
    }

    return ret;
}

void spl06_init(void)
{
    drv_spl06_ops.init = drv_spl06_init;
    drv_spl06_ops.open = drv_spl06_open;
    drv_spl06_ops.close = drv_spl06_close;
    drv_spl06_ops.read = drv_spl06_read;
    drv_spl06_ops.write = drv_spl06_write;
    drv_spl06_ops.ctrl = drv_spl06_ctrl;

    for(int i=0;i<sizeof(dev_spl06)/sizeof(drv_spl06_t);i++)
    {
        drv_add(&dev_spl06[i]);
    }

    printf("spl06 init ok\n");
}

DRIVER_REG(spl06_init);

#endif

