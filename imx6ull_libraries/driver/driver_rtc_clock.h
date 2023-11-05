#ifndef __DRIVER_RTC_CLOCK__H__
#define __DRIVER_RTC_CLOCK__H__

#include "common_typedef.h"

/* 相关宏定义 */	
#define SECONDS_IN_A_DAY 		(86400) // 一天86400秒	
#define SECONDS_IN_A_HOUR 		(3600)	// 一个小时3600秒 
#define SECONDS_IN_A_MINUTE 	(60)	// 一分钟60秒
#define DAYS_IN_A_YEAR 			(365)	// 一年365天 
#define YEAR_RANGE_START 		(1970)	// 开始年份1970年 
#define YEAR_RANGE_END 			(2099)	// 结束年份2099年 

/* 时间日期结构体 */	
struct rtc_datetime
{
    uint16 rtc_year;     //范围为:1970 ~ 2099
    uint8 rtc_month;            //范围为:1 ~ 12
    uint8 rtc_day;              //范围为:1 ~ 31 (不同的月，天数不同)
    uint8 rtc_hour;             // 范围为:0 ~ 23 	
    uint8 rtc_minute;           // 范围为:0 ~ 59	
    uint8 rtc_second;           // 范围为:0 ~ 59	
}rtc_datetime_t;


/* 函数声明 */
void rtc_init(void);
void rtc_enable(void);
void rtc_disable(void);
uint32 rtc_coverdate_to_seconds(struct rtc_datetime *datetime);
uint32 rtc_getseconds(void);
void rtc_setdatetime(struct rtc_datetime *datetime);
void rtc_getdatetime(struct rtc_datetime *datetime);
void rtc_update_time(uint16 year, uint16 month, uint16 day, uint16 hour, uint16 minute, uint16 second);

#endif  //!__DRIVER_RTC_CLOCK__H__