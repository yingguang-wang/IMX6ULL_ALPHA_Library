#include "driver_pwm.h"
#include "buzzer.h"
#define IPG_CLK_FREQ           1000000.0f                       // IPG时钟频率

pwm_duty_value_t pwm_duty_value;
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     使能PWM
// 参数说明     void     
// 返回参数     void 
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void pwm_enable(PWM_Type *base)
{
	base->PWMCR |= 1 << 0;	 
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     失能PWM
// 参数说明     void     
// 返回参数     void 
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void pwm_disable(PWM_Type *base)
{
	base->PWMCR &= ~(1 << 0);	 
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     设置Sample寄存器，Sample数据会写入到FIFO中，
// 参数说明     base            选择 PWM 对应端口
// 参数说明     value           寄存器值，范围0~0XFFFF
// 返回参数     void 
// 备注信息     所谓的Sample寄存器，就相当于比较寄存器，假如PWMCR中的POUTC设置为00的时候。
//             当PWM计数器中的计数值小于Sample的时候,就会输出高电平，当PWM计数器值大于Sample的时候输出底电平,
//-------------------------------------------------------------------------------------------------------------------
inline void PWM_SetSampleValue(PWM_Type *base, uint32_t value)
{
    base->PWMSAR = (value & PWM_PWMSAR_SAMPLE_MASK);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PWM 设置PWM周期
// 参数说明     base            选择 PWM 对应端口
// 参数说明     value           周期值，范围0~0XFFFF
// 返回参数     void 
// 备注信息     就是设置寄存器PWMPR，PWM周期公式如下: PWM_FRE = PWM_CLK / (PERIOD + 2)，
//             比如当前PWM_CLK=1MHz,要产生1KHz的PWM，那么PERIOD = 1000000/1K - 2 = 	998
//-------------------------------------------------------------------------------------------------------------------
inline void PWM_SetPeriodValue(PWM_Type *base, uint32_t value)
{
    uint32 regvalue = 0;
	if(value < 2)
		regvalue = 2;
	else 
		regvalue = value - 2;
    base->PWMPR = (regvalue & PWM_PWMPR_PERIOD_MASK);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PWM 设置占空比
// 参数说明     base            选择 PWM 对应端口
// 参数说明     duty            设置占空比
// 返回参数     void 
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void pwm_set_duty(PWM_Type *base, uint32 duty)
{
	uint32 preiod;
	uint32 sample;

	preiod = base->PWMPR + 2;
	sample = ( preiod * pwm_duty_value.pwm_duty[save_pwm_duty(base, duty)] )/ PWM_DUTY_MAX;
	PWM_SetSampleValue(base, sample);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PWM 初始化
// 参数说明     base            选择 PWM 对应端口
// 参数说明     iomuxc          选择 PWM 引脚
// 参数说明     freq            设置频率 同个模块只有最后一次设置生效
// 参数说明     duty            设置占空比
// 返回参数     void   
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void pwm_init(PWM_Type *base, iomuxc_value_t iomuxc, uint32 freq, uint32 duty)
{
    uint8 i;
    iomuxc_set(iomuxc, 0, 0XB090);
   
	/* 初始化PWM */
	/*
   	 * 初始化寄存器PWMCR
   	 * bit[27:26]	: 01  当FIFO中空余位置大于等于2的时候FIFO空标志值位
   	 * bit[25]		: 0   停止模式下PWM不工作
   	 * bit[24]		: 0	  休眠模式下PWM不工作
   	 * bit[23]		: 0   等待模式下PWM不工作
   	 * bit[22]		: 0   调试模式下PWM不工作
   	 * bit[21]		: 0   关闭字节交换
   	 * bit[20]		: 0	  关闭半字数据交换
   	 * bit[19:18]	: 00  PWM输出引脚在计数器重新计数的时候输出高电平
   	 *					  在计数器计数值达到比较值以后输出低电平
   	 * bit[17:16]	: 01  PWM时钟源选择IPG CLK = 66MHz
   	 * bit[15:4]	: 65  分频系数为65+1=66，PWM时钟源 = 66MHZ/66=1MHz
   	 * bit[3]		: 0	  PWM不复位
   	 * bit[2:1]		: 00  FIFO中的sample数据每个只能使用一次。
   	 * bit[0]		: 0   先关闭PWM，后面再使能
	 */
	base->PWMCR = 0;	// 寄存器先清零
	base->PWMCR |= (1 << 26) | (1 << 16) | (65 << 4);

	PWM_SetPeriodValue(base, IPG_CLK_FREQ / freq);
	for(i = 0; i < 4; i++)
	{
		pwm_set_duty(base, duty);	
	}
	
	/* 使能FIFO空中断，设置寄存器PWMIR寄存器的bit0为1 */
	base->PWMIR |= 1 << 0;
    Pwm_Install_IrqHandler(base);
	base->PWMSR = 0;			    // PWM中断状态寄存器清零
	
	pwm_enable(base);				// 使能PWM
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PWM 保存占空比信息
// 参数说明     base        选择 PWM 对应端口
// 返回参数     uint8_t     占空比保存位号   
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 save_pwm_duty(PWM_Type *base, uint32 duty)
{
    if      (base == PWM1)   {  pwm_duty_value.pwm_duty[1] = duty;    return 1; }
    else if (base == PWM2)   {  pwm_duty_value.pwm_duty[2] = duty;    return 2; }   
    else if (base == PWM3)   {  pwm_duty_value.pwm_duty[3] = duty;    return 3; }   
    else if (base == PWM4)   {  pwm_duty_value.pwm_duty[4] = duty;    return 4; }   
    else if (base == PWM5)   {  pwm_duty_value.pwm_duty[5] = duty;    return 5; }    
    else if (base == PWM6)   {  pwm_duty_value.pwm_duty[6] = duty;    return 6; }   
    else if (base == PWM7)   {  pwm_duty_value.pwm_duty[7] = duty;    return 7; }   
    else if (base == PWM8)   {  pwm_duty_value.pwm_duty[8] = duty;    return 8; }    
    else return 0;
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PWM 注册中断函数
// 参数说明     base            选择 PWM 对应端口
// 返回参数     void   
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void Pwm_Install_IrqHandler(PWM_Type *base)
{   
    if (base == PWM1)
	{
		SystemInstallIrqHandler(PWM1_IRQn, (system_irq_handler_t)PWM1_Irq_Handler, NULL);	/* 注册中断服务函数 */
		GIC_EnableIRQ(PWM1_IRQn);
	}    
    else if (base == PWM2)
	{		
		SystemInstallIrqHandler(PWM2_IRQn, (system_irq_handler_t)PWM2_Irq_Handler, NULL);	/* 注册中断服务函数 */
	    GIC_EnableIRQ(PWM2_IRQn);
    }
    else if (base == PWM3)
	{		
		SystemInstallIrqHandler(PWM3_IRQn, (system_irq_handler_t)PWM3_Irq_Handler, NULL);	/* 注册中断服务函数 */
	    GIC_EnableIRQ(PWM3_IRQn);
    }
    else if (base == PWM4)
	{
		SystemInstallIrqHandler(PWM4_IRQn, (system_irq_handler_t)PWM4_Irq_Handler, NULL);	/* 注册中断服务函数 */
	    GIC_EnableIRQ(PWM4_IRQn);
    }
    else if (base == PWM5)
	{		
		SystemInstallIrqHandler(PWM5_IRQn, (system_irq_handler_t)PWM5_Irq_Handler, NULL);	/* 注册中断服务函数 */
	    GIC_EnableIRQ(PWM5_IRQn);
    }
    else if (base == PWM6)
	{
		SystemInstallIrqHandler(PWM6_IRQn, (system_irq_handler_t)PWM6_Irq_Handler, NULL);	/* 注册中断服务函数 */
		GIC_EnableIRQ(PWM6_IRQn);
    }    
    else if (base == PWM7)
	{
		SystemInstallIrqHandler(PWM7_IRQn, (system_irq_handler_t)PWM7_Irq_Handler, NULL);	/* 注册中断服务函数 */	
		GIC_EnableIRQ(PWM7_IRQn);
    }    
    else if (base == PWM8)
	{
		SystemInstallIrqHandler(PWM8_IRQn, (system_irq_handler_t)PWM8_Irq_Handler, NULL);	/* 注册中断服务函数 */
		GIC_EnableIRQ(PWM8_IRQn);
    }
}


/*******************************************************PWM中断服务函数*******************************************************/
void PWM1_Irq_Handler(void)
{    
	if(PWM1->PWMSR & (1 << 3)) 	    /* FIFO为空中断 */
	{
		/* 将占空比信息写入到FIFO中,其实就是设置占空比 */
		pwm_set_duty(PWM1,  pwm_duty_value.pwm_duty[1]); 
		PWM1->PWMSR |= (1 << 3); /* 写1清除中断标志位 */ 
	}
}
void PWM2_Irq_Handler(void)
{ 
	if(PWM2->PWMSR & (1 << 3)) 	    /* FIFO为空中断 */
	{      
		/* 将占空比信息写入到FIFO中,其实就是设置占空比 */
		pwm_set_duty(PWM2,  pwm_duty_value.pwm_duty[2]); 
		PWM2->PWMSR |= (1 << 3); /* 写1清除中断标志位 */ 
	}
}
void PWM3_Irq_Handler(void)
{    
	if(PWM3->PWMSR & (1 << 3)) 	    /* FIFO为空中断 */
	{
		/* 将占空比信息写入到FIFO中,其实就是设置占空比 */
		pwm_set_duty(PWM3,  pwm_duty_value.pwm_duty[3]); 
		PWM3->PWMSR |= (1 << 3); /* 写1清除中断标志位 */ 
	}
}
void PWM4_Irq_Handler(void)
{    
	if(PWM4->PWMSR & (1 << 3)) 	    /* FIFO为空中断 */
	{
		/* 将占空比信息写入到FIFO中,其实就是设置占空比 */
		pwm_set_duty(PWM4,  pwm_duty_value.pwm_duty[4]); 
		PWM4->PWMSR |= (1 << 3); /* 写1清除中断标志位 */ 
	}
}
void PWM5_Irq_Handler(void)
{    
	if(PWM5->PWMSR & (1 << 3)) 	    /* FIFO为空中断 */
	{
		/* 将占空比信息写入到FIFO中,其实就是设置占空比 */
		pwm_set_duty(PWM5,  pwm_duty_value.pwm_duty[5]); 
		PWM5->PWMSR |= (1 << 3); /* 写1清除中断标志位 */ 
	}
}
void PWM6_Irq_Handler(void)
{    
	if(PWM6->PWMSR & (1 << 3)) 	    /* FIFO为空中断 */
	{
		/* 将占空比信息写入到FIFO中,其实就是设置占空比 */
		pwm_set_duty(PWM6,  pwm_duty_value.pwm_duty[6]); 
		PWM6->PWMSR |= (1 << 3); /* 写1清除中断标志位 */ 
	}
}
void PWM7_Irq_Handler(void)
{    
	if(PWM7->PWMSR & (1 << 3)) 	    /* FIFO为空中断 */
	{
		/* 将占空比信息写入到FIFO中,其实就是设置占空比 */
		pwm_set_duty(PWM7,  pwm_duty_value.pwm_duty[7]); 
		PWM7->PWMSR |= (1 << 3); /* 写1清除中断标志位 */ 
	}
}
void PWM8_Irq_Handler(void)
{    
	if(PWM8->PWMSR & (1 << 3)) 	    /* FIFO为空中断 */
	{
		/* 将占空比信息写入到FIFO中,其实就是设置占空比 */
		pwm_set_duty(PWM8,  pwm_duty_value.pwm_duty[8]); 
		PWM8->PWMSR |= (1 << 3); /* 写1清除中断标志位 */ 
	}
}
/*******************************************************PWM中断服务函数*******************************************************/




