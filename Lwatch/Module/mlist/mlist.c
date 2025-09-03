
/*
 * @Author: RoadToSea 3376714571@qq.com
 * @Date:        2025-08-17 23:25:13
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-08-18 12:04:08
 * @FilePath: \watch\Lwatch\Module\mlist\mlist.c
 * @Description: 
 * 版权声明 保留文件所有权利 
 * Copyright (c) 2025 by RoadToSea , All Rights Reserved. 
 */


#include <string.h> 
#include <stdlib.h>
#include "mlist.h"

#if USE_MLIST == 1

/**
 * @desciption: 创建一个链表
 * @param:  无
 * @return: (mlist_t*) 链表指针
 */
mlist_t* mlist_create(void)
{
    mlist_t* list = (mlist_t*)malloc(sizeof(mlist_t));
    if(list == NULL)
        return NULL;
    
    mnode_t* head = (mnode_t*)malloc(sizeof(mnode_t));
    mnode_t* tail = (mnode_t*)malloc(sizeof(mnode_t));
    if(head == NULL || tail == NULL)
    {
        free(list);
        free(head);
        free(tail);
        return NULL;
    }
        
    head->prev = NULL;
    head->next = tail;
    head->data = NULL;
    tail->prev = head;
    tail->next = NULL;
    tail->data = NULL;
    list->size = 0;

    list->head = head;
    list->tail = tail;

    return list;
}

/**
 * @desciption: 销毁链表,用户需要自己释放节点携带的数据
 * @param:  (mlist_t*) 链表指针
 * @return: 无
 */
void mlist_destroy(mlist_t* list)
{
    if(list == NULL)
        return;
    
    mnode_t* p_iter = list->head->next;
    mnode_t* p_tail = list->tail;
    while(p_iter != p_tail)
    {
        mnode_t* p_release = p_iter;
        p_iter = p_iter->next;
        free(p_release);
    }


    free(list->head);
    free(list->tail);
    free(list);

}


/**
 * @description: 添加节点到链表尾部
 * @param {mlist_t*} 链表指针
 * @param {void*}    数据指针
 * @return {*}
 */
void mlist_push_back(mlist_t* list, void* data)
{
    if(list == NULL)
        return;
    
    mnode_t* p_node = (mnode_t*)malloc(sizeof(mnode_t));
    p_node->data = data;
    
    mnode_t* p_tail = list->tail;
    p_node->prev = p_tail->prev;
    p_tail->prev->next = p_node;
    p_tail->prev = p_node;
    p_node->next = p_tail;

    list->size++;
}

/**
 * @description: 添加节点到链表头部
 * @param {mlist_t*} 链表指针
 * @param {void*}    数据指针
 * @return {*}
 */
void mlist_push_front(mlist_t* list, void* data)
{
    if(list == NULL)
        return;

    mnode_t* p_node = (mnode_t*)malloc(sizeof(mnode_t));
    p_node->data = data;

    mnode_t* p_head = list->head;
    p_node->next = p_head->next;
    p_head->next->prev = p_node;
    p_head->next = p_node;
    p_node->prev = p_head;

    list->size++;
}

/**
 * @description: 弹出链表尾部节点
 * @param {mlist_t*} 链表指针
 * @return {*}链表节点指针
 */
void mlist_pop_back(mlist_t* list)
{
    if(list == NULL)
        return ;
    
    mnode_t* p_release = list->tail->prev;

    //如果链表为空
    if(p_release == list->head)
        return ;
    
    list->tail->prev = p_release->prev;
    list->tail->prev->next = list->tail;

    free(p_release);
    list->size--;
}

/**
 * @description: 弹出链表头部节点
 * @param {mlist_t*} 链表指针
 * @return {*}
 */
void mlist_pop_front(mlist_t* list)
{
    if(list == NULL)
        return ;

    mnode_t* p_release = list->head->next;
    if(p_release == list->tail)
        return ;

    list->head->next = p_release->next;
    list->head->next->prev = list->head;

    free(p_release);
    list->size--;
}

/**
 * @description: 查看链表尾部节点
 * @param {mlist_t*} 链表指针
 * @return {mlist_t*}尾部节点
 */
mnode_t* mlist_peek_back(mlist_t* list)
{
    if(list == NULL || list->tail->prev == list->head)
        return NULL;

    return list->tail->prev;
}

/**
 * @description: 查看链表头部节点
 * @param {mlist_t*} 链表指针
 * @return {mlist_t*}头部节点
 */
mnode_t* mlist_peek_front(mlist_t* list)
{
    if(list == NULL || list->head->next == list->tail)
        return NULL;

    return list->head->next;
}

/**
 * @description: 清空链表
 * @param {mlist_t*} 链表指针
 * @return {*}
 */
void mlist_clear(mlist_t* list)
{
    if(list == NULL)
        return ;

    mnode_t* p_iter = list->head->next;
    mnode_t* p_tail = list->tail;
    while(p_iter != p_tail)
    {
        mnode_t* p_release = p_iter;
        p_iter = p_iter->next;
        free(p_release);
    }

    list->size = 0;
}

/**
 * @description: 获取链表任意位置的节点
 * @param {mlist_t*}     链表指针
 * @param {unsigned int} 节点位置
 * @return {*}
 */
mnode_t* mlist_get(mlist_t* list,unsigned int index)
{
    if(list == NULL || index > list->size || index == 0)
        return NULL;

    mnode_t* p_iter = list->head->next;
    for(unsigned int i = 0; i < index; i++)
        p_iter = p_iter->next;

    return p_iter;  
}

/**
 * @description: 插入节点到链表任意位置
 * @param {mlist_t*} 链表指针
 * @param {int}      插入位置
 * @param {void*}    数据指针
 * @return {*}
 */
void mlist_insert(mlist_t* list,unsigned int index, void* data)
{
    if(list == NULL || index > list->size)
        return ;

    mnode_t* p_iter = list->head;
    for(unsigned int i = 0; i < index; i++)
    {
        p_iter = p_iter->next;
    }

    mnode_t* p_new = (mnode_t*)malloc(sizeof(mnode_t));
    p_new->data = data;

    p_new->next = p_iter->next;
    p_iter->next->prev = p_new;
    p_iter->next = p_new;
    p_new->prev = p_iter;

    list->size++;
}

/**
 * @description: 移除链表任意位置的节点
 * @param {mlist_t*}     链表指针
 * @param {unsigned int} 待移除的位置
 * @return {*}
 */
void mlist_remove(mlist_t* list,unsigned int index)
{
    if(list == NULL || index > list->size || index == 0)
        return ;

    mnode_t* p_iter = list->head;
    for(unsigned int i = 0; i < index; i++)
    {
        p_iter = p_iter->next;
    }

    p_iter->prev->next = p_iter->next;
    p_iter->next->prev = p_iter->prev;

    free(p_iter);
    list->size--;
}


#endif
