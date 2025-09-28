/*
 * @Author: RoadToSea 3376714571@qq.com
 * @Date: 2025-09-02 15:13:26
 * @LastEditors: RoadToSea 3376714571@qq.com
 * @LastEditTime: 2025-09-10 16:50:24
 * @FilePath: \watch\Lwatch\bsp\stm32f411ceu6\driver\stm32f411ceu6_mpu6050.c
 * @Description: stm32f411ceu6 mpu6050底层驱动
 * 版权声明 保留文件所有权利 
 * Copyright: (c) 2025 by RoadToSea , All Rights Reserved. 
 */

 #include "drv_mpu6050.h"
 #include "sw_iic.h"
 #include "stm32f4xx_hal.h"
 #include "mpu6050_reg.h"
 #include "delay.h"
 #include "inv_mpu.h"
 #include "inv_mpu_dmp_motion_driver.h"
 #include "port_mpu.h"
 #include "math.h"

/**************************************************************** */
/*                      defines                                   */
/**************************************************************** */
#define DEFAULT_MPU_HZ   100      //默认采样频率100hz
#define q30  1073741824.0f //用于归一化四元数

/**************************************************************** */
/*                      typedef                                   */
/**************************************************************** */
typedef struct 
{
    uint8_t accel_range;
    uint8_t gyro_range;
}board_mpu6050_cfg_t;


/**************************************************************** */
/*                      variable                                  */
/**************************************************************** */
 board_iic_t mpu6050_iic = 
{
    {
        .port = GPIOB,.pin = GPIO_PIN_13,.mode = GPIO_MODE_OUTPUT_OD
    },
    {
        .port = GPIOB,.pin = GPIO_PIN_14,.mode = GPIO_MODE_OUTPUT_PP
    }
};

board_mpu6050_cfg_t mpu6050_cfg[] = {
    {
        .accel_range = MPU6050_ACCEL_RANGE_8G,
        .gyro_range  = MPU6050_GYRO_RANGE_2000DPS
    }
};

static signed char gyro_orientation[9] = { 1, 0, 0,  //方向矩阵
                                           0, 1, 0,
                                           0, 0, 1};


/**************************************************************** */
/*                      function declaration                      */
/**************************************************************** */
                                        
int drv_mpu6050_board_start_work(int id);


int drv_mpu6050_board_init(int id)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    HAL_GPIO_WritePin(mpu6050_iic.sda.port, mpu6050_iic.sda.pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(mpu6050_iic.scl.port, mpu6050_iic.scl.pin, GPIO_PIN_SET);

    //sensor iic sda 配置
    GPIO_InitStruct.Pin = mpu6050_iic.sda.pin;
    GPIO_InitStruct.Mode = mpu6050_iic.sda.mode;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(mpu6050_iic.sda.port, &GPIO_InitStruct);

    //sensor iic scl 配置
    GPIO_InitStruct.Pin = mpu6050_iic.scl.pin;
    GPIO_InitStruct.Mode = mpu6050_iic.scl.mode;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(mpu6050_iic.scl.port, &GPIO_InitStruct);
		
    
    drv_mpu6050_board_start_work(id);
    return DRV_OK;
}



/**
 * @description: 获取mpu6050芯片id
 * @param {int} id
 * @param {uint8_t*} chip_id
 * @return {*}
 */
int drv_mpu6050_board_get_chip_id(int id,uint8_t* chip_id)
{
    return sw_iic_read_one_byte(&mpu6050_iic,MPU6050_ADDR,MPU6050_DEVICE_ID_REG,chip_id);
}

/**
 * @description: 获取mpu6050加速度计数据
 * @param {int} id
 * @param {drv_mpu6050_accel_t*} data
 * @return {*}
 */
int drv_mpu6050_board_get_accel(int id,drv_mpu6050_accel_t* data)
{
    int ret = DRV_ERR_NOSUPPORT;
    float factor = 16384;  //参数因子,默认2G
    uint8_t tmp[6]={0};
    ret = sw_iic_read_multi_byte(&mpu6050_iic,MPU6050_ADDR,MPU6050_ACCEL_XOUTH_REG,tmp,6);

    switch (mpu6050_cfg->accel_range)
    {
    case MPU6050_ACCEL_RANGE_2G: factor = 16384; break;
    case MPU6050_ACCEL_RANGE_4G: factor = 8192; break;
    case MPU6050_ACCEL_RANGE_8G: factor = 4096; break;
    case MPU6050_ACCEL_RANGE_16G: factor = 2048; break;
    default: factor = 16384; break;
    }

    // 转成 int16_t，保证补码符号正确
    data->ax = (int16_t)((tmp[0] << 8) | tmp[1])/ factor;
    data->ay = (int16_t)((tmp[2] << 8) | tmp[3])/ factor;
    data->az = (int16_t)((tmp[4] << 8) | tmp[5])/ factor;

    return ret;
}

