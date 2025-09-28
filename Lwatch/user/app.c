/*
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-08-28 20:07:40
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-09-25 10:14:12
 * @FilePath: \watch\Lwatch\user\app.c
 * @Description: 
 * 版权声明 保留文件所有权利 
 * Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */
#include "app.h"
#include "delay.h"
#include "usart.h"
#include "spl06_app.h"
#include "mpu6050_app.h"

lwatch_dev_t lwatch={0};

void app_init(void)
{
    drv_find("dev_board");

    // lwatch.bluetooth_serial = drv_find("dev_bluetooth_serial");

    spl06_app_init();
    
}

void app_run(void)
{
    
}

