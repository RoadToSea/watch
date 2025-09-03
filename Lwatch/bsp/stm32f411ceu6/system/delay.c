/*
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-08-28 16:19:45
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-08-28 17:29:34
 * @FilePath: \watch\Lwatch\bsp\stm32f411ceu6\system\delay.c
 * @Description: 
 * 版权声明 保留文件所有权利 
 * Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */

#include "delay.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

/**
 * @description: 延时微秒
 * @param {uint32_t} us
 * @return {*}
 */
void mdelay_us(uint32_t us)
{
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;				    	 
	ticks=us*TICK_1US; 						
	told=SysTick->VAL;        				
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			
		}  
	}
}

/**
 * @description: 延时毫秒
 * @param {uint32_t} ms
 * @return {*}
 */
void mdelay_ms(uint32_t ms)
{
    for(uint32_t i=0;i<ms;i++)
    {
        mdelay_us(1000);
    }
}
