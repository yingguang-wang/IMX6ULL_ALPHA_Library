#include "test_device.h"
#include "common_headfile.h"

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TFT_GT9147测试
// 参数说明     void
// 返回参数     void          
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void test_tft_gt9147(void)
{
	tft_lcd_clear(TFTLCD_DEFAULT_BGCOLOR); //清屏

    tft_lcd_show_string(0, 24 * 0, "---------test_tft_gt9147--------");
	tft_lcd_show_string(0, 24 * 1, "TP Num	:");  
	tft_lcd_show_string(0, 24 * 2, "Point0 X:");  
	tft_lcd_show_string(0, 24 * 3, "Point0 Y:");  
	tft_lcd_show_string(0, 24 * 4, "Point1 X:");  
	tft_lcd_show_string(0, 24 * 5, "Point1 Y:");  
	tft_lcd_show_string(0, 24 * 6, "Point2 X:");  
	tft_lcd_show_string(0, 24 * 7, "Point2 Y:");  
	tft_lcd_show_string(0, 24 * 8, "Point3 X:");  
	tft_lcd_show_string(0, 24 * 9, "Point3 Y:");  
	tft_lcd_show_string(0, 24 * 10, "Point4 X:");  
	tft_lcd_show_string(0, 24 * 11, "Point4 Y:");  
	
	while(1)					
	{
		#if 1
		tft_lcd_show_uint(24 * 10, 24 * 1, gt9147_dev.point_num , 1);
		tft_lcd_show_uint(24 * 10, 24 * 2, gt9147_dev.x[0], 5);
		tft_lcd_show_uint(24 * 10, 24 * 3, gt9147_dev.y[0], 5);
		tft_lcd_show_uint(24 * 10, 24 * 4, gt9147_dev.x[1], 5);
		tft_lcd_show_uint(24 * 10, 24 * 5, gt9147_dev.y[1], 5);
		tft_lcd_show_uint(24 * 10, 24 * 6, gt9147_dev.x[2], 5);
		tft_lcd_show_uint(24 * 10, 24 * 7, gt9147_dev.y[2], 5);
		tft_lcd_show_uint(24 * 10, 24 * 8, gt9147_dev.x[3], 5);
		tft_lcd_show_uint(24 * 10, 24 * 9, gt9147_dev.y[3], 5);
		tft_lcd_show_uint(24 * 10, 24 * 10, gt9147_dev.x[4], 5);
		tft_lcd_show_uint(24 * 10, 24 * 11, gt9147_dev.y[4], 5);
		#endif
		tft_lcd_draw_bigpoint(gt9147_dev.x[0], gt9147_dev.y[0], RGB888_BLACK);
		Key_Scan_Save(1);
        if (key_last_status[1] == 1)
        {
            tft_lcd_clear(TFTLCD_DEFAULT_BGCOLOR);
            break;
        }
	}

}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TFT5XX6测试
// 参数说明     void
// 返回参数     void          
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void test_tft5xx6(void)
{
	tft_lcd_clear(TFTLCD_DEFAULT_BGCOLOR); //清屏

    tft_lcd_show_string(0, 24 * 0, "---------test_tft5xx6--------");
	tft_lcd_show_string(0, 24 * 1, "TP Num	:");  
	tft_lcd_show_string(0, 24 * 2, "Point0 X:");  
	tft_lcd_show_string(0, 24 * 3, "Point0 Y:");  
	tft_lcd_show_string(0, 24 * 4, "Point1 X:");  
	tft_lcd_show_string(0, 24 * 5, "Point1 Y:");  
	tft_lcd_show_string(0, 24 * 6, "Point2 X:");  
	tft_lcd_show_string(0, 24 * 7, "Point2 Y:");  
	tft_lcd_show_string(0, 24 * 8, "Point3 X:");  
	tft_lcd_show_string(0, 24 * 9, "Point3 Y:");  
	tft_lcd_show_string(0, 24 * 10, "Point4 X:");  
	tft_lcd_show_string(0, 24 * 11, "Point4 Y:");  
	
	while(1)					
	{
		tft_lcd_show_uint(24 * 10, 24 * 1, ft5426_dev.point_num , 1);
		tft_lcd_show_uint(24 * 10, 24 * 2, ft5426_dev.x[0], 5);
		tft_lcd_show_uint(24 * 10, 24 * 3, ft5426_dev.y[0], 5);
		tft_lcd_show_uint(24 * 10, 24 * 4, ft5426_dev.x[1], 5);
		tft_lcd_show_uint(24 * 10, 24 * 5, ft5426_dev.y[1], 5);
		tft_lcd_show_uint(24 * 10, 24 * 6, ft5426_dev.x[2], 5);
		tft_lcd_show_uint(24 * 10, 24 * 7, ft5426_dev.y[2], 5);
		tft_lcd_show_uint(24 * 10, 24 * 8, ft5426_dev.x[3], 5);
		tft_lcd_show_uint(24 * 10, 24 * 9, ft5426_dev.y[3], 5);
		tft_lcd_show_uint(24 * 10, 24 * 10, ft5426_dev.x[4], 5);
		tft_lcd_show_uint(24 * 10, 24 * 11, ft5426_dev.y[4], 5);
		Key_Scan_Save(1);
        if (key_last_status[1] == 1)
        {
            tft_lcd_clear(TFTLCD_DEFAULT_BGCOLOR);
            break;
        }
	}

}




