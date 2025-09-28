/*** 
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-09-24 15:59:12
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-09-26 21:07:43
 * @FilePath: \watch\Lwatch\bsp\peripheral_reg\spl06_reg.h
 * @Description: spl06气压计寄存器定义
 * @版权声明 保留文件所有权利 
 * @Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */
#ifndef __SPL06_REG_H
#define __SPL06_REG_H

//IIC地址
#define SPL06_ADDR     0x76
//芯片ID
#define SPL06_ID         0x10
//海平面气压
#define SPL06_SEA_LEVEL_PRESSURE     101325.0f  
//海拔转换乘法系数
#define SPL06_ALTITUDE_MUTIFY        44330
//海拔转换乘方系数
#define SPL06_ALTITUDE_POWER   0.1903f


//气压数据寄存器,数据高位在前
#define SPL06_PSR_B2         0x00
#define SPL06_PSR_B1         0x01
#define SPL06_PSR_B0         0x02
//温度数据寄存器
#define SPL06_TMP_B2         0x03
#define SPL06_TMP_B1         0x04
#define SPL06_TMP_B0         0x05
//压力测量速率和采样率配置寄存器
#define SPL06_PRS_CFG        0x06
//温度测量速率和采样率配置寄存器
#define SPL06_TMP_CFG        0x07
    // ===== 温度传感器选择 (Bit 7) =====
    #define TMP_EXT_INTERNAL    0x00  // 0 << 7 = 0x00    → 内部传感器 (ASIC)
    #define TMP_EXT_EXTERNAL    0x80  // 1 << 7 = 0x80    → 外部传感器 (MEMS)
    // ===== 测量速率配置 SPL06_RATE (Bits 6-4) =====
    #define SPL06_RATE_1HZ    0x00  // 000 << 4 = 0x00   → 1 measurement/sec
    #define SPL06_RATE_2HZ    0x10  // 001 << 4 = 0x10   → 2
    #define SPL06_RATE_4HZ    0x20  // 010 << 4 = 0x20   → 4
    #define SPL06_RATE_8HZ    0x30  // 011 << 4 = 0x30   → 8
    #define SPL06_RATE_16HZ   0x40  // 100 << 4 = 0x40   → 16
    #define SPL06_RATE_32HZ   0x50  // 101 << 4 = 0x50   → 32
    #define SPL06_RATE_64HZ   0x60  // 110 << 4 = 0x60   → 64
    #define SPL06_RATE_128HZ  0x70  // 111 << 4 = 0x70   → 128

    // ===== 过采样率/分辨率配置 SPL06_PRC (Bits 3-0) =====
    #define SPL06_PRC_1X     0x00  // 0000 → 1x
    #define SPL06_PRC_2X     0x01  // 0001 → 2x (Low Power)
    #define SPL06_PRC_4X     0x02  // 0010 → 4x
    #define SPL06_PRC_8X     0x03  // 0011 → 8x
    #define SPL06_PRC_16X    0x04  // 0100 → 16x (Standard)
    #define SPL06_PRC_32X    0x05  // 0101 → 32x
    #define SPL06_PRC_64X    0x06  // 0110 → 64x (High Precision)
    #define SPL06_PRC_128X   0x07  // 0111 → 128x
//运行模式和状态寄存器
#define SPL06_MEAS_CFG               0x08
    #define STANDBY_MEAS             0x00
    #define ONCE_PRES_MEAS           0x01
    #define ONCE_TEMP_MEAS           0x02
    #define CONTINOUS_PRES_MEAS      0x05
    #define CONTINOUS_TEMP_MEAS      0x06
    #define CONTINOUS_PRES_TEMP_MEAS 0x07
//中断配置，测量数据移位，FIFO使能
#define SPL06_CFG_REG        0x09
    /* 单个位值（直接可 OR 使用） */
    #define CFG_INT_HL              0x80U  /* bit7: 中断电平 1=高电平有效, 0=低电平有效 */
    #define CFG_INT_FIFO            0x40U  /* bit6: FIFO满中断使能 */
    #define CFG_INT_PRS             0x20U  /* bit5: 压力测量完成中断使能 */
    #define CFG_INT_TMP             0x10U  /* bit4: 温度测量完成中断使能 */
    #define CFG_T_SHIFT             0x08U  /* bit3: 温度结果右移(>8x过采样需置1) */
    #define CFG_P_SHIFT             0x04U  /* bit2: 压力结果右移(>8x过采样需置1) */
    #define CFG_FIFO_EN             0x02U  /* bit1: FIFO使能 */
    #define CFG_SPI_MODE            0x01U  /* bit0: SPI模式 1=3线, 0=4线 */
//中断状态
#define SPL06_INT_STS        0x0A
//FIFO状态
#define SPL06_FIFO_STS       0x0B
//刷新FIFO,软件复位
#define SPL06_RESET          0x0C
//设备ID
#define SPL06_ID_REG         0x0D

//校准数据
#define SPL06_CALIB_C0_H        0x10
#define SPL06_CALIB_C0_L_C1_H   0x11
#define SPL96_CALIB_C1_L        0x12
#define SPL06_CALIB_C00_H       0x13
#define SPL06_CALIB_C00_L       0x14
#define SPL06_CALIB_C00_XL_C10_H   0x15
#define SPL06_CALIB_C10_L       0x16
#define SPL06_CALIB_C10_XL      0x17
#define SPL06_CALIB_C01_H       0x18
#define SPL06_CALIB_C01_L       0x19
#define SPL06_CALIB_C11_H       0x1A
#define SPL06_CALIB_C11_L       0x1B
#define SPL06_CALIB_C20_H       0x1C
#define SPL06_CALIB_C20_L       0x1D
#define SPL06_CALIB_C21_H       0x1E
#define SPL06_CALIB_C21_L       0x1F
#define SPL06_CALIB_C30_H       0x20
#define SPL06_CALIB_C30_L       0x21

/* SPL06 Oversampling Scale Factors (kP / kT)
 * 参考官方手册：Oversampling Rate -> Scale Factor
 */
#define SPL06_SCALE_OSR_1        524288.0f     /* 1x (single)        */
#define SPL06_SCALE_OSR_2        1572864.0f    /* 2x (Low Power)     */
#define SPL06_SCALE_OSR_4        3670016.0f    /* 4x                 */
#define SPL06_SCALE_OSR_8        7864320.0f    /* 8x                 */
#define SPL06_SCALE_OSR_16       253952.0f     /* 16x (Standard)     */
#define SPL06_SCALE_OSR_32       516096.0f     /* 32x                */
#define SPL06_SCALE_OSR_64       1040384.0f    /* 64x (High Precision) */
#define SPL06_SCALE_OSR_128      2088960.0f    /* 128x               */

#define SPL06_OSR_SELECT(prc)    \
    ((prc)==SPL06_PRC_1X   ? SPL06_SCALE_OSR_1   : \
    (prc)==SPL06_PRC_2X    ? SPL06_SCALE_OSR_2   : \
    (prc)==SPL06_PRC_4X    ? SPL06_SCALE_OSR_4   : \
    (prc)==SPL06_PRC_8X    ? SPL06_SCALE_OSR_8   : \
    (prc)==SPL06_PRC_16X   ? SPL06_SCALE_OSR_16  : \
    (prc)==SPL06_PRC_32X   ? SPL06_SCALE_OSR_32  : \
    (prc)==SPL06_PRC_64X   ? SPL06_SCALE_OSR_64  : \
    (prc)==SPL06_PRC_128X  ? SPL06_SCALE_OSR_128 : \
    0)


#endif /* __SPL06_REG_H */

