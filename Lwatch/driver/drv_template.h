#ifndef __DRV_TEMPLATE_H
#define __DRV_TEMPLATE_H

#include "drv_manager.h"

#if USE_TEMPLATE == 1

/**************************************************************************************** *
                                typedef
**************************************************************************************** */
typedef drv_if_t drv_template_t;


/**************************************************************************************** *
                                function
**************************************************************************************** */

int drv_template_board_init(int id);
int drv_template_board_open(int id);
int drv_template_board_close(int id);
int drv_template_board_read(int id, unsigned int addr, void* buf, unsigned int size);
int drv_template_board_write(int id, unsigned int addr, const void* buf, unsigned int size);

#endif

#endif /* __DRV_TEMPLATE_H */
