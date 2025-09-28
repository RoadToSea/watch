/*
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-09-02 15:07:35
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-09-02 15:52:58
 * @FilePath: \watch\Lwatch\driver\drv_mpu6050.c
 * @Description: 
 * 版权声明 保留文件所有权利 
 * Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */
#include "drv_mpu6050.h"


#if USE_MPU6050 == 1


/**************************************************************************************** *
                                    variable
**************************************************************************************** */
drv_ops_t drv_mpu6050_ops = {0};

drv_mpu6050_data_t dev_mpu6050_data[] = {
    {0}
};

drv_mpu6050_t dev_mpu6050[] = {
    {
        .drv_name = "dev_mpu6050",
        .id = 0,
        .status = DRV_STATUS_UNINIT,
        .ops = &drv_mpu6050_ops,
        .priv_data = &dev_mpu6050_data[0]
    }
};

/**************************************************************************************** *
                                    weak function
**************************************************************************************** */
 WEAK_FUNC int drv_mpu6050_board_init(int id)
{
    return DRV_ERR_NOSUPPORT;
}

WEAK_FUNC int drv_mpu6050_board_open(int id)
{
    return DRV_ERR_NOSUPPORT;
}

WEAK_FUNC int drv_mpu6050_board_close(int id)
{
    return DRV_ERR_NOSUPPORT;
}

WEAK_FUNC int drv_mpu6050_board_read(int id, unsigned int addr, void* buf, unsigned int size)
{
    return DRV_ERR_NOSUPPORT;
}

WEAK_FUNC int drv_mpu6050_board_write(int id, unsigned int addr, const void* buf, unsigned int size)
{
    return DRV_ERR_NOSUPPORT;
}

WEAK_FUNC int drv_mpu6050_board_get_chip_id(int id,uint8_t* chip_id)
{
    return DRV_ERR_NOSUPPORT;
}

WEAK_FUNC int drv_mpu6050_board_get_accel(int id,drv_mpu6050_accel_t* accel)
{
    return DRV_ERR_NOSUPPORT;
}

WEAK_FUNC int drv_mpu6050_board_get_gyro(int id,drv_mpu6050_gyro_t* gyro)
{
    return DRV_ERR_NOSUPPORT;
}

WEAK_FUNC int drv_mpu6050_board_get_angle(int id,drv_mpu6050_angle_t* angle)
{
    return DRV_ERR_NOSUPPORT;
}

/**************************************************************************************** *
                                function declares
**************************************************************************************** */
int drv_mpu6050_init(drv_if_t* drv)
{
    return drv_mpu6050_board_init(drv->id);
}

int drv_mpu6050_open(drv_if_t* drv,int status)
{
    return drv_mpu6050_board_open(drv->id);
}

int drv_mpu6050_close(drv_if_t* drv,int status)
{
    return drv_mpu6050_board_close(drv->id);
}

int drv_mpu6050_read(drv_if_t* drv,unsigned int addr,void* send,unsigned int size)
{
    return drv_mpu6050_board_read(drv->id,addr,send,size);
}

int drv_mpu6050_write(drv_if_t* drv,unsigned int addr,const void* recv,unsigned int size)
{
    return drv_mpu6050_board_write(drv->id,addr,recv,size);
}

int drv_mpu6050_ctrl(drv_if_t* drv,int cmd, void* user_data)
{
    int ret = DRV_ERR_NOSUPPORT;
    switch (cmd)
    {
    case MPU6050_CMD_GET_CHIP_ID:
    {
        ret = drv_mpu6050_board_get_chip_id(drv->id,(uint8_t*)user_data);
    }
    break;
    case MPU6050_CMD_GET_ACCEL:
    {
        ret = drv_mpu6050_board_get_accel(drv->id,(drv_mpu6050_accel_t*)user_data);
    }
    break;
    case MPU6050_CMD_GET_GYRO:
    {
        ret = drv_mpu6050_board_get_gyro(drv->id,(drv_mpu6050_gyro_t*)user_data);
    }
    break;
    case MPU6050_CMD_GET_ANGLE:
    {
        ret = drv_mpu6050_board_get_angle(drv->id,(drv_mpu6050_angle_t*)user_data);
    }
    break;
    default:
        break;
    }

    return ret;
}

void mpu6050_init(void)
{
    drv_mpu6050_ops.init = drv_mpu6050_init;
    drv_mpu6050_ops.open = drv_mpu6050_open;
    drv_mpu6050_ops.close = drv_mpu6050_close;
    drv_mpu6050_ops.read = drv_mpu6050_read;
    drv_mpu6050_ops.write = drv_mpu6050_write;
    drv_mpu6050_ops.ctrl = drv_mpu6050_ctrl;

    for(int i=0;i<sizeof(dev_mpu6050)/sizeof(drv_mpu6050_t);i++)
    {
        drv_add(&dev_mpu6050[i]);
    }

    printf("mpu6050 init ok\n");
}

DRIVER_REG(mpu6050_init);

#endif

