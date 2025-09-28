/*** 
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-09-02 15:38:10
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-09-03 18:13:12
 * @FilePath: \watch\Lwatch\bsp\peripheral_reg\mpu6050_reg.h
 * @Description: 
 * @版权声明 保留文件所有权利 
 * @Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */
#ifndef __MPU6050_H__
#define __MPU6050_H__


#define MPU6050_SELF_TESTX_REG		0X0D	//自检寄存器X
#define MPU6050_SELF_TESTY_REG		0X0E	//自检寄存器Y
#define MPU6050_SELF_TESTZ_REG		0X0F	//自检寄存器Z
#define MPU6050_SELF_TESTA_REG		0X10	//自检寄存器A
#define MPU6050_SAMPLE_RATE_REG		0X19	//采样频率分频器
#define MPU6050_CFG_REG						0X1A	//配置寄存器
#define MPU6050_GYRO_CFG_REG			0X1B	//陀螺仪配置寄存器
    /*********************  @MPU6050_GYRO_RANGE *******************************/
    #define MPU6050_GYRO_RANGE_250DPS      (0x00 << 3)
    #define MPU6050_GYRO_RANGE_500DPS      (0x01 << 3)
    #define MPU6050_GYRO_RANGE_1000DPS     (0x02 << 3)
    #define MPU6050_GYRO_RANGE_2000DPS     (0x03 << 3)
    /**************************** END ****************************************/
#define MPU6050_ACCEL_CFG_REG			0X1C	//加速度计配置寄存器
    /********************* @MPU6050_ACCEL_RANGE *******************************/
    #define MPU6050_ACCEL_RANGE_2G      (0x00 << 3)
    #define MPU6050_ACCEL_RANGE_4G      (0x01 << 3)
    #define MPU6050_ACCEL_RANGE_8G      (0x02 << 3)
    #define MPU6050_ACCEL_RANGE_16G     (0x03 << 3)
    /*********************  END  ************************************************/
#define MPU6050_MOTION_DET_REG		0X1F	//运动检测阀值设置寄存器
#define MPU6050_MOTION_DUR_REG		0X20	//motion time value
#define MPU6050_FIFO_EN_REG				0X23	//FIFO使能寄存器
    /********************* @MPU6050_FIFO_EN *******************************/
    #define MPU6050_TEMP_FIFO_EN        (0x01 << 7)
    #define MPU6050_XG_FIFO_EN          (0x01 << 6)
    #define MPU6050_YG_FIFO_EN          (0x01 << 5)
    #define MPU6050_ZG_FIFO_EN          (0x01 << 4)
    #define MPU6050_ACCEL_FIFO_EN       (0x01 << 3)
    #define MPU6050_SLV2_FIFO_EN        (0x01 << 2)
    #define MPU6050_SLV1_FIFO_EN        (0x01 << 1)
    #define MPU6050_SLV0_FIFO_EN        (0x01 << 0)
    /*********************  END  *******************************************/
#define MPU6050_I2CMST_CTRL_REG		0X24	//IIC主机控制寄存器
#define MPU6050_I2CSLV0_ADDR_REG	0X25	//IIC从机0器件地址寄存器
#define MPU6050_I2CSLV0_REG				0X26	//IIC从机0数据地址寄存器
#define MPU6050_I2CSLV0_CTRL_REG	0X27	//IIC从机0控制寄存器
#define MPU6050_I2CSLV1_ADDR_REG	0X28	//IIC从机1器件地址寄存器
#define MPU6050_I2CSLV1_REG				0X29	//IIC从机1数据地址寄存器
#define MPU6050_I2CSLV1_CTRL_REG	0X2A	//IIC从机1控制寄存器
#define MPU6050_I2CSLV2_ADDR_REG	0X2B	//IIC从机2器件地址寄存器
#define MPU6050_I2CSLV2_REG				0X2C	//IIC从机2数据地址寄存器
#define MPU6050_I2CSLV2_CTRL_REG	0X2D	//IIC从机2控制寄存器
#define MPU6050_I2CSLV3_ADDR_REG	0X2E	//IIC从机3器件地址寄存器
#define MPU6050_I2CSLV3_REG				0X2F	//IIC从机3数据地址寄存器
#define MPU6050_I2CSLV3_CTRL_REG	0X30	//IIC从机3控制寄存器
#define MPU6050_I2CSLV4_ADDR_REG	0X31	//IIC从机4器件地址寄存器
#define MPU6050_I2CSLV4_REG				0X32	//IIC从机4数据地址寄存器
#define MPU6050_I2CSLV4_DO_REG		0X33	//IIC从机4写数据寄存器
#define MPU6050_I2CSLV4_CTRL_REG	0X34	//IIC从机4控制寄存器
#define MPU6050_I2CSLV4_DI_REG		0X35	//IIC从机4读数据寄存器

#define MPU6050_I2CMST_STA_REG		0X36	//IIC主机状态寄存器
#define MPU6050_INTBP_CFG_REG			0X37	//中断/旁路设置寄存器
#define MPU6050_INT_EN_REG				0X38	//中断使能寄存器
#define MPU6050_INT_STA_REG				0X3A	//中断状态寄存器

#define MPU6050_ACCEL_XOUTH_REG		0X3B	//加速度值,X轴高8位寄存器
#define MPU6050_ACCEL_XOUTL_REG		0X3C	//加速度值,X轴低8位寄存器
#define MPU6050_ACCEL_YOUTH_REG		0X3D	//加速度值,Y轴高8位寄存器
#define MPU6050_ACCEL_YOUTL_REG		0X3E	//加速度值,Y轴低8位寄存器
#define MPU6050_ACCEL_ZOUTH_REG		0X3F	//加速度值,Z轴高8位寄存器
#define MPU6050_ACCEL_ZOUTL_REG		0X40	//加速度值,Z轴低8位寄存器

