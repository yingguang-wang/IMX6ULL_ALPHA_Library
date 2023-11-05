#include "driver_exit.h"
#include "driver_gpio.h"
#include "isr.h"
#include "common_headfile.h"
//-------------------------------------------------------------------------------------------------------------------
//  函数简介      中断标志位获取
//  参数说明      pin		中断端口
//  返回参数      uint32	0/1
//  备注信息
//-------------------------------------------------------------------------------------------------------------------
uint32 exti_flag_get(gpio_pin_enum pin)		 
{   
	uint32 flag;
	flag = get_port(pin)->ISR >> ( 0 | (pin & 0x1f) );
	return flag & 1;   
}
//-------------------------------------------------------------------------------------------------------------------
//  函数简介      中断标志位清除
//  参数说明      pin		中断端口
//  返回参数      void
//  备注信息
//-------------------------------------------------------------------------------------------------------------------
void exti_flag_clear(gpio_pin_enum pin)      
{  
	get_port(pin)->ISR |= (1 << ( 0 | (pin & 0x1f) ));   
}

//-------------------------------------------------------------------------------------------------------------------
//  函数简介      EXTI 中断初始化,注册"isr.c"文件对应端口中断函数
//  参数说明      pin		设置中断端口
//  参数说明      iomux     设置对应中断端口配置，由"fsl_iomuxc.h"文件IOMUXC_..._..._..._GPIOX_IOXX宏定义决定
//  参数说明      trigger   设置触发方式,(由gpio_interrupt_mode_t枚举值决定)
//  返回参数      void
//  备注信息
//-------------------------------------------------------------------------------------------------------------------
//void exti_init(gpio_pin_enum pin, iomuxc_value_t iomux, gpio_interrupt_mode_t trigger)
void exti_init(gpio_pin_enum pin, gpio_interrupt_mode_t trigger)
{
    uint32 pin_value = 0;
    pin_value |= (pin & 0x1f);//目的为转换为pin口的0-31

    gpio_pin_config_t gpio_pin_config;
    gpio_pin_config.direction = kGPIO_DigitalInput;
	gpio_pin_config.interruptMode = trigger;
	gpio_pin_config.outputLogic = 1;
    GPIO_PinInit(get_port(pin), pin_value, &gpio_pin_config);  

	//iomuxc_set(iomux, 0, 0xF080);
	if (pin >= 32 && pin < 64)
	{
		if (pin_value >= 0 && pin_value < 16)
		{
			GIC_EnableIRQ(GPIO1_Combined_0_15_IRQn);
			SystemInstallIrqHandler(GPIO1_Combined_0_15_IRQn, 
				(system_irq_handler_t)GPIO1_Combined_0_15_Irq_Handler, 
				NULL);	/* 注册中断服务函数 */
		}
		else
		{
			GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);				/* 使能GIC中对应的中断 */
			SystemInstallIrqHandler(GPIO1_Combined_16_31_IRQn, 
				(system_irq_handler_t)GPIO1_Combined_16_31_Irq_Handler, 
				NULL);	/* 注册中断服务函数 */
		}
	}
	else if (pin < 96)
	{
		if (pin_value >= 0 && pin_value < 16)
		{
			GIC_EnableIRQ(GPIO2_Combined_0_15_IRQn);
			SystemInstallIrqHandler(GPIO2_Combined_0_15_IRQn,
				(system_irq_handler_t)GPIO2_Combined_0_15_Irq_Handler,
				NULL);	/* 注册中断服务函数 */
		}
		else
		{
			GIC_EnableIRQ(GPIO2_Combined_16_31_IRQn);				/* 使能GIC中对应的中断 */
			SystemInstallIrqHandler(GPIO2_Combined_16_31_IRQn,
				(system_irq_handler_t)GPIO2_Combined_16_31_Irq_Handler,
				NULL);	/* 注册中断服务函数 */
		}
	}
	else if (pin < 128)
	{
		if (pin_value >= 0 && pin_value < 16)
		{
			GIC_EnableIRQ(GPIO3_Combined_0_15_IRQn);
			SystemInstallIrqHandler(GPIO3_Combined_0_15_IRQn,
				(system_irq_handler_t)GPIO3_Combined_0_15_Irq_Handler,
				NULL);	/* 注册中断服务函数 */
		}
		else
		{
			GIC_EnableIRQ(GPIO3_Combined_16_31_IRQn);				/* 使能GIC中对应的中断 */
			SystemInstallIrqHandler(GPIO3_Combined_16_31_IRQn,
				(system_irq_handler_t)GPIO3_Combined_16_31_Irq_Handler,
				NULL);	/* 注册中断服务函数 */
		}
	}
	else if (pin < 160)
	{
		if (pin_value >= 0 && pin_value < 16)
		{
			GIC_EnableIRQ(GPIO4_Combined_0_15_IRQn);
			SystemInstallIrqHandler(GPIO4_Combined_0_15_IRQn,
				(system_irq_handler_t)GPIO4_Combined_0_15_Irq_Handler,
				NULL);	/* 注册中断服务函数 */
		}
		else
		{
			GIC_EnableIRQ(GPIO4_Combined_16_31_IRQn);				/* 使能GIC中对应的中断 */
			SystemInstallIrqHandler(GPIO4_Combined_16_31_IRQn,
				(system_irq_handler_t)GPIO4_Combined_16_31_Irq_Handler,
				NULL);	/* 注册中断服务函数 */
		}
	}
	else if (pin < 192)
	{
		if (pin_value >= 0 && pin_value < 16)
		{
			GIC_EnableIRQ(GPIO5_Combined_0_15_IRQn);
			SystemInstallIrqHandler(GPIO5_Combined_0_15_IRQn,
				(system_irq_handler_t)GPIO5_Combined_0_15_Irq_Handler,
				NULL);	/* 注册中断服务函数 */
		}
		else
		{
			GIC_EnableIRQ(GPIO5_Combined_16_31_IRQn);				/* 使能GIC中对应的中断 */
			SystemInstallIrqHandler(GPIO5_Combined_16_31_IRQn,
				(system_irq_handler_t)GPIO5_Combined_16_31_Irq_Handler,
				NULL);	/* 注册中断服务函数 */
		}
	}
	GPIO_EnableInterrupts(get_port(pin), pin_value);
}
