/*** 
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-09-09 17:49:13
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-09-12 10:44:38
 * @FilePath: \watch\Lwatch\Lib\dmp\port_mpu.h
 * @Description: 
 * @版权声明 保留文件所有权利 
 * @Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */
#ifndef _PORT_MPU_H_
#define _PORT_MPU_H_

#include <stdint.h>

int port_iic_write(uint8_t addr,uint8_t reg,uint8_t data_len,uint8_t* pdata );

int port_iic_read(uint8_t addr,uint8_t reg,uint8_t data_len,uint8_t* pdata );

void port_get_ms(unsigned long* time);

void dmp_init(void);

#endif // !_PORT_MPU_H_
