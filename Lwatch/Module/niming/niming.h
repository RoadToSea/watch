/*** 
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-09-22 09:00:23
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-09-23 12:20:13
 * @FilePath: \watch\Lwatch\Module\niming\niming.h
 * @Description: 实现匿名上位机通信协议的简单封装,下位机->上位机发送
 * @版权声明 保留文件所有权利 
 * @Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */

/**
 *   匿名上位机通信协议
 *   帧头           1byte   固定值0xAA
 *   目标地址       1byte   本帧数据发送到哪个设备,参见@ano_dst_addr_t
 *   功能码         1byte   本帧数据的功能,参见@ano_func_id_t
 *   数据长度       1byte   表示数据内容字段的长度
 *   数据内容       nbyte   数据内容
 *   和校验         1byte   
 *   附加校验       1byte   
 * 
 * 和校验方式:
 *    从帧头 0xAA 字节开始，一直到 DATA 区结束，对每一字节进行累加操作，只取低 8 位
 *    sumcheck += data_buf[i]; //从帧头开始，对每一字节进行求和，直到DATA区结束
 * 
 * 附加校验方式:
 *     计算和校验时，每进行一字节的加法运算，同时进行一次 SUM CHECK 的累加操作，只取低 8 位。
 *     addcheck += sumcheck; //每一字节的求和操作，进行一次sumcheck的累加
 */
#ifndef __NIMING_H__
#define __NIMING_H__

#include "module_config.h"

#if USE_NIMING == 1

/*****************************************************************************/
/*                               define                                      */
/*****************************************************************************/
#define ANO_FRAME_HEAD 0xAA
#define ANO_FRAME_ADDR ANO_DST_BROADCAST

/*** 
 * @description: 匿名数传协议目标地址
 */
typedef enum 
{
    ANO_DST_BROADCAST   = 0xFF, // 无特定目标，用于数据广播型输出
    ANO_DST_PC          = 0xAF, // 上位机
    ANO_DST_PRO_FC      = 0x05, // 拓空者 PRO 飞控
    ANO_DST_ANO_DATATR  = 0x10, // 匿名数传
    ANO_DST_ANO_DATATR_ESP = 0x11, // 匿名数传 - ESP 版
    ANO_DST_ANO_FLOW    = 0x22, // 匿名光流
    ANO_DST_ANO_UWB     = 0x30, // 匿名 UWB
    ANO_DST_LINGXIAO_IMU= 0x60, // 匿名凌霄 IMU
    ANO_DST_LINGXIAO_FC = 0x61, // 匿名凌霄飞控
    ANO_DST_CONSOLE     = 0x66  // 匿名控制台
}ano_dst_addr_t;


/*** 
 * @description: 匿名数传协议功能ID
 */
typedef enum 
{
    ANO_IMU_DATA = 0x01,                 //IMU数据
    ANO_SENSOR_DATA = 0x02,              //罗盘、气压、温度传感器数据
    ANO_EULER_ANGLE = 0x03,              //飞控姿态：欧拉角格式
    ANO_QUATERNION = 0x04                //飞控姿态：四元数格式
}ano_func_id_t;

/*** 
 * @description: 匿名数传协议头
 */
#pragma pack(1)
typedef struct 
{
    uint8_t head;
    uint8_t dst_addr;
    uint8_t func_id;
}ano_frame_header_t;
#pragma pack()

/*** 
 * @description: 匿名欧拉角数据帧
 * @return {*}
 */
typedef struct 
{
    ano_frame_header_t header;
    float pitch;
    float roll;
    float yaw;
}ano_euler_frame_t;


uint32_t ano_frame_data_euler(ano_euler_frame_t* param,uint8_t* output,uint32_t len);



#endif

#endif // !__NIMING_H__
