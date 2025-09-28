/*** 
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-09-24 15:38:31
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-09-26 20:19:57
 * @FilePath: \watch\Lwatch\driver\drv_spl06.h
 * @Description: 
 * @版权声明 保留文件所有权利 
 * @Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */
#ifndef __DRV_SPL06_H__
#define __DRV_SPL06_H__


#include "drv_manager.h"

#if USE_SPL006 == 1

/**************************************************************************************** *
                                typedef
**************************************************************************************** */
typedef drv_if_t drv_spl06_t;

typedef struct 
{
    float pressure;        //气压
    float temperature;     //温度
    float altitude;        //海拔
}drv_spl06_data_t;

typedef enum {
    SPL06_STANDBY,
    SPL06_LOW_POWER,
    SPL06_STANDARD,
    SPL06_SPORT
}spl06_mode_t;

typedef enum 
{
    DRV_SPL06_GET_CHIP_ID = 0x00,      //获取芯片ID
    DRV_SPL06_MODE_LOW_POWER,          //设置为低功耗模式
    DRV_SPL06_MODE_STANDARD,           //设置为标准模式
    DRV_SPL06_MODE_SPORT               //设置为运动模式
}drv_spl06_cmd_t;
/**************************************************************************************** *
                                function
**************************************************************************************** */

int drv_spl06_board_init(int id);
int drv_spl06_board_open(int id);
int drv_spl06_board_close(int id);
int drv_spl06_board_read(int id, unsigned int addr, void* buf, unsigned int size);
int drv_spl06_board_write(int id, unsigned int addr, const void* buf, unsigned int size);
int drv_spl06_board_read_id(int id, unsigned char* chip_id);
int drv_spl06_board_set_mode(int id,spl06_mode_t mode);

#endif



#endif /* __DRV_SPL06_H__ */
