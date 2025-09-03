#ifndef __MPWM_H
#define __MPWM_H

#include "module_config.h"
#include <stdint.h>

#if USE_MPWM == 1

typedef struct
{
    uint32_t tim_clock;                 //定时器时钟频率(分频后的)
    uint32_t channel_freq;              //PWM频率
    uint32_t channel_duty;              //PWM占空比
    void (*set_autoreload)(uint32_t);   //设置自动重装载值
    void (*set_compare)(uint32_t);      //设置比较值
    void (*pwm_start)(void);            //启动pwm
    void (*pwm_stop)(void);             //停止pwm
}mpwm_ctrl_t;


void mpwm_start(mpwm_ctrl_t* mpwm_t);
void mpwm_stop(mpwm_ctrl_t* mpwm_t);

#endif

#endif /* __MPWM_H */
