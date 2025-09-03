#ifndef __DRV_LCD_H
#define __DRV_LCD_H


#include "drv_manager.h"

#if USE_LCD == 1

typedef drv_if_t drv_lcd_t;

int drv_lcd_board_init(int id);

#endif

#endif /* __DRV_LCD_H */
