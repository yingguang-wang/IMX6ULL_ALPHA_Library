#include "driver_gpio.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"


//-------------------------------------------------------------------------------------------------------------------
//  函数简介      获取GPIO基地址
//  参数说明      pin         选择的引脚 (可选择范围由 zf_driver_gpio.h 内gpio_pin_enum枚举值确定)
//  返回参数      void
//  使用示例      get_port(P01_0)
//  备注信息      
//-------------------------------------------------------------------------------------------------------------------
GPIO_Type* get_port(gpio_pin_enum pin)
{
    GPIO_Type* gpio_type;
    switch (pin & 0xffe0)
    {
        case P01_0: gpio_type = GPIO1; break;
        case P02_0: gpio_type = GPIO2; break;
        case P03_0: gpio_type = GPIO3; break;
        case P04_0: gpio_type = GPIO4; break;
        case P05_0: gpio_type = GPIO5; break;
        default:gpio_type = NULL;break;
    }
    return gpio_type;
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     gpio 输出设置
// 参数说明     pin         选择的引脚 (可选择范围由 driver_gpio.h 内 gpio_pin_enum 枚举值确定)
// 参数说明     dat         0：低电平 1：高电平
// 返回参数     void
// 使用示例     gpio_set_level(P01_0, 1);// P01_0 输出高电平
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void gpio_set_level(gpio_pin_enum pin, uint8 dat)
{
    uint32 pin_value = 0;
    pin_value |= (pin & 0x1f);//目的为转换为pin口的0-31

    GPIO_Type* gpio_type;
    gpio_type = get_port(pin);

    if (dat == 0U)
    {
        gpio_type->DR &= ~(1U << pin_value);  /* Set pin output to low level.*/
    }
    else
    {
        gpio_type->DR |= (1U << pin_value);  /* Set pin output to high level.*/
    }
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     gpio电平获取
// 参数说明     base	   要读取的GPIO组，详见"fsl_iomuxc.h"文件
// 参数说明     pin        要读取的GPIO在组内的编号
// 返回参数     uint8      引脚当前电平
// 使用示例     uint8 status = gpio_get_level(P01_0);// 获取P01_0引脚电平
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8 gpio_get_level(gpio_pin_enum pin)
{   
    uint32 pin_value = 0 ;
    pin_value |= (pin & 0x1f);//目的为转换为pin口的0-31

    return (((get_port(pin)->DR) >> pin_value) & 0x1U);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     gpio翻转电平
// 参数说明     pin         选择的引脚 (可选择范围由 driver_gpio.h 内 gpio_pin_enum 枚举值确定)
// 返回参数     void
// 使用示例     gpio_toggle_level(P01_0);// P01_0引脚电平翻转
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void gpio_toggle_level(gpio_pin_enum pin)
{
    uint32 pin_value = 0;
    pin_value |= (pin & 0x1f);//目的为转换为pin口的0-31

    GPIO_Type* gpio_type;
    gpio_type = get_port(pin);

    if ( ( ((get_port(pin)->DR) >> pin_value) & 0x1U ) == 0U)
    {
        gpio_type->DR |= (1U << pin_value);  /* Set pin output to high level.*/
    }
    else
    {
        gpio_type->DR &= ~(1U << pin_value);  /* Set pin output to low level.*/
    }
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     gpio 初始化
// 参数说明     pin             选择的引脚 (可选择范围由 driver_gpio.h 内 gpio_pin_enum 枚举值确定)
// 参数说明     dir             引脚的方向 [GPI/GPO]
// 参数说明     dat             引脚初始化时设置的电平状态，输出时有效, 0：低电平 1：高电平
// 返回参数     void
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void gpio_init(gpio_pin_enum pin, gpio_dir_enum dir, uint8 dat)
{
    uint32 pin_value = 0 ;
    pin_value |= (pin & 0x1f);//目的为转换为pin口的0-31

    gpio_pin_config_t gpio_pin_config;
    gpio_pin_config.direction = dir;

    if (dir == GPI)
    {
        GPIO_PinInit(get_port(pin), pin_value, &gpio_pin_config);
    }
    else if (dir == GPO)
    {        
        gpio_pin_config.outputLogic = dat;
        GPIO_PinInit(get_port(pin), pin_value, &gpio_pin_config);
    }  
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     gpio 端口模式设置
// 参数说明     muxRegister     The pin mux register,       decide on file "fsl_iomux.h"
// 参数说明     muxMode         The pin mux mode,           decide on file "fsl_iomux.h"
// 参数说明     inputRegister   The select input register,  decide on file "fsl_iomux.h"
// 参数说明     inputDaisy      The input daisy,            decide on file "fsl_iomux.h"
// 参数说明     configRegister  The config register,        decide on file "fsl_iomux.h"
// 参数说明     inputOnfield    Software input on field,    default 0
// 参数说明     configValue     The pin config value,       below for details
// 返回参数     void
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
#if 0
//void iomuxc_set(uint32 muxRegister, uint32 muxMode, uint32 inputRegister,uint32 inputDaisy, uint32 configRegister, uint32 inputOnfield, uint32 configValue)
//{
    /* 初始化IO复用*/
    //IOMUXC_SetPinMux(muxRegister, muxMode, inputRegister, inputDaisy, configRegister, inputOnfield);
    /* 配置IO属性——configValue
    *      Field      |                                 Description
    *bit [16]:        |  0:   Hysteresis Disabled;    1:  Hysteresis Enabled 
    *bit [15:14]:     |  00:  100K Pull Down;    01: 47K Pull Up;    10: 100K Pull Up;    11:22K Pull Up
    *bit [13]:        |  0:   Keeper;  1:Pull
    *bit [12]:        |  0:   Pull/Keeper Disabled;    1: Pull/Keeper Enabled
    *bit [11]:        |  0:   Open Drain Disabled;     1:Open Drain Enabled
    *bit [8-10]:      |  0：  Reserved
    *bit [7:6]:       |  00:  low(50MHz);     01: medium(100MHz);     10:  medium(100MHz);    11: max(200MHz)
    *bit [5:3]:       |  000: output driver disabled; 
                      |  001: R0(260 Ohm @3.3V, 150 Ohm@1.8V, 240 Ohm for DDR)
                      |  010: DSE_2_R0_2 — R0/2
                      |  011: DSE_3_R0_3 — R0/3
                      |  100: DSE_4_R0_4 — R0/4
                      |  101: DSE_5_R0_5 — R0/5
                      |  110: DSE_6_R0_6 — R0/6
                      |  111: DSE_7_R0_7 — R0/7
    *bit [2-1]:       |  0：  Reserved
    *bit [0]: 0       |  0:   Slow Slew Rate;  1: Fast Slew Rate    
    */
    //IOMUXC_SetPinConfig(muxRegister, muxMode, inputRegister, inputDaisy, configRegister, configValue);
//}
#else
void iomuxc_set(iomuxc_value_t iomux, uint32 inputOnfield, uint32 configValue)
{
    /* 初始化IO复用*/
    IOMUXC_SetPinMux(iomux.muxRegister, iomux.muxMode, iomux.inputRegister,iomux.inputDaisy, iomux.configRegister, inputOnfield);
    /* 配置IO属性——configValue
    *      Field      |                                 Description
    *bit [16]:        |  0:   Hysteresis Disabled;    1:  Hysteresis Enabled 
    *bit [15:14]:     |  00:  100K Pull Down;    01: 47K Pull Up;    10: 100K Pull Up;    11:22K Pull Up
    *bit [13]:        |  0:   Keeper;  1:Pull
    *bit [12]:        |  0:   Pull/Keeper Disabled;    1: Pull/Keeper Enabled
    *bit [11]:        |  0:   Open Drain Disabled;     1:Open Drain Enabled
    *bit [8-10]:      |  0：  Reserved
    *bit [7:6]:       |  00:  low(50MHz);     01: medium(100MHz);     10:  medium(100MHz);    11: max(200MHz)
    *bit [5:3]:       |  000: output driver disabled; 
                      |  001: R0(260 Ohm @3.3V, 150 Ohm@1.8V, 240 Ohm for DDR)
                      |  010: DSE_2_R0_2 — R0/2
                      |  011: DSE_3_R0_3 — R0/3
                      |  100: DSE_4_R0_4 — R0/4
                      |  101: DSE_5_R0_5 — R0/5
                      |  110: DSE_6_R0_6 — R0/6
                      |  111: DSE_7_R0_7 — R0/7
    *bit [2-1]:       |  0：  Reserved
    *bit [0]: 0       |  0:   Slow Slew Rate;  1: Fast Slew Rate    
    */
    IOMUXC_SetPinConfig(iomux.muxRegister, iomux.muxMode, iomux.inputRegister,iomux.inputDaisy, iomux.configRegister, configValue);
}
#endif