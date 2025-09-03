#include "drv_template.h"

#if USE_TEMPLATE == 1

drv_ops_t drv_template_ops = {0};

drv_template_t dev_template[] = {
    {
        .drv_name = "dev_template_1",
        .id = 0,
        .status = DRV_STATUS_UNINIT,
        .ops = &drv_template_ops,
        .priv_data = NULL
    }
};

 WEAK_FUNC int drv_template_board_init(int id)
{
    return DRV_ERR_NOSUPPORT;
}

WEAK_FUNC int drv_template_board_open(int id)
{
    return DRV_ERR_NOSUPPORT;
}

WEAK_FUNC int drv_template_board_close(int id)
{
    return DRV_ERR_NOSUPPORT;
}

WEAK_FUNC int drv_template_board_read(int id, unsigned int addr, void* buf, unsigned int size)
{
    return DRV_ERR_NOSUPPORT;
}

WEAK_FUNC int drv_template_board_write(int id, unsigned int addr, const void* buf, unsigned int size)
{
    return DRV_ERR_NOSUPPORT;
}

int drv_template_init(drv_if_t* drv)
{
    return drv_template_board_init(drv->id);
}

int drv_template_open(drv_if_t* drv,int status)
{
    return drv_template_board_open(drv->id);
}

int drv_template_close(drv_if_t* drv,int status)
{
    return drv_template_board_close(drv->id);
}

int drv_template_read(drv_if_t* drv,unsigned int addr,void* send,unsigned int size)
{
    return drv_template_board_read(drv->id,addr,send,size);
}

int drv_template_write(drv_if_t* drv,unsigned int addr,const void* recv,unsigned int size)
{
    return drv_template_board_write(drv->id,addr,recv,size);
}

int drv_template_ctrl(drv_if_t* drv,int cmd, void* user_data)
{
    switch (cmd)
    {

    default:
        break;
    }

    return DRV_OK;
}

void template_init(void)
{
    drv_template_ops.init = drv_template_init;
    drv_template_ops.open = drv_template_open;
    drv_template_ops.close = drv_template_close;
    drv_template_ops.read = drv_template_read;
    drv_template_ops.write = drv_template_write;
    drv_template_ops.ctrl = drv_template_ctrl;

    for(int i=0;i<sizeof(dev_template)/sizeof(drv_template_t);i++)
    {
        drv_add(&dev_template[i]);
    }

    printf("template init ok");
}

DRIVER_REG(template_init);

#endif
