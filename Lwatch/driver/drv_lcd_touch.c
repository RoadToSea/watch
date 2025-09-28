/*
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-08-29 19:38:09
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-09-21 16:30:00
 * @FilePath: \watch\Lwatch\driver\drv_lcd_touch.c
 * @Description: 
 * 版权声明 保留文件所有权利 
 * Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */

#include "drv_lcd_touch.h"

#if USE_LCD_TOUCH == 1

drv_ops_t drv_lcd_touch_ops = {0};

drv_lcd_touch_t dev_lcd_touch[] = {
    {
        .drv_name = "dev_lcd_touch",
        .id = 0,
        .status = DRV_STATUS_UNINIT,
        .ops = &drv_lcd_touch_ops,
        .priv_data = NULL
    }
};

 WEAK_FUNC int drv_lcd_touch_board_init(int id)
{
    return DRV_ERR_NOSUPPORT;
}

WEAK_FUNC int drv_lcd_touch_board_open(int id)
{
    return DRV_ERR_NOSUPPORT;
}

WEAK_FUNC int drv_lcd_touch_board_close(int id)
{
    return DRV_ERR_NOSUPPORT;
}

WEAK_FUNC int drv_lcd_touch_board_read(int id, unsigned int addr, void* buf, unsigned int size)
{
    return DRV_ERR_NOSUPPORT;
}

WEAK_FUNC int drv_lcd_touch_board_write(int id, unsigned int addr, const void* buf, unsigned int size)
{
    return DRV_ERR_NOSUPPORT;
}

WEAK_FUNC int drv_lcd_touch_board_get_chip_id(int id,uint8_t *chip_id)
{
    return DRV_ERR_NOSUPPORT;
}

int drv_lcd_touch_init(drv_if_t* drv)
{
    return drv_lcd_touch_board_init(drv->id);
}

int drv_lcd_touch_open(drv_if_t* drv,int status)
{
    return drv_lcd_touch_board_open(drv->id);
}

int drv_lcd_touch_close(drv_if_t* drv,int status)
{
    return drv_lcd_touch_board_close(drv->id);
}

int drv_lcd_touch_read(drv_if_t* drv,unsigned int addr,void* send,unsigned int size)
{
    return drv_lcd_touch_board_read(drv->id,addr,send,size);
}

int drv_lcd_touch_write(drv_if_t* drv,unsigned int addr,const void* recv,unsigned int size)
{
    return drv_lcd_touch_board_write(drv->id,addr,recv,size);
}

int drv_lcd_touch_ctrl(drv_if_t* drv,int cmd, void* user_data)
{
    int ret = DRV_ERR_NOSUPPORT;
    switch (cmd)
    {
    case LCD_TOUCH_CMD_GET_CHIP_ID:
        ret = drv_lcd_touch_board_get_chip_id(drv->id,(uint8_t*)user_data);
    break;
    default:
        break;
    }

    return ret;
}

void lcd_touch_init(void)
{
    drv_lcd_touch_ops.init = drv_lcd_touch_init;
    drv_lcd_touch_ops.open = drv_lcd_touch_open;
    drv_lcd_touch_ops.close = drv_lcd_touch_close;
    drv_lcd_touch_ops.read = drv_lcd_touch_read;
    drv_lcd_touch_ops.write = drv_lcd_touch_write;
    drv_lcd_touch_ops.ctrl = drv_lcd_touch_ctrl;

    for(int i=0;i<sizeof(dev_lcd_touch)/sizeof(drv_lcd_touch_t);i++)
    {
        drv_add(&dev_lcd_touch[i]);
    }

    printf("lcd_touch init ok\n");
}

DRIVER_REG(lcd_touch_init);


#endif

