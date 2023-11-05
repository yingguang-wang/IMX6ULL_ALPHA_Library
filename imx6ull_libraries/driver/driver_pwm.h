#ifndef __DRIVER_PWM__H__
#define __DRIVER_PWM__H__

#include "common_typedef.h"
#include "MCIMX6Y2.h"
#include "driver_gpio.h"

#define PWM_DUTY_MAX     10000                 // PWM最大占空比  最大占空比越大占空比的步进值越小

typedef struct
{
    uint32 pwm_duty[9];
}pwm_duty_value_t;

extern pwm_duty_value_t pwm_duty_value;
//====================================================PWM 基础函数====================================================
void pwm_init(PWM_Type *base, iomuxc_value_t iomuxc, uint32 freq, uint32 duty);
void pwm_set_duty(PWM_Type *base, uint32 duty);
void pwm_enable(PWM_Type *base);
void pwm_disable(PWM_Type *base);
//====================================================PWM 基础函数====================================================

void Pwm_Install_IrqHandler(PWM_Type *base);
uint8 save_pwm_duty(PWM_Type *base, uint32 duty);
inline void PWM_SetSampleValue(PWM_Type *base, uint32_t value);
inline void PWM_SetPeriodValue(PWM_Type *base, uint32_t value);

/*******************************************************PWM中断服务函数*******************************************************/
void PWM1_Irq_Handler(void);
void PWM2_Irq_Handler(void);
void PWM3_Irq_Handler(void);
void PWM4_Irq_Handler(void);
void PWM5_Irq_Handler(void);
void PWM6_Irq_Handler(void);
void PWM7_Irq_Handler(void);
void PWM8_Irq_Handler(void);
/*******************************************************PWM中断服务函数*******************************************************/

#endif  //!__DRIVER_PWM__H__