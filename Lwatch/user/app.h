/*** 
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-08-28 20:07:46
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-09-02 11:26:38
 * @FilePath: \watch\Lwatch\user\app.h
 * @Description: 
 * @版权声明 保留文件所有权利 
 * @Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */
#ifndef _APP_H
#define _APP_H

#include "drv_manager.h"
#include "drv_lcd.h"
#include "drv_lcd_touch.h"
#include "drv_mpu6050.h"

typedef struct 
{
    drv_lcd_touch_t* lcd_touch;
    drv_mpu6050_t* mpu6050;
}lwatch_dev_t;


void app_init(void);
void app_run(void);






#endif


