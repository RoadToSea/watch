/*** 
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-08-29 19:38:17
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-08-29 19:38:44
 * @FilePath: \watch\Lwatch\driver\drv_touch.h
 * @Description: 
 * @版权声明 保留文件所有权利 
 * @Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */
#ifndef __DRV_TOUCH_H__
#define __DRV_TOUCH_H__

#include "drv_manager.h"

#if USE_LCD_TOUCH == 1



/**************************************************************************************** *
                                typedef
**************************************************************************************** */
typedef enum
{
    LCD_TOUCH_CMD_GET_CHIP_ID,

}drv_lcd_touch_ctrl_param;


typedef drv_if_t drv_lcd_touch_t;

/**************************************************************************************** *
                                函数声明
**************************************************************************************** */
int drv_lcd_touch_board_init(int id);
int drv_lcd_touch_board_open(int id);
int drv_lcd_touch_board_close(int id);
int drv_lcd_touch_board_read(int id, unsigned int addr, void* buf, unsigned int size);
int drv_lcd_touch_board_write(int id, unsigned int addr, const void* buf, unsigned int size);

int drv_lcd_touch_board_get_chip_id(int id,uint8_t *chip_id);


#endif


#endif // !__DRV_TOUCH_H__
