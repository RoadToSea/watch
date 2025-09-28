/*
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-09-09 17:49:05
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-09-12 10:43:01
 * @FilePath: \watch\Lwatch\Lib\dmp\port_mpu.c
 * @Description: 
 * 版权声明 保留文件所有权利 
 * Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */

 #include <stdint.h>

__attribute__((weak)) int port_iic_write(uint8_t addr,uint8_t reg,uint8_t data_len,uint8_t* pdata )
{
    return 1;
}

__attribute__((weak)) int port_iic_read(uint8_t addr,uint8_t reg,uint8_t data_len,uint8_t* pdata )
{
    return 1;
}

__attribute__((weak)) void port_get_ms(unsigned long* time)
{
    *time = 0;
}

__attribute__((weak)) void dmp_init(void)
{
    
}

