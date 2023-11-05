
#include "common_headfile.h"

int main(void)
{
	system_init();
	uart_init(UART1, 115200);	printf("print texting successing!\r\n");
	/* 初始化LED灯 */
	gpio_init(P01_3, GPO, 0);
	/* 初始化蜂鸣器*/
	gpio_init(P05_1, GPO , 1);
	Buzzer(100);
	/* 初始化tft_lcd */
	uint8 i = 0;
	tft_lcd_init();
	tft_lcd_set_color(RGB888_BROWN,TFTLCD_DEFAULT_BGCOLOR);
	tft_lcd_set_font(TFTLCD_DEFAULT_DISPLAY_FONT);
	tft_lcd_show_string(0,0,"TFT_LCD INIT SUCCESS!");
	system_delay_ms(500);
	/* 初始化TC时钟 */
	rtc_init();
	/* 初始化按键 */
	key_init(300);
	//test_rtc_clock();
	/* 初始化AP3216C */
	while(ap3216c_init())
	{
		tft_lcd_show_string(0, 32*3, "AP3216C Check Failed!");
		system_delay_ms(500);
		tft_lcd_show_string(0, 32*4, "Please Check!        ");
		system_delay_ms(500);
	}	


	//text_ap3216c();
	/* 初始化ICM20608 */
	icm20608_init();
	test_icm20608();
	/* 初始化TFT5XX6 */
	pit_ms_init(EPIT2, 500);
	gt9147_init();
	test_tft_gt9147();
	pit_ms_init(EPIT2, 500);
	exti_init(P01_18, kGPIO_IntFallingEdge);
	/*TFT-LCD背光测试*/
	pwm_init(PWM1, (iomuxc_value_t){IOMUXC_GPIO1_IO08_PWM1_OUT}, 3000, 10000);
	uint32 duty = 1000;
	tft_lcd_show_string(20,0,"TFT_LCD INIT SUCCESS");
	while(1)			
	{	
		Key_Scan_Save(1);
		if(key_last_status[1]==1)
		{
			duty +=100;
			if(duty > 10000)
			{
				duty = 1000;
			}
			pwm_set_duty(PWM1, duty);
			
		}
		tft_lcd_show_int(20, 24 * 1, (PWM1->PWMSAR & PWM_PWMSAR_SAMPLE_MASK),10);
		tft_lcd_show_int(20, 24 * 2, duty,10);
		tft_lcd_show_int(20, 24 * 3, (PWM1->PWMPR & PWM_PWMSAR_SAMPLE_MASK),10);
		

		/*lcd字体大小测试*/
		#if 0
		Key_Scan_Save(1);
		if(key_last_status[1]==1)
		{
			Buzzer(100);
			if(i == 0)
			{i++;tft_lcd_set_font(TFTLCD_12X12_FONT);}
			else if(i == 1){i++;tft_lcd_set_font(TFTLCD_16X16_FONT);}
			else if(i == 2){i++;tft_lcd_set_font(TFTLCD_24X24_FONT);}
			else if(i == 3){i=0;tft_lcd_set_font(TFTLCD_32X32_FONT);}
			tft_lcd_clear(TFTLCD_DEFAULT_BGCOLOR);
		}
		tft_lcd_show_string(20,0,"TFT_LCD INIT SUCCESS");
		#endif

	}

	return 0;
}

