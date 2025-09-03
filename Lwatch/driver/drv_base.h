#ifndef __drv_base_H
#define __drv_base_H


#include <stdint.h>

#define DRIVER_VERSION         "V1.0.0"
#define DRIVER_ARCHITECTURE  
#define DRIVER_SECTION         driver_init_section

#define ARM     0
#define RISCV   1

typedef void (*driver_init_fn_t)(void);

/************************************************************************** 
                                 工具宏  
************************************************************************** */
#define NULL 0
//连接两个标识符
#define CONTACT(a,b)            a##b
//转换成字符串
#define STRINGIFY(a)            #a
//虚函数定义
#define WEAK_FUNC               __attribute__((weak))


/************************************************************************** 
                                 段注册  
************************************************************************** */

/**
 * @description: 将变量注册到段中
 * @param        {section_name} 段名称
 * @param        {var}          变量
 */
#define REGISTER_VAR_TO_SECTION(section_name,var)      var  __attribute__((section(STRINGIFY(section_name)))) __attribute__((used))


/**
 * @description: 将变量转换成常量并注册到段中,存放在FLASH
 * @param        {section_name} 段名称
 * @param        {var}          变量
 */
#define REGISTER_VAR_TO_SECTION_FLASH(section_name,var)      REGISTER_VAR_TO_SECTION(section_name,const var)


/**
 * @description: 将变量注册到段中,存放在RAM
 * @param        {section_name} 段名称
 * @param        {var}          变量
 */
#define REGISTER_VAR_TO_SECTION_RAM(section_name,var)      REGISTER_VAR_TO_SECTION(section_name,var)


/**
 * @description: 将函数注册到指定段driver_init_section中,注册名为__driver_init_函数名,值为函数指针
 * @param        {func}          函数指针
 */                                                         
#define DRIVER_REG(func) \
    REGISTER_VAR_TO_SECTION_FLASH(DRIVER_SECTION, driver_init_fn_t CONTACT(__driver_,func)) = func

/*** 
 * @description: 根据不同的编译器选择对应的宏
 * @return {*}
 */
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)   // Keil MDK ARMCC/ArmClang
    /**
     * @description: 提供段的开始和结尾地址
     * @param        {section_name} 段名称
     */
    #define SECTION_INIT(section_name) \
        extern unsigned int CONTACT(section_name, $$Base); \
        extern unsigned int CONTACT(section_name, $$Limit);

    /**
     * @description: 提供段的开始和结尾地址
     */
    #define SECTION_START(section_name) ((void*)&CONTACT(section_name, $$Base))
    #define SECTION_END(section_name)   ((void*)&CONTACT(section_name, $$Limit))

#elif defined(__GNUC__)   // GCC / RISC-V-GCC
    /**
     * @description: 提供段的开始和结尾地址
     * @param        {section_name} 段名称
     */
    #define SECTION_INIT(section_name) \
        extern char CONTACT(__start_,section_name); \
        extern char CONTACT(__stop_,section_name);

    /**
     * @description: 提供段的开始和结尾地址
     */
    #define SECTION_START(section_name) ((void*)&CONTACT(__start_,section_name))
    #define SECTION_END(section_name)   ((void*)&CONTACT(__stop_,section_name))

#else
    #error "Unsupported compiler"
#endif

/**
 * @description: 遍历指定段中的所有变量
 */
#define DRIVER_FOREACH(section_name,type,iter)            for(type* iter=(type*)SECTION_START(section_name);\
                                                              iter<=(type*)SECTION_END(section_name);\
                                                              iter++)
                                                          

/************************************************************************** 
                                错误码  
************************************************************************** */
/* 成功 */
#define DRV_OK              (0)     // 操作成功

/* 通用错误 */
#define DRV_ERR_GENERIC     (-1)    // 未知错误
#define DRV_ERR_INVAL       (-2)    // 无效参数
#define DRV_ERR_NOMEM       (-3)    // 内存不足
#define DRV_ERR_NOSUPPORT   (-4)    // 不支持的功能
#define DRV_ERR_TIMEOUT     (-5)    // 操作超时
#define DRV_ERR_BUSY        (-6)    // 设备忙
#define DRV_ERR_NODEV       (-7)    // 设备不存在
#define DRV_ERR_NOINIT      (-8)    // 设备未初始化
#define DRV_ERR_IO          (-9)    // IO 错误
#define DRV_ERR_PERM        (-10)   // 权限不足
#define DRV_ERR_OVERFLOW    (-11)   // 缓冲区溢出
#define DRV_ERR_UNDERFLOW   (-12)   // 缓冲区下溢
#define DRV_ERR_CRC         (-13)   // 数据校验错误
#define DRV_ERR_DISCONNECT  (-14)   // 设备断开
#define DRV_ERR_STATE       (-15)   // 状态不正确，操作不允许


/************************************************************************** 
                                 驱动运行状态 
************************************************************************** */
#define DRV_STATUS_UNINIT    0x00    // 未初始化
#define DRV_STATUS_RUNNING   0x01    // 运行中
#define DRV_STATUS_PAUSE     0x02    // 暂停
#define DRV_STATUS_STOP      0x03    // 停止
#define DRV_STATUS_ERROR     0x04    // 错误



typedef struct 
{
    int cmd;            //命令
    void* user_data;    //用户数据
}drv_param_t;


//  前向声明结构体，并同时定义 typedef
typedef struct drv_if drv_if_t;


typedef struct {
    int (*init)(drv_if_t* );
    int (*open)(drv_if_t* ,int);
    int (*close)(drv_if_t* ,int);
    int (*ctrl)(drv_if_t* ,int cmd, void* user_data);
    int (*read)(drv_if_t*, unsigned int, void*, unsigned int);
    int (*write)(drv_if_t*, unsigned int, const void*, unsigned int);
} drv_ops_t;


struct drv_if {
    const char* drv_name;
    int id;
    unsigned int status;
    drv_ops_t* ops;
    void* priv_data;
};


int drv_init(drv_if_t* driver);
int drv_open(drv_if_t* driver,int cmd);
int drv_close(drv_if_t* driver,int cmd);
int drv_ctrl(drv_if_t* driver,int cmd, void* user_data);
int drv_write(drv_if_t* driver,unsigned int addr,const void* send,unsigned int len);
int drv_read(drv_if_t* driver,unsigned int addr,void* recv,unsigned int len);
void drv_base_init(void);
drv_if_t* drv_find(const char* name);
void drv_add(drv_if_t* driver);

#endif /* __drv_base_H */
