/*
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-09-25 10:14:14
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-09-25 10:57:23
 * @FilePath: \watch\Lwatch\user\spl06_app.c
 * @Description: 
 * 版权声明 保留文件所有权利 
 * Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */
#include "app.h"
#include "spl06_app.h"
#include <stdio.h>
#include "delay.h"

void spl06_app_init(void)
{
    lwatch.spl06 = drv_find("dev_spl06");
    drv_spl06_data_t* spl06_data = (drv_spl06_data_t*)lwatch.spl06->priv_data;
    uint8_t id = 0;

    while(1)
    {
        drv_ctrl(lwatch.spl06,DRV_SPL06_GET_CHIP_ID,&id);
        int ret = drv_read(lwatch.spl06,0,spl06_data,sizeof(spl06_data));
        printf("ret:%d,id:%d,pressure:%.2f,temperature:%.2f,altitude:%.2f\n",ret,id,spl06_data->pressure,spl06_data->temperature,spl06_data->altitude);
        mdelay_ms(500);
    }
}


void spl06_app_run(void)
{

}

