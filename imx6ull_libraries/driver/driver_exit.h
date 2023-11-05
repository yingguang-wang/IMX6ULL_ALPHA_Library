#ifndef _DRIVER_EXIT_H_
#define _DRIVER_EXIT_H_

#include "driver_gpio.h"



typedef struct
{
	gpio_pin_enum exti_pin_enum;
	iomuxc_value_t iomuxc;
}exti_pin_value;
// 中断标志位获取
uint32 exti_flag_get(gpio_pin_enum pin);	
// 中断标志位清除
void exti_flag_clear(gpio_pin_enum pin);      

//====================================================EXIT 基础函数====================================================
//void exti_all_close         (void);                                                 // EXTI 失能
//void exti_enable            (gpio_pin_enum eru_pin);                                // EXTI 中断使能
//void exti_disable           (gpio_pin_enum eru_pin);                                // EXTI 中断失能
#if 0
//void exti_init(gpio_pin_enum pin, uint32 muxRegister, uint32 muxMode, uint32 inputRegister,uint32 inputDaisy, uint32 configRegister, gpio_interrupt_mode_t trigger);     // EXTI 中断初始化
#endif
//void exti_init(gpio_pin_enum pin, iomuxc_value_t iomux, gpio_interrupt_mode_t trigger);
void exti_init(gpio_pin_enum pin, gpio_interrupt_mode_t trigger);

//====================================================EXIT 基础函数====================================================



#endif // !_DRIVER_EXIT_H_
