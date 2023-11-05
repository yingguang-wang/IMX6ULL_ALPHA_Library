#include "common_headfile.h"


// **************************** EPIT中断函数 ****************************
void EPIT1_IRQn_Irq_Handler(void)
{

	if (pit_flag_get(EPIT1))
	{
		pit_flag_clear(EPIT1);
		
		pit_close(EPIT1);
		if (gpio_get_level(P01_18) == 0)	/* KEY0 				*/
		{
			Buzzer(70);				/* 反转蜂鸣器 				*/
		}
		
	}
}
void EPIT2_IRQn_Irq_Handler(void)
{
	if (pit_flag_get(EPIT2))
	{		
		pit_flag_clear(EPIT2);
		gpio_toggle_level(P01_3);


	}
}

// **************************** EPIT中断函数 ****************************
 

// **************************** 外部中断函数 ****************************

void GPIO1_Combined_0_15_Irq_Handler(void)
{
	if(exti_flag_get(P01_9))		//由选择的引脚决定
	{
		exti_flag_clear(P01_9);		//此引脚默认被TFT-LCD占用

		if(gt9147_dev.initfalg == GT9147_INIT_FINISHED)
		{

			gt9147_dev.intflag = 1;
			gt9147_read_tpcoord();
		}
		else if(ft5426_dev.initfalg == FT5426_INIT_FINISHED)
		{
			ft5426_dev.intflag = 1;
			ft5426_read_tpcoord();
		}
	}	
	
}
void GPIO1_Combined_16_31_Irq_Handler(void)
{
	if(exti_flag_get(P01_18))		//由选择的引脚决定
	{
		//Buzzer(100);
		pit_restart(EPIT1);
		
		exti_flag_clear(P01_18);
	}
}

void GPIO2_Combined_0_15_Irq_Handler(void)
{
	exti_flag_clear(P02_0);
}
void GPIO2_Combined_16_31_Irq_Handler(void)
{
	exti_flag_clear(P02_0);
}

void GPIO3_Combined_0_15_Irq_Handler(void)
{
	exti_flag_clear(P03_0);
}
void GPIO3_Combined_16_31_Irq_Handler(void)
{
	exti_flag_clear(P03_0);
}

void GPIO4_Combined_0_15_Irq_Handler(void)
{
	exti_flag_clear(P04_0);
}
void GPIO4_Combined_16_31_Irq_Handler(void)
{
	exti_flag_clear(P04_0);
}

void GPIO5_Combined_0_15_Irq_Handler(void)
{
	exti_flag_clear(P05_0);
}
void GPIO5_Combined_16_31_Irq_Handler(void)
{
	exti_flag_clear(P05_0);
}

// **************************** 外部中断函数 ****************************