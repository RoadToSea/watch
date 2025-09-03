#ifndef __M_LIST_H
#define __M_LIST_H

#include "module_config.h"

#if USE_MLIST == 1


typedef struct mnode
{
    void* data;
    struct mnode* prev;
    struct mnode* next;
}mnode_t;

typedef struct m_list
{
    mnode_t* head;
    mnode_t* tail;
    unsigned int size;
}mlist_t;



/************************************************************************** 
                                defines
************************************************************************** */
//获取链表头节点后一个节点
#define MLIST_GET_BEGIN(p_list)   ((p_list) != NULL ? (p_list)->head->next : NULL)
//获取链表尾节点
#define MLIST_GET_END(p_list)   ((p_list) != NULL ? (p_list)->tail : NULL)

/************************************************************************** 
                                functions  
************************************************************************** */
mlist_t* mlist_create(void);
void mlist_destroy(mlist_t* list);

void mlist_push_back(mlist_t* list, void* data);
void mlist_push_front(mlist_t* list, void* data);
void mlist_pop_back(mlist_t* list);
void mlist_pop_front(mlist_t* list);
mnode_t* mlist_peek_back(mlist_t* list);
mnode_t* mlist_peek_front(mlist_t* list);
void mlist_clear(mlist_t* list);
mnode_t* mlist_get(mlist_t* list,unsigned int index);
void mlist_insert(mlist_t* list,unsigned int index, void* data);
void mlist_remove(mlist_t* list,unsigned int index);




#endif /* USE_MLIST */

#endif /* __M_LIST_H */
