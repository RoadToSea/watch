/*
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-09-25 10:18:12
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-09-25 10:41:42
 * @FilePath: \watch\Lwatch\user\mpu6050_app.c
 * @Description: 
 * 版权声明 保留文件所有权利 
 * Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */
#include "mpu6050_app.h"
#include "app.h"
#include "niming.h"
#include "stm32f4xx_hal.h"
#include <string.h>


void mpu6050_app_init(void)
{
    lwatch.mpu6050   = drv_find("dev_mpu6050");
    drv_mpu6050_data_t* mpu6050_data = (drv_mpu6050_data_t*)lwatch.mpu6050->priv_data;
    ano_euler_frame_t param = {.header.func_id = ANO_EULER_ANGLE,.header.dst_addr = ANO_DST_BROADCAST,.header.head=ANO_FRAME_HEAD};
 
    char debug_buf[64] = {0};
    char send_buf[64] = {0};

    while(1)
    {
        //数据读取
        int ret = drv_ctrl(lwatch.mpu6050,MPU6050_CMD_GET_ANGLE,&(mpu6050_data->angle));

        //调试打印
        // int num = snprintf(debug_buf,64,"pitch:%.2f,roll:%.2f,yaw:%.2f",mpu6050_data->angle.pitch,mpu6050_data->angle.roll,mpu6050_data->angle.yaw);
        // drv_write(lwatch.bluetooth_serial,0,debug_buf,num);
        // mdelay_ms(200);

        //数据发送
        //    param.pitch = mpu6050_data->angle.pitch;
        //    param.roll = mpu6050_data->angle.roll;
        //    param.yaw = mpu6050_data->angle.yaw;
        //    int send_len = ano_frame_data_euler(&param,send_buf,64);
        //    write_ret = drv_write(lwatch.bluetooth_serial,0,send_buf,send_len);

        //数据接收
        // int read_ret = drv_read(lwatch.bluetooth_serial,0,send_buf,64);
        // int debug_len = snprintf(debug_buf,64,"read result:%d\n",read_ret);
        // drv_write(lwatch.bluetooth_serial,0,debug_buf,debug_len);
        // mdelay_ms(50);
        // drv_write(lwatch.bluetooth_serial,0,send_buf,64);
    }
}
void mpu6050_app_run(void)
{

}

