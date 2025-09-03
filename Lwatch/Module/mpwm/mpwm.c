#include "mpwm.h"
#include <stdint.h>

#if USE_MPWM == 1

/**
 * @description: 按照设置的参数启动pwm
 * @param {mpwm_ctrl_t*} pwm参数结构体
 * @return {*}
 */
void mpwm_start(mpwm_ctrl_t* mpwm_t)
{
    //使用float提高精度
    uint32_t arr_val = (uint32_t)( (float)mpwm_t->tim_clock / mpwm_t->channel_freq - 1 );
    //加50是为了四舍五入
    uint32_t ccr_val = (uint32_t)( (mpwm_t->channel_duty * (arr_val + 1) + 50) / 100 );
    mpwm_t->set_autoreload(arr_val);
    mpwm_t->set_compare(ccr_val);
    
    mpwm_t->pwm_start();
}
    
/**
 * @description: 关闭pwm
 * @param {mpwm_ctrl_t*} mpwm_t
 * @return {*}
 */
void mpwm_stop(mpwm_ctrl_t* mpwm_t)
{
    mpwm_t->pwm_stop();
}

#endif