#define MPU6050_TEMP_OUTH_REG			0X41	//温度值高八位寄存器
#define MPU6050_TEMP_OUTL_REG			0X42	//温度值低8位寄存器

#define MPU6050_GYRO_XOUTH_REG		0X43	//陀螺仪值,X轴高8位寄存器
#define MPU6050_GYRO_XOUTL_REG		0X44	//陀螺仪值,X轴低8位寄存器
#define MPU6050_GYRO_YOUTH_REG		0X45	//陀螺仪值,Y轴高8位寄存器
#define MPU6050_GYRO_YOUTL_REG		0X46	//陀螺仪值,Y轴低8位寄存器
#define MPU6050_GYRO_ZOUTH_REG		0X47	//陀螺仪值,Z轴高8位寄存器
#define MPU6050_GYRO_ZOUTL_REG		0X48	//陀螺仪值,Z轴低8位寄存器

#define MPU6050_MOT_DET_STA_REG 	0x61	//motion detect status

#define MPU6050_I2CSLV0_DO_REG		0X63	//IIC从机0数据寄存器
#define MPU6050_I2CSLV1_DO_REG		0X64	//IIC从机1数据寄存器
#define MPU6050_I2CSLV2_DO_REG		0X65	//IIC从机2数据寄存器
#define MPU6050_I2CSLV3_DO_REG		0X66	//IIC从机3数据寄存器

#define MPU6050_I2CMST_DELAY_REG	0X67	//IIC主机延时管理寄存器
#define MPU6050_SIGPATH_RST_REG		0X68	//信号通道复位寄存器
#define MPU6050_MDETECT_CTRL_REG	0X69	//运动检测控制寄存器
#define MPU6050_USER_CTRL_REG			0X6A	//用户控制寄存器
    /********************* @MPU6050_USER_CTRL *******************************/
    #define MPU6050_FIFO_EN_BIT		(1<<6)		//FIFO使能位
    #define MPU6050_I2CMST_EN_BIT	(1<<5)		//IIC主机使能位
    #define MPU6050_FIFO_RESET_BIT	(1<<2)		//FIFO缓存区重置位
    #define MPU6050_I2CMST_RESET_BIT (1<<1)		//IIC主机缓存区重置位
    #define MPU6050_SIG_RESET_BIT	(1<<0)		//信号通道重置位
    /*********************         END        *******************************/
#define MPU6050_PWR_MGMT1_REG			0X6B	//电源管理寄存器1
    /********************* @MPU6050_PWR_MGMT1 *******************************/
    #define MPU6050_DEVICE_RESET    (1<<7)		//设备复位位
    #define MPU6050_SLEEP_BIT       (1<<6)		//睡眠模式位
    #define MPU6050_CYCLE_BIT       (1<<5)		//循环模式位
    #define MPU6050_TEMP_DIS_BIT    (1<<3)		//温度传感器禁用位
    #define MPU6050_CLK_INTERNAL_8MHZ  0	//内部8M时钟
    #define MPU6050_CLK_PLL_XGYRO  1	//PLL_X轴陀螺仪时钟
    #define MPU6050_CLK_PLL_YGYRO  2	//PLL_Y轴陀螺仪时钟
    #define MPU6050_CLK_PLL_ZGYRO  3	//PLL_Z轴陀螺仪时钟
    #define MPU6050_CLK_PLL_EXT32K 4	//PLL外部32K时钟
    #define MPU6050_CLK_PLL_EXT19M 5	//PLL外部19M时钟
    #define MPU6050_CLK_STOP       7	//停止时钟
    /*********************         END        *******************************/
#define MPU6050_PWR_MGMT2_REG			0X6C	//电源管理寄存器2 
    /********************* @MPU6050_PWR_MGMT2 *******************************/
    #define MPU6050_WEKA_FREQ_1HZ    (0<<6)		//唤醒频率为1.25Hz
    #define MPU6050_WEKA_FREQ_5HZ    (1<<6)		//唤醒频率为5Hz
    #define MPU6050_WEKA_FREQ_20HZ   (2<<6)		//唤醒频率为20Hz
    #define MPU6050_WEKA_FREQ_40HZ   (3<<6)		//唤醒频率为40Hz
    #define MPU6050_STBY_XA          (1<<5)		//X轴加速度计待机模式
    #define MPU6050_STBY_YA          (1<<4)		//Y轴加速度计待机模式
    #define MPU6050_STBY_ZA          (1<<3)		//Z轴加速度计待机模式
    #define MPU6050_STBY_XG          (1<<2)		//X轴陀螺仪待机模式
    #define MPU6050_STBY_YG          (1<<1)		//Y轴陀螺仪待机模式
    #define MPU6050_STBY_ZG          (1<<0)		//Z轴陀螺仪待机模式
    /*********************         END        *******************************/
#define MPU6050_FIFO_CNTH_REG			0X72	//FIFO计数寄存器高八位
#define MPU6050_FIFO_CNTL_REG			0X73	//FIFO计数寄存器低八位
#define MPU6050_FIFO_RW_REG				0X74	//FIFO读写寄存器
#define MPU6050_DEVICE_ID_REG			0X75	//器件ID寄存器
 
//如果AD0脚(9脚)接地,IIC地址为0X68(不包含最低位).
//如果接V3.3,则IIC地址为0X69(不包含最低位).
#define MPU6050_ADDR				0X68
#define MPU6050_ID					0x68



#endif /* __MPU6050_H__ */
