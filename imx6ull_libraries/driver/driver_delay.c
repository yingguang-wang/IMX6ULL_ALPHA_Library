﻿#include "driver_delay.h"

//-------------------------------------------------------------------------------------------------------------------
//  函数简介      gpt定时器初始化
//  参数说明      void      
//  返回参数      void
//  使用示例      
//-------------------------------------------------------------------------------------------------------------------
void gpt_clock_delay_init(void)
{
    GPT1->CR = 0; 					/* 清零，bit0也为0，即停止GPT  			*/

    GPT1->CR = 1 << 15;				/* bit15置1进入软复位 				*/
    while ((GPT1->CR >> 15) & 0x01);	/*等待复位完成 						*/
    /*
     * GPT的CR寄存器,GPT通用设置
     * bit [22-20]	000 输出比较1的输出功能关闭，也就是对应的引脚没反应
     * bit [9]:     0   Restart模式,当CNT等于OCR1的时候就产生中断
     * bit [8:6]    001 GPT时钟源选择ipg_clk=66Mhz
     * bit
     */
    GPT1->CR = (1 << 6);
    /*
     * GPT的PR寄存器，GPT的分频设置
     * bit11:0  设置分频值，设置为0表示1分频，
     *          以此类推，最大可以设置为0XFFF，也就是最大4096分频
     */
    GPT1->PR = 65;	/* 设置为65，即66分频，因此GPTX时钟为66M/(65+1)=1MHz */

    /*
     * GPT的OCR1寄存器，GPT的输出比较1比较计数值，
     * GPT的时钟为1Mz，那么计数器每计一个值就是就是1us。
     * 为了实现较大的计数，我们将比较值设置为最大的0XFFFFFFFF,
     * 这样一次计满就是：0XFFFFFFFFus = 4294967296us = 4295s = 71.5min
     * 也就是说一次计满最多71.5分钟，存在溢出
     */
    GPT1->OCR[0] = 0XFFFFFFFF;

    GPT1->CR |= 1 << 0;			//使能GPT
}
//-------------------------------------------------------------------------------------------------------------------
//  函数简介      systick延时函数
//  参数说明      time            延时一轮的时间（单位为纳秒，可设置范围0-20000000）
//  参数说明      num             延时多少轮
//  返回参数      void
//  使用示例      无需用户调用，用户请使用h文件中的宏定义
//-------------------------------------------------------------------------------------------------------------------
void system_delay(uint32 time, uint32 num)
{
    uint32 oldcnt, newcnt;
    uint32 tcntvalue = 0;	/* 走过的总时间  */
    while (num--)
    {
        tcntvalue = 0;
        oldcnt = GPT1->CNT;
        while (1)
        {
            newcnt = GPT1->CNT;
            if (newcnt != oldcnt)
            {
                if (newcnt > oldcnt)		/* GPT是向上计数器,并且没有溢出 */
                {
                    tcntvalue += newcnt - oldcnt;
                }
                else  					    /* 发生溢出    */
                {
                    tcntvalue += 0XFFFFFFFF - oldcnt + newcnt;
                }
                oldcnt = newcnt;
                if (tcntvalue >= time)   /* 延时时间到了 */
                {
                    break;			 		/*  跳出 */
                }
            }
        }
    }
}