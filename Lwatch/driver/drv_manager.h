/*** 
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-08-18 12:10:03
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-09-02 11:34:28
 * @FilePath: \watch\Lwatch\driver\drv_manager.h
 * @Description: 
 * @版权声明 保留文件所有权利 
 * @Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */

#ifndef __DRV_MANAGER_H__
#define __DRV_MANAGER_H__

#include <stdio.h>
#include "drv_base.h"

/************************************************************************** 
                                 启用驱动 
************************************************************************** */
#define USE_DRV_BOARD     1
#define USE_TEMPLATE      1
#define USE_LCD           1
#define USE_SW_IIC        1
#define USE_LCD_TOUCH     1
#define USE_MPU6050       1
#define USE_SERIAL        1
#define USE_SPL006        1

/************************************************************************** 
                                 禁用驱动 
************************************************************************** */



/************************************************************************** 
                                自定义错误码   
************************************************************************** */
/* 驱动层自定义错误码可以从 -100 开始扩展 */
#define DRV_ERR_CUSTOM_BASE  (-100)




/************************************************************************** 
                                自定义设备状态   
************************************************************************** */
/* 驱动层自定义运行状态可以从 0x05 开始扩展 */
#define DRV_STATUS_CUSTOM_BASE 0x05




#endif // !__DRV_MANAGER_H__
