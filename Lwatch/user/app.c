/*
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-08-28 20:07:40
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-09-02 20:16:21
 * @FilePath: \watch\Lwatch\user\app.c
 * @Description: 
 * 版权声明 保留文件所有权利 
 * Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */
#include "app.h"
#include "delay.h"

lwatch_dev_t lwatch={0};

void app_init(void)
{
    lwatch.lcd_touch = drv_find("dev_lcd_touch");
    lwatch.mpu6050   = drv_find("dev_mpu6050");

    uint8_t chip_id = 0;
    // drv_ctrl(lwatch.lcd_touch,LCD_TOUCH_CMD_GET_CHIP_ID,&chip_id);
    

    while(1){
        int ret = drv_ctrl(lwatch.mpu6050,MPU6050_CMD_GET_CHIP_ID,&chip_id);
        printf("chip_id:%d\n",chip_id);
        mdelay_ms(500);
    }
    
}

void app_run(void)
{
    
}

