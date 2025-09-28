/*
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-09-10 15:55:23
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-09-10 16:39:27
 * @FilePath: \watch\Lwatch\user\task\init_task.c
 * @Description: 
 * 版权声明 保留文件所有权利 
 * Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */
#include "freertos.h"
#include "app.h"
#include "freeRTOS.h"
#include "task.h"

void init_task_run(void *argument)
{

    while (1)
    {
        drv_mpu6050_data_t* data = (drv_mpu6050_data_t*)lwatch.mpu6050->priv_data;
        int ret = drv_ctrl(lwatch.mpu6050,MPU6050_CMD_GET_ANGLE,&data->angle);
        
        // DEBUG("ctrl status:%d\n",ret);
        DEBUG("roll:%f pitch:%f yaw:%f\n",data->angle.roll,data->angle.pitch,data->angle.yaw);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}