//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ICM20608测试
// 参数说明     void
// 返回参数     void          
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void test_icm20608(void)
{

    tft_lcd_clear(TFTLCD_DEFAULT_BGCOLOR); //清屏
    tft_lcd_show_string(0, 24 * 0, "---------text_imx20608--------");

    while (1)
    {
        icm20608_get_acc();
        icm20608_get_gyro();

        //printf("\r\nicm20608 acc data:  x=%5d, y=%5d, z=%5d\r\n", icm20608_acc_x, icm20608_acc_y, icm20608_acc_z);
        //printf("\r\nicm20608 gyro data: x=%5d, y=%5d, z=%5d\r\n", icm20608_gyro_x, icm20608_gyro_y, icm20608_gyro_z);

        tft_lcd_show_string(0, 24 * 1, "icm20608_acc x y z");
        tft_lcd_show_int(0, 24 * 2, icm20608_acc_x, 4);
        tft_lcd_show_int(24*4, 24 * 2, icm20608_acc_y, 4);
        tft_lcd_show_int(24*8, 24 * 2, icm20608_acc_z, 4);

        tft_lcd_show_float(0, 24 * 3, icm20608_acc_transition(icm20608_acc_x), 3,2);
        tft_lcd_show_float(24*4, 24 * 3, icm20608_acc_transition(icm20608_acc_y), 3, 2);
        tft_lcd_show_float(24*8, 24 * 3, icm20608_acc_transition(icm20608_acc_z), 3,2);


        tft_lcd_show_string(0, 24 * 4, "icm20608_gyro x y z");
        tft_lcd_show_int(0, 24 * 5, icm20608_gyro_x, 5);
        tft_lcd_show_int(24*5, 24 * 5, icm20608_gyro_y, 5);
        tft_lcd_show_int(24*10, 24 * 5, icm20608_gyro_z, 5);

		tft_lcd_show_float(0, 24 * 6, icm20608_gyro_transition(icm20608_gyro_x), 3,2);
        tft_lcd_show_float(24*5, 24 * 6, icm20608_gyro_transition(icm20608_gyro_y), 3,2);
        tft_lcd_show_float(24*10, 24 * 6, icm20608_gyro_transition(icm20608_gyro_z), 3,2);
		tft_lcd_show_string(0, 24 * 7, "icm20608_temp:");
		tft_lcd_show_int(0, 24 * 8, icm20608_temp, 5);
		tft_lcd_show_float(24*5, 24 * 8, ((((float)(icm20608_temp) - 25 ) / 326.8 + 25)), 5,2);

		Key_Scan_Save(1);
        if (key_last_status[1] == 1)
        {
            tft_lcd_clear(TFTLCD_DEFAULT_BGCOLOR);
            break;
        }
	}
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     AP3216C测试
// 参数说明     void
// 返回参数     void          
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void text_ap3216c(void)
{
	uint16 ir, ps, als;

	tft_lcd_clear(TFTLCD_DEFAULT_BGCOLOR);	
	tft_lcd_show_string(0, 32*3, "AP3216C Ready!");  
    tft_lcd_show_string(0, 32*4, " IR:");	 
	tft_lcd_show_string(0, 32*5, " PS:");	
	tft_lcd_show_string(0, 32*6, "ALS:");	
	while(1)					
	{		
		ap3216c_read_data(&ir, &ps, &als);		// 读取数据
		tft_lcd_show_uint(64, 32*4, ir, 16);	// 显示IR数据 
        tft_lcd_show_uint(64, 32*5, ps, 16);	// 显示PS数据
        tft_lcd_show_uint(64, 32*6, als, 16);	// 显示ALS数据
		system_delay_ms(120);
		Key_Scan_Save(1);
		if(key_last_status[1]==1)
		{		
			return;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     RTC时钟测试
// 参数说明     void
// 返回参数     void          
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void test_rtc_clock(void)
{
	struct rtc_datetime rtc_date;
	uint8 x = 0, y = 3;
	char buf[160];
	memset(buf, 0, sizeof(buf));
    tft_lcd_clear(TFTLCD_DEFAULT_BGCOLOR);	
    tft_lcd_show_string(0,32*1,"TEST_RTC_CLOCK");	
    while(1)
	{
		Key_Scan_Save(1);
		if(key_last_status[1]==1)
		{
			rtc_update_time(2023,9,5,00,03,50);	
			tft_lcd_show_string(0,32*3,"change success");	
			system_delay_ms(500);			
			break;
		}
		x++;
		if(x == 90)
		{
			x=0;	y--;									
		}
		tft_lcd_show_uint(10,32*2,y,2);	
		if(y == 0)		break;	
		tft_lcd_show_string(42,32*2,"second later will be running");		
		system_delay_ms(10);
	}
	system_delay_ms(100);
	tft_lcd_clear(TFTLCD_DEFAULT_BGCOLOR);	
	while(1)			
	{	

		rtc_getdatetime(&rtc_date);
		tft_lcd_show_string(0,32*4,"current time:");	
		sprintf(buf,"%d/%d/%d %d:%d:%d",rtc_date.rtc_year, rtc_date.rtc_month, rtc_date.rtc_day, rtc_date.rtc_hour, rtc_date.rtc_minute, rtc_date.rtc_second);		
		tft_lcd_show_string(448, 32*4, buf);  /* 显示字符串 */
		Key_Scan_Save(1);
		if(key_last_status[1]==1)
		{		
			return;
		}
	}
}