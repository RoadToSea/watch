/*** 
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-09-02 15:07:44
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-09-02 15:07:50
 * @FilePath: \watch\Lwatch\driver\drv_mpu6050.h
 * @Description: 
 * @版权声明 保留文件所有权利 
 * @Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */
#ifndef __DRV_MPU6050_H
#define __DRV_MPU6050_H


#include "drv_manager.h"

#if USE_MPU6050 == 1



/**************************************************************************************** *
                                typedef
**************************************************************************************** */
typedef enum
{
    MPU6050_CMD_GET_CHIP_ID,

}drv_mpu6050_ctrl_param;


typedef drv_if_t drv_mpu6050_t;

/**************************************************************************************** *
                                函数声明
**************************************************************************************** */
int drv_mpu6050_board_init(int id);
int drv_mpu6050_board_open(int id);
int drv_mpu6050_board_close(int id);
int drv_mpu6050_board_read(int id, unsigned int addr, void* buf, unsigned int size);
int drv_mpu6050_board_write(int id, unsigned int addr, const void* buf, unsigned int size);

int drv_mpu6050_board_get_chip_id(int id,uint8_t* chip_id);

#endif




#endif /* __DRV_MPU6050_H */
