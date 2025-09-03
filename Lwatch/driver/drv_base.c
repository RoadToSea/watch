#include "drv_base.h"
#include "mlist/mlist.h"
#include <string.h>
#include <stdio.h>

static mlist_t* drv_list = NULL;
SECTION_INIT(DRIVER_SECTION);

int drv_init(drv_if_t* driver)
{
    if(driver!=NULL)
    {
        int ret = driver->ops->init(driver);
        if(ret == DRV_OK)
        {
            driver->status = DRV_STATUS_RUNNING;
        }
        return ret;
    }

    return DRV_ERR_INVAL;
}

int drv_open(drv_if_t* driver,int cmd)
{
    if(driver!=NULL)
    {
        int ret = driver->ops->open(driver,cmd);
        if(ret == DRV_OK)
        {
            driver->status = DRV_STATUS_RUNNING;
        }
        return ret;
    }

    return DRV_ERR_INVAL;
}

int drv_close(drv_if_t* driver,int cmd)
{
    if(driver!=NULL)
    {
        int ret = driver->ops->close(driver,cmd);
        if(ret == DRV_OK)
        {
            driver->status = DRV_STATUS_STOP;
        }
        return ret;
    }
    return DRV_ERR_INVAL;
}

int drv_ctrl(drv_if_t* driver,int cmd, void* user_data)
{
    if(driver!=NULL)
    { 
        if(driver->status !=DRV_STATUS_UNINIT && driver->status !=DRV_STATUS_STOP && driver->status !=DRV_STATUS_ERROR)
            return driver->ops->ctrl(driver,cmd,user_data);
    }
    return DRV_ERR_INVAL;
}

int drv_write(drv_if_t* driver,unsigned int addr,const void* send,unsigned int len)
{
    if(driver!=NULL)
    {
        if(driver->status !=DRV_STATUS_UNINIT && driver->status !=DRV_STATUS_STOP && driver->status !=DRV_STATUS_ERROR)
            return driver->ops->write(driver,addr,send,len);
    }
    return DRV_ERR_INVAL;
}

int drv_read(drv_if_t* driver,unsigned int addr,void* recv,unsigned int len)
{
    if(driver!=NULL)
    {
        if(driver->status !=DRV_STATUS_UNINIT && driver->status !=DRV_STATUS_STOP && driver->status !=DRV_STATUS_ERROR)
            return driver->ops->read(driver,addr,recv,len);
    }
    return DRV_ERR_INVAL;
}


void drv_base_init(void)
{
    drv_list = mlist_create();

    // DRIVER_FOREACH(DRIVER_SECTION,driver_init_fn_t,drv_iter)

    for(driver_init_fn_t* drv_iter=SECTION_START(DRIVER_SECTION);drv_iter<SECTION_END(DRIVER_SECTION);drv_iter++)
    {
        (*drv_iter)();
    }
}

drv_if_t* drv_find(const char* name)
{
    if(drv_list == NULL ||name == NULL)
        return NULL;

    mnode_t* p_iter = MLIST_GET_BEGIN(drv_list);

    while(p_iter != MLIST_GET_END(drv_list))
    {
        drv_if_t* tmp = (drv_if_t*)p_iter->data;
        if(strcmp(tmp->drv_name ,name) == 0)
        {
            //调用设备的初始化函数
            drv_init(tmp);
            return tmp;
        }

        p_iter = p_iter->next;
    }
    return NULL;
}

void drv_add(drv_if_t* driver)
{
    if(driver == NULL)
        return;

    mlist_push_back(drv_list,driver);
}