/**
 * @description: 获取mpu6050陀螺仪数据
 * @param {int} id
 * @param {drv_mpu6050_gyro_t*} data
 * @return {*}
 */
int drv_mpu6050_board_get_gyro(int id,drv_mpu6050_gyro_t* data)
{
    int ret = DRV_ERR_NOSUPPORT;
    float factor = 131;   //参数因子
    uint8_t tmp[6]={0};
    ret = sw_iic_read_multi_byte(&mpu6050_iic,MPU6050_ADDR,MPU6050_GYRO_XOUTH_REG,tmp,6);

    switch (mpu6050_cfg->gyro_range)
    {
    case MPU6050_GYRO_RANGE_250DPS: factor = 131; break;
    case MPU6050_GYRO_RANGE_500DPS: factor =  65.5; break;
    case MPU6050_GYRO_RANGE_1000DPS: factor = 32.8; break;
    case MPU6050_GYRO_RANGE_2000DPS: factor = 16.4; break;
    default: factor = 131; break;
    }

    data->gx = (int16_t)(tmp[0]<<8|tmp[1])/factor;
    data->gy = (int16_t)(tmp[2]<<8|tmp[3])/factor;
    data->gz = (int16_t)(tmp[4]<<8|tmp[5])/factor;

    return ret;
}

/**
 * @description: 获取mpu6050 俯仰角、横滚角、航向角
 * @param {int} id
 * @param {drv_mpu6050_angle_t*} angle
 * @return {*}
 */
