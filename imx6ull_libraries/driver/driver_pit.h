#ifndef  _DRIVER_PIT_H_
#define _DRIVER_PIT_H_

//#include "fsl_epit.h"
#include "common_typedef.h"
#include "fsl_common.h"
#if 1
typedef enum _epit_clock_source
{
    kEPIT_ClockSource_Off = 0U,      /*!< EPIT Clock Source Off.*/
    kEPIT_ClockSource_Periph = 1U,   /*!< EPIT Clock Source from Peripheral Clock.*/
    kEPIT_ClockSource_HighFreq = 2U, /*!< EPIT Clock Source from High Frequency Reference Clock.*/
    kEPIT_ClockSource_LowFreq = 3U,  /*!< EPIT Clock Source from Low Frequency Reference Clock.*/
} epit_clock_source_t;
#endif
//获取pit中断标识位
uint8 pit_flag_get(EPIT_Type* base);
//清除pit中断标识位
void pit_flag_clear(EPIT_Type* base);
//====================================================PIT 基础函数====================================================
void pit_close(EPIT_Type* base);
void pit_restart(EPIT_Type* base);
void pit_init(EPIT_Type* base, uint32 time);
//====================================================PIT 基础函数====================================================

//====================================================PIT 扩展函数====================================================
//-------------------------------------------------------------------------------------------------------------------
//  函数简介      pit_ms初始化
//  参数说明      pit_index           选择EPIT_Type模块
//  参数说明      time                周期时间(单位：毫秒)
//  返回参数      void
//  使用示例      pit_ms_init(EPIT1_IRQn, 5);      // 设置周期中断5ms
//-------------------------------------------------------------------------------------------------------------------
#define pit_ms_init(base, time)  pit_init(base, (time*1000))   // (单位为 毫秒)
//-------------------------------------------------------------------------------------------------------------------
//  函数简介      pit_us初始化
//  参数说明      pit_index           选择EPIT_Type模块
//  参数说明      time                周期时间(单位：微秒)
//  返回参数      void
//  使用示例      pit_us_init(EPIT1_IRQn, 5);      // 设置周期中断5us
//-------------------------------------------------------------------------------------------------------------------
#define pit_us_init(base, time)  pit_init(base, (time))        // (单位为 微秒)
//====================================================PIT 扩展函数====================================================



#endif // !_DRIVER_EPIT_H_
