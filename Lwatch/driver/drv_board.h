/*** 
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-08-18 12:09:23
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-08-18 22:51:29
 * @FilePath: \watch\Lwatch\driver\drv_board.h
 * @Description: 
 * @版权声明 保留文件所有权利 
 * @Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */
#include "drv_manager.h"


#if USE_DRV_BOARD == 1

typedef drv_if_t drv_board_t;


int drv_board_board_init(int id);
int drv_board_board_open(int id);
int drv_board_board_close(int id);
int drv_board_board_read(int id, unsigned int addr, void* buf, unsigned int size);
int drv_board_board_write(int id, unsigned int addr, const void* buf, unsigned int size);

#endif
