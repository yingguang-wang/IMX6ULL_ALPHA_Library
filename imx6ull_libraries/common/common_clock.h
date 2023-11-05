#ifndef _COMMON_CLOCK_H_
#define _COMMON_CLOCK_H_


//==================================================== 系统初始化 基础函数=================================================
void clk_enable(void);		        //全局CCR时钟初始化
void imx6u_clkinit(void);	        //初始化系统时钟
void imx6ul_hardfpu_enable(void);   //开启硬件浮点运算
void system_init(void);		        //系统初始化
//==================================================== 系统初始化 基础函数=================================================

#endif // !_COMMON_CLOCK_H_
