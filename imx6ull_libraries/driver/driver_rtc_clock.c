#include "driver_rtc_clock.h"
#include "MCIMX6Y2.h"

//-------------------------------------------------------------------------------------------------------------------
//  函数简介      RTC初始化
//  参数说明      void           
//  返回参数      void         
//  备注信息
//-------------------------------------------------------------------------------------------------------------------
void rtc_init(void)
{
	/*
     * 设置HPCOMR寄存器
     * bit[31] 1 : 允许访问SNVS寄存器，一定要置1
     * bit[8]  1 : 此位置1，需要签署NDA协议才能看到此位的详细说明，
     *             这里不置1也没问题
	 */
	SNVS->HPCOMR |= (1 << 31) | (1 << 8);
	
#if 0
	struct rtc_datetime rtcdate;

	rtcdate.year = 2018U;
    rtcdate.month = 12U;
    rtcdate.day = 13U;
    rtcdate.hour = 14U;
    rtcdate.minute = 52;
    rtcdate.second = 0;
	rtc_setDatetime(&rtcdate); //初始化时间和日期
#endif
	
	rtc_enable();	//使能RTC

}
//-------------------------------------------------------------------------------------------------------------------
//  函数简介      RTC使能
//  参数说明      void           
//  返回参数      void         
//  备注信息
//-------------------------------------------------------------------------------------------------------------------
void rtc_enable(void)
{
	/*
	 * LPCR寄存器bit0置1，使能RTC
 	 */
	SNVS->LPCR |= 1 << 0;	
	while(!(SNVS->LPCR & 0X01));//等待使能完成
	
}
//-------------------------------------------------------------------------------------------------------------------
//  函数简介      RTC失能
//  参数说明      void           
//  返回参数      void         
//  备注信息
//-------------------------------------------------------------------------------------------------------------------
void rtc_disable(void)
{
	/*
	 * LPCR寄存器bit0置0，关闭RTC
 	 */
	SNVS->LPCR &= ~(1 << 0);	
	while(SNVS->LPCR & 0X01);//等待关闭完成
}
//-------------------------------------------------------------------------------------------------------------------
//  函数简介      判断指定年份是否为闰年
//  参数说明      year      要判断的年份      
//  返回参数      uint8     1 是闰年，0 不是闰年   
//  备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8 rtc_isleapyear(uint16 year)
{	
	uint8 value=0;
	
	if(year % 400 == 0)
		value = 1;
	else 
	{
		if((year % 4 == 0) && (year % 100 != 0))
			value = 1;
		else 
			value = 0;
	}
	return value;
}
//-------------------------------------------------------------------------------------------------------------------
//  函数简介      将时间转换为秒数
//  参数说明      datetime      要转换日期和时间。      
//  返回参数      uint32        转换后的秒数 
//  备注信息
//-------------------------------------------------------------------------------------------------------------------
uint32 rtc_coverdate_to_seconds(struct rtc_datetime *datetime)
{	
	uint16 i = 0;
	uint32 seconds = 0;
	uint32 days = 0;
	uint16 monthdays[] = {0U, 0U, 31U, 59U, 90U, 120U, 151U, 181U, 212U, 243U, 273U, 304U, 334U};
	
	for(i = 1970; i < datetime->rtc_year; i++)
	{
		days += DAYS_IN_A_YEAR; 		// 平年，每年365天 
		if(rtc_isleapyear(i)) days += 1;//闰年多加一天 
	}

	days += monthdays[datetime->rtc_month];
	if(rtc_isleapyear(i) && (datetime->rtc_month >= 3)) days += 1;//闰年，并且当前月份大于等于3月的话加一天

	days += datetime->rtc_day - 1;

	seconds = days * SECONDS_IN_A_DAY + 
				datetime->rtc_hour * SECONDS_IN_A_HOUR +
				datetime->rtc_minute * SECONDS_IN_A_MINUTE +
				datetime->rtc_second;

	return seconds;	
}
//-------------------------------------------------------------------------------------------------------------------
//  函数简介      设置时间和日期
//  参数说明      datetime      要设置的日期和时间      
//  返回参数      void         
//  备注信息
//-------------------------------------------------------------------------------------------------------------------
void rtc_setdatetime(struct rtc_datetime *datetime)
{
	
	uint32 seconds = 0;
	uint32 tmp = SNVS->LPCR; 
	
	rtc_disable();	/* 设置寄存器HPRTCMR和HPRTCLR的时候一定要先关闭RTC */

	
	/* 先将时间转换为秒         */
	seconds = rtc_coverdate_to_seconds(datetime);
	
	SNVS->LPSRTCMR = (uint32)(seconds >> 17); /* 设置高16位 */
	SNVS->LPSRTCLR = (uint32)(seconds << 15); /* 设置地16位 */

	/* 如果此前RTC是打开的在设置完RTC时间以后需要重新打开RTC */
	if (tmp & 0x1)
		rtc_enable();
}
//-------------------------------------------------------------------------------------------------------------------
//  函数简介      将秒数转换为时间
//  参数说明      seconds      要转换的秒数      
//  参数说明      *datetime     转换后的日期和时间      
//  返回参数      void         
//  备注信息
//-------------------------------------------------------------------------------------------------------------------
void rtc_convertseconds_to_datetime(uint64 seconds, struct rtc_datetime *datetime)
{
    uint64 x;
    uint64  secondsRemaining, days;
    uint16 daysInYear;

    /* 每个月的天数 */
    uint8 daysPerMonth[] = {0U, 31U, 28U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U};

    secondsRemaining = seconds;                             //剩余秒数初始化
    days = secondsRemaining / SECONDS_IN_A_DAY + 1; 		//根据秒数计算天数,加1是当前天数
    secondsRemaining = secondsRemaining % SECONDS_IN_A_DAY; //计算天数以后剩余的秒数 

	/* 计算时、分、秒 */
    datetime->rtc_hour = secondsRemaining / SECONDS_IN_A_HOUR;
    secondsRemaining = secondsRemaining % SECONDS_IN_A_HOUR;
    datetime->rtc_minute = secondsRemaining / 60;
    datetime->rtc_second = secondsRemaining % SECONDS_IN_A_MINUTE;

    /* 计算年 */
    daysInYear = DAYS_IN_A_YEAR;
    datetime->rtc_year = YEAR_RANGE_START;
    while(days > daysInYear)
    {
        /* 根据天数计算年 */
        days -= daysInYear;
        datetime->rtc_year++;

        /* 处理闰年 */
        if (!rtc_isleapyear(datetime->rtc_year))
            daysInYear = DAYS_IN_A_YEAR;
        else	//闰年，天数加一
            daysInYear = DAYS_IN_A_YEAR + 1;
    }
	/*根据剩余的天数计算月份 */
    if(rtc_isleapyear(datetime->rtc_year)) /* 如果是闰年的话2月加一天 */
        daysPerMonth[2] = 29;

    for(x = 1; x <= 12; x++)
    {
        if (days <= daysPerMonth[x])
        {
            datetime->rtc_month = x;
            break;
        }
        else
        {
            days -= daysPerMonth[x];
        }
    }

    datetime->rtc_day = days;

}

