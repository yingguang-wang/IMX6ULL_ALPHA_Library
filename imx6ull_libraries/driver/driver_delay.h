#ifndef _DRIVER_DELAY_H_
#define _DRIVER_DELAY_H_

#include "common_typedef.h"
#include "MCIMX6Y2.h"

typedef enum _gpt_clock_source
{
    kGPT_ClockSource_Off = 0U,      /*!< GPT Clock Source Off.*/
    kGPT_ClockSource_Periph = 1U,   /*!< GPT Clock Source from Peripheral Clock.*/
    kGPT_ClockSource_HighFreq = 2U, /*!< GPT Clock Source from High Frequency Reference Clock.*/
    kGPT_ClockSource_Ext = 3U,      /*!< GPT Clock Source from external pin.*/
    kGPT_ClockSource_LowFreq = 4U,  /*!< GPT Clock Source from Low Frequency Reference Clock.*/
    kGPT_ClockSource_Osc = 5U,      /*!< GPT Clock Source from Crystal oscillator.*/
} gpt_clock_source_t;

/*采用GPT定时器实现精准延时*/
void gpt_clock_delay_init(void);
void system_delay(uint32 time, uint32 num);

//====================================================延时 基础函数====================================================
#define system_delay_ms(time)       system_delay((1000), (time))    // 设置延时时间  单位ms
#define system_delay_us(time)       system_delay((time), (1))     // 设置延时时间  单位us
//====================================================延时 基础函数====================================================


#endif // !_DRIVER_DELAY_H_
