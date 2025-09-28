/*
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-09-22 09:00:17
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-09-23 13:33:33
 * @FilePath: \watch\Lwatch\Module\niming\niming.c
 * @Description: 
 * 版权声明 保留文件所有权利 
 * Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */
#include "niming.h"
#include <string.h>

#if USE_NIMING == 1

/**
 * @description: 计算和校验和附加校验
 * @param {uint8_t*} buf
 * @param {uint32_t} len
 * @param {uint8_t*} sum_check
 * @param {uint8_t*} add_check
 * @return {*}
 */
void cal_check(uint8_t* buf, uint32_t len,uint8_t* sum_check,uint8_t* add_check)
{
    *sum_check = 0;
    *add_check = 0;
    for(uint32_t i=0;i<len;i++)
    {
        *sum_check+=buf[i];
        *add_check+=*sum_check;
    }
}



static uint32_t ano_frame_build(ano_frame_header_t* head,uint32_t data_len,uint8_t* output , uint32_t len)
{
    if(head == NULL || len < data_len + 6)
    {
        return 0;
    }
    uint32_t write_index = 0;

    output[write_index++] = head->head;
    output[write_index++] = head->dst_addr;
    output[write_index++] = head->func_id;
    output[write_index++] = data_len;

    write_index += data_len;

    cal_check(output,write_index,&output[write_index],&output[write_index+1]);
    write_index +=2;

    return write_index;

}

uint32_t ano_frame_data_euler(ano_euler_frame_t* param,uint8_t* output,uint32_t len)
{
    int16_t euler[3] = {param->roll*100,param->pitch*100,param->yaw*100};
    uint32_t index = sizeof(ano_frame_header_t);
    memcpy(output+index,euler,sizeof(euler));
    index += sizeof(euler);
    output[index++] = 0;

    return ano_frame_build(&param->header,index-sizeof(ano_frame_header_t),output,len);

}

#endif


