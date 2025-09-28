/*
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-08-18 16:10:27
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-09-21 12:58:16
 * @FilePath: \watch\Lwatch\bsp\stm32f411ceu6\driver\stm32f411ceu6_board.c
 * @Description: 
 * 版权声明 保留文件所有权利 
 * Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */

#include "drv_board.h"
#include "stm32f4xx.h"


#if USE_DRV_BOARD == 1

int drv_board_board_init(int id)
{
    
    /*初始化systick*/
    SysTick->LOAD  = 1000;                                   
    SysTick->VAL   = 0UL;                                    
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_ENABLE_Msk;                

    return DRV_OK;
}





#endif


