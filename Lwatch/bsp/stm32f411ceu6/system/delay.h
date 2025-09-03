/*** 
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-08-28 16:19:52
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-08-28 16:30:30
 * @FilePath: \watch\Lwatch\bsp\stm32f411ceu6\system\delay.h
 * @Description: 
 * @版权声明 保留文件所有权利 
 * @Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */

#ifndef __DELAY_H
#define __DELAY_H

#include <stdint.h>

#define SYSCLK     100000000
#define TICK_1US   (SYSCLK/1000000)

void mdelay_us(uint32_t us);
void mdelay_ms(uint32_t ms); 



#endif /* __DELAY_H */