int drv_mpu6050_board_get_angle(int id,drv_mpu6050_angle_t* angle)
{
    static short gyro[3], accel[3],sensors;
    unsigned long sensor_timestamp;
    unsigned char more;
    long quat[4];
    
    // 从DMP FIFO读取四元数数据
    if (dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors, &more))
        return DRV_ERR_GENERIC;

    // DEBUG("accel[0]:%d,accel[1]:%d,accel[2]:%d\n",accel[0],accel[1],accel[2]);
    // DEBUG("gyro[0]:%d,gyro[1]:%d,gyro[2]:%d\n",gyro[0],gyro[1],gyro[2]);
    // DEBUG("quat[0]:%d,quat[1]:%d,quat[2]:%d,quat[3]:%d\n",quat[0],quat[1],quat[2],quat[3]);

    // 四元数数据解析（q30格式转浮点）
    if (sensors & INV_WXYZ_QUAT) 
    {
        float q0 = quat[0] / q30;
        float q1 = quat[1] / q30;
        float q2 = quat[2] / q30;
        float q3 = quat[3] / q30;
        // DEBUG("q0:%f,q1:%f,q2:%f,q3:%f\n",q0,q1,q2,q3);

        // 欧拉角计算（单位：度）
        angle->pitch = asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.3;  // 俯仰角
        angle->roll  = atan2(2 * q2 * q3 + 2 * q0 * q1, 
                      -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.3;  // 横滚角
        angle->yaw   = atan2(2 * (q1 * q2 + q0 * q3),
                       q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * 57.3;  // 偏航角
    }

    return DRV_OK;
}

/**
 * @description: 设置mpu6050加速度计范围
 * @param {int} id
 * @param {uint8_t} find param in @MPU6050_ACCEL_RANGE
 * @return {*}
 */
int drv_mpu6050_set_accel_range(int id, uint8_t range)
{
    return sw_iic_write_one_byte(&mpu6050_iic,MPU6050_ADDR,MPU6050_ACCEL_CFG_REG,MPU6050_ACCEL_RANGE_8G);
}

/**
 * @description: 设置mpu6050陀螺仪范围
 * @param {int} id
 * @param {uint8_t} find param in @MPU6050_GYRO_RANGE
 * @return {*}
 */
int drv_mpu6050_set_gyro_range(int id, uint8_t range)
{
    return sw_iic_write_one_byte(&mpu6050_iic,MPU6050_ADDR,MPU6050_GYRO_CFG_REG,MPU6050_GYRO_RANGE_2000DPS);
}

/**
 * @description: 设置mpu6050低通滤波器
 * @param {int} id
 * @param {uint8_t} filter(5~500)
 * @return {*}
 */
int drv_mpu6050_board_set_low_pass_filter(int id, uint8_t filter)
{
    uint8_t reg_data = 0;
    if(filter>=260)  reg_data = 0;
    else if(filter>=184)  reg_data = 1;
    else if(filter>=94)  reg_data = 2;
    else if(filter>=44)  reg_data = 3;
    else if(filter>=21)  reg_data = 4;
    else if(filter>=10)  reg_data = 5;
    else if(filter>=5)   reg_data = 6;
    else                return DRV_ERR_INVAL;

    return sw_iic_write_one_byte(&mpu6050_iic,MPU6050_ADDR,MPU6050_CFG_REG,reg_data);
}

/**
 * @description: 设置mpu6050采样率
 * @param {int} id
 * @param {uint16_t} rate(4-1000hz)
 * @return {*}
 */
int drv_mpu6050_board_set_sample_rate(int id, uint16_t rate)
{
    int ret = DRV_ERR_NOSUPPORT;
    if(rate > 1000)  rate = 1000;
    else if(rate <4) rate = 4;

    uint16_t sample = 1000/rate-1;

    ret = drv_mpu6050_board_set_low_pass_filter(id,rate/2);
    FUNC_CHECK_RET(ret);
    
    ret = sw_iic_write_one_byte(&mpu6050_iic,MPU6050_ADDR,MPU6050_SAMPLE_RATE_REG,sample);
    return ret;
}

/**
 * @description: 设置mpu6050中断使能
 * @param {int} id
 * @param {uint8_t} enable
 * @return {*}
 */
int drv_mpu6050_board_set_interrupt(int id, uint8_t enable)
{
    return sw_iic_write_one_byte(&mpu6050_iic,MPU6050_ADDR,MPU6050_INT_EN_REG,enable);
}

/**
 * @description: 设置mpu6050用户控制寄存器
 * @param {int} id
 * @param {uint8_t} find param in @MPU6050_USER_CTRL
 * @return {*}
 */
int drv_mpu6050_board_set_user_ctrl(int id,uint8_t select)
{
    return sw_iic_write_one_byte(&mpu6050_iic,MPU6050_ADDR,MPU6050_USER_CTRL_REG,select);
}

/**
 * @description: 设置mpu6050 fifo寄存器
 * @param {int} id
 * @param {uint8_t} find param in @MPU6050_FIFO_EN
 * @return {*}
 */
int drv_mpu6050_board_set_fifo(int id, uint8_t select)
{
    return sw_iic_write_one_byte(&mpu6050_iic,MPU6050_ADDR,MPU6050_FIFO_EN_REG,select);
}

/**
 * @description: mpu 运行自测
 * @return {*}
 */
int run_self_test(void)
{
    int result;
    long gyro[3], accel[3];

    result = mpu_run_self_test(gyro, accel);
    if (result == 0x7) {
        /* Test passed. We can trust the gyro data here, so let's push it down
         * to the DMP.
         */
        float sens;
        unsigned short accel_sens;
        mpu_get_gyro_sens(&sens);
        gyro[0] = (long)(gyro[0] * sens);
        gyro[1] = (long)(gyro[1] * sens);
        gyro[2] = (long)(gyro[2] * sens);
        dmp_set_gyro_bias(gyro);
        mpu_get_accel_sens(&accel_sens);
        accel[0] *= accel_sens;
        accel[1] *= accel_sens;
        accel[2] *= accel_sens;
        dmp_set_accel_bias(accel);
        DEBUG("setting bias succesfully ......\r\n");
    }

    return DRV_OK;
}

int drv_mpu6050_board_start_work(int id)
{
    int ret = DRV_ERR_NOSUPPORT;
    ret = sw_iic_write_one_byte(&mpu6050_iic,MPU6050_ADDR,MPU6050_PWR_MGMT1_REG,0X80);	//复位MPU6050
    FUNC_CHECK_RET(ret);
    mdelay_ms(100);
	ret = sw_iic_write_one_byte(&mpu6050_iic,MPU6050_ADDR,MPU6050_PWR_MGMT1_REG,0X00);	//唤醒MPU6050
    FUNC_CHECK_RET(ret);

    ret = drv_mpu6050_set_accel_range(id,MPU6050_ACCEL_RANGE_8G);
    FUNC_CHECK_RET(ret);
    ret = drv_mpu6050_set_gyro_range(id,MPU6050_GYRO_RANGE_2000DPS);
    FUNC_CHECK_RET(ret);
    ret = drv_mpu6050_board_set_sample_rate(id,50);
    FUNC_CHECK_RET(ret);

    ret = drv_mpu6050_board_set_interrupt(id,0);		//关闭所有中断
    FUNC_CHECK_RET(ret);
	ret = drv_mpu6050_board_set_user_ctrl(id,0);	//IIC主模式关闭
    FUNC_CHECK_RET(ret);
	ret = drv_mpu6050_board_set_fifo(id,0);		//dis FIFO
    FUNC_CHECK_RET(ret);
	ret = sw_iic_write_one_byte(&mpu6050_iic,MPU6050_ADDR,MPU6050_INTBP_CFG_REG,0X80);	//INT active low
    FUNC_CHECK_RET(ret);

    ret = sw_iic_write_one_byte(&mpu6050_iic,MPU6050_ADDR,MPU6050_PWR_MGMT1_REG,MPU6050_CYCLE_BIT|MPU6050_CLK_INTERNAL_8MHZ|MPU6050_TEMP_DIS_BIT);	
    FUNC_CHECK_RET(ret);
    ret = sw_iic_write_one_byte(&mpu6050_iic,MPU6050_ADDR,MPU6050_PWR_MGMT2_REG,MPU6050_WEKA_FREQ_20HZ);
    FUNC_CHECK_RET(ret);

    ret = sw_iic_write_one_byte(&mpu6050_iic,MPU6050_ADDR,MPU6050_INTBP_CFG_REG,0X90);     //INT Pin active low level, reset until 50us
    FUNC_CHECK_RET(ret);
    ret = sw_iic_write_one_byte(&mpu6050_iic,MPU6050_ADDR,MPU6050_INT_EN_REG,0x40);       	//enable INT

    dmp_init();
    return ret;
}

/**
 * @description: dmp库iic写操作接口函数
 * @param {uint8_t} addr
 * @param {uint8_t} reg
 * @param {uint8_t} data_len
 * @param {uint8_t*} pdata
 * @return {*}
 */
int port_iic_write(uint8_t addr,uint8_t reg,uint8_t data_len,uint8_t* pdata )
{
    if(sw_iic_write_multi_byte(&mpu6050_iic,addr,reg,pdata,data_len) == DRV_OK)
    {
        return 0;
    }
    else 
    {
        return 1;
    }
}

/**
 * @description: dmp库iic读操作接口函数
 * @param {uint8_t} addr
 * @param {uint8_t} reg
 * @param {uint8_t} data_len
 * @param {uint8_t*} pdata
 * @return {*}
 */
int port_iic_read(uint8_t addr,uint8_t reg,uint8_t data_len,uint8_t* pdata )
{
    if(sw_iic_read_multi_byte(&mpu6050_iic,addr,reg,pdata,data_len) == DRV_OK)
    {
        return 0;
    }
    else 
    {
        return 1;
    }
}

static  unsigned short inv_row_2_scale(const signed char *row) //用于初始化DMP
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    return b;

}


