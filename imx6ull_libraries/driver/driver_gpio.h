#ifndef _DRIVER_GPIO_H_
#define _DRIVER_GPIO_H_

#include "common_typedef.h"
#include "MCIMX6Y2.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"

typedef enum // 枚举端口 
{
    P01_0 = 1 * 32, P01_1, P01_2, P01_3, P01_4, P01_5, P01_6, P01_7,
    P01_8, P01_9, P01_10, P01_11, P01_12, P01_13, P01_14, P01_15,
    P01_16, P01_17, P01_18, P01_19, P01_20, P01_21, P01_22, P01_23, 
    P01_24, P01_25, P01_26, P01_27, P01_28, P01_29, P01_30, P01_31,

    P02_0 = 2 * 32, P02_1, P02_2, P02_3, P02_4, P02_5, P02_6, P02_7,
    P02_8, P02_9, P02_10, P02_11, P02_12, P02_13, P02_14, P02_15,
    P02_16, P02_17, P02_18, P02_19, P02_20, P02_21,

    P03_0 = 3 * 32, P03_1, P03_2, P03_3, P03_4, P03_5, P03_6, P03_7,
    P03_8, P03_9, P03_10, P03_11, P03_12, P03_13, P03_14, P03_15,
    P03_16, P03_17, P03_18, P03_19, P03_20, P03_21, P03_22, P03_23,
    P03_24, P03_25, P03_26, P03_27, P03_28,

    P04_0 = 4 * 32, P04_1, P04_2, P04_3, P04_4, P04_5, P04_6, P04_7,
    P04_8, P04_9, P04_10, P04_11, P04_12, P04_13, P04_14, P04_15,
    P04_16, P04_17, P04_18, P04_19, P04_20, P04_21, P04_22, P04_23,
    P04_24, P04_25, P04_26, P04_27, P04_28,

    P05_0 = 5 * 32, P05_1, P05_2, P05_3, P05_4, P05_5, P05_6, P05_7,
    P05_8, P05_9, P05_10, P05_11, 
}gpio_pin_enum;

typedef enum               // 枚举端口方向
{
    GPI = 0U,               // 定义管脚输入方向
    GPO = 1U,               // 定义管脚输出方向
}gpio_dir_enum;

typedef enum               // 枚举端口电平
{
    GPIO_LOW = 0,          // 定义低电平
    GPIO_HIGH = 1,         // 定义高电平
}gpio_level_enum;

typedef struct {
    uint32 muxRegister;
    uint32 muxMode;
    uint32 inputRegister;
    uint32 inputDaisy;
    uint32 configRegister;
} iomuxc_value_t;
//====================================================GPIO 快捷函数====================================================
GPIO_Type* get_port(gpio_pin_enum pin);
//------------------------------------------------------------------------------------------------------------------
// 函数简介     对应 IO 置位为高电平
// 参数说明     pin           选择的引脚 (可选择范围由 driver_gpio.h 内 gpio_pin_enum 枚举值确定)
// 返回参数     void
// 使用示例     gpio_high(P01_0);// P00_0 输出高电平
// 备注信息
//------------------------------------------------------------------------------------------------------------------
#define     gpio_high(pin)                    get_port(pin)->DR |=  ( 1U << ( ( 0 | (pin & 0x1f) ) ) )   // GPIO置位

//------------------------------------------------------------------------------------------------------------------
// 函数简介     对应 IO 置位为低电平
// 参数说明     pin           选择的引脚 (可选择范围由 driver_gpio.h 内 gpio_pin_enum 枚举值确定)
// 返回参数     void
// 使用示例     gpio_low(P01_0);// P00_0 输出低电平
// 备注信息
//------------------------------------------------------------------------------------------------------------------
#define     gpio_low(pin)                     get_port(pin)->DR &= ~( 1U << ( ( 0 | (pin & 0x1f) ) ) )   // GPIO复位

//====================================================GPIO 快捷函数====================================================

//====================================================GPIO 基础函数====================================================
void gpio_set_level(gpio_pin_enum pin, uint8 dat);
uint8 gpio_get_level(gpio_pin_enum pin);
void gpio_toggle_level(gpio_pin_enum pin);
void gpio_init(gpio_pin_enum pin, gpio_dir_enum dir, uint8 dat);
#if 0
void iomuxc_set(uint32 muxRegister, uint32 muxMode, uint32 inputRegister,uint32 inputDaisy, uint32 configRegister, uint32 inputOnfield, uint32 configValue);
#endif
void iomuxc_set(iomuxc_value_t iomux, uint32 inputOnfield, uint32 configValue);
//====================================================GPIO 基础函数====================================================

#endif // !_DRIVER_GPIO_H_