//-------------------------------------------------------------------------------------------------------------------
//  函数简介      获取RTC当前秒数
//  参数说明      void      
//  返回参数      uint32     当前秒数
//  备注信息
//-------------------------------------------------------------------------------------------------------------------
uint32 rtc_getseconds(void)
{
	uint32 seconds = 0;
	seconds = (SNVS->LPSRTCMR << 17) | (SNVS->LPSRTCLR >> 15);
	return seconds;
}
//-------------------------------------------------------------------------------------------------------------------
//  函数简介      获取当前时间
//  参数说明      *datetime     获取到的时间，日期等参数 
//  返回参数      void     当前秒数
//  备注信息
//-------------------------------------------------------------------------------------------------------------------
void rtc_getdatetime(struct rtc_datetime *datetime)
{
	uint64 seconds;
	seconds = rtc_getseconds();
	rtc_convertseconds_to_datetime(seconds, datetime);	
}
//-------------------------------------------------------------------------------------------------------------------
//  函数简介      更新当前时间
//  参数说明      *datetime     获取到的时间，日期等参数 
//  返回参数      void     当前秒数
//  备注信息
//-------------------------------------------------------------------------------------------------------------------
void rtc_update_time(uint16 year, uint16 month, uint16 day, uint16 hour, uint16 minute, uint16 second)
{    
	struct rtc_datetime rtc_date;
    rtc_date.rtc_year = year;
   	rtc_date.rtc_month = month;
    rtc_date.rtc_day = day;
    rtc_date.rtc_hour = hour;
    rtc_date.rtc_minute = minute;
    rtc_date.rtc_second = second;
	rtc_setdatetime(&rtc_date); /* 初始化时间和日期 */
}