static  unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx) //用于初始化DMP
{
    unsigned short scalar;
    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;

    return scalar;

}

/**
 * @description: 初始化dmp库
 * @return {*}
 */
void dmp_init(void)
{
    uint8_t temp[1]={0};
    drv_mpu6050_board_get_chip_id(0,&temp[0]);          // 读取MPU6050设备ID
    if(temp[0]!=0x68) NVIC_SystemReset(); // 校验设备ID是否为0x68（正常值），错误则系统复位


    // 配置传感器使能状态
    if(!mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL)) {} // 启用陀螺仪和加速度计的三轴数据
    
    // FIFO配置
    if(!mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL)) {} // 配置FIFO存储陀螺仪和加速度计数据
    
    // 采样率设置
    if(!mpu_set_sample_rate(DEFAULT_MPU_HZ)) {} // 设置采样率为默认值（典型值如100Hz）
    
    // DMP固件加载
    if(!dmp_load_motion_driver_firmware()) {} // 加载DMP运动驱动固件
    
    // 方向矩阵配置
    if(!dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation))) {} // 设置传感器方向校准矩阵
    
    // 启用DMP特性
    if(!dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |  // 启用6轴低功耗四元数 | 敲击检测
            DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL |   // Android方向识别 | 原始加速度数据
            DMP_FEATURE_SEND_CAL_GYRO | DMP_FEATURE_GYRO_CAL)) {}       // 校准后的陀螺仪数据 | 陀螺仪校准
    
    // FIFO速率设置
    if(!dmp_set_fifo_rate(DEFAULT_MPU_HZ)) {} // 设置DMP输出速率与采样率同步
    
    run_self_test(); // 运行自检程序（校准传感器）
    
    // 启用DMP
    if(!mpu_set_dmp_state(1)) {} // 激活DMP功能（1=启用，0=关闭）
    
}

