#include "device_lcd.h"
#include "common_function.h"
#include "driver_gpio.h"
#include "driver_delay.h"
#include "common_front.h"
#include "fsl_iomuxc.h"
#include "string.h"

static uint32 tft_lcd_pencolor = TFTLCD_DEFAULT_PENCOLOR;
static uint32 tft_lcd_bgcolor = TFTLCD_DEFAULT_BGCOLOR;
static tft_lcd_font_size_enum    tft_lcd_display_font = TFTLCD_DEFAULT_DISPLAY_FONT;

struct tft_lcd_value tft_lcd_dev;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     初始化TFT-LCD屏
// 参数说明     void             
// 返回参数     void           
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void tft_lcd_init(void)
{
    tft_lcd_id_enum  tft_lcd_id;
	tft_lcd_id = tft_lcd_read_panelid();		/* 读取屏幕ID值 */
	tft_lcd_dev.id = tft_lcd_id;
	printf("LCD ID=%#X\r\n", tft_lcd_id);
	/* 初始化IO */
	tft_lcd_gpio_init();		
	/* 复位LCD */
	tft_lcd_reset();				
	system_delay_ms(10);
	/* 结束复位 */
	tft_lcd_no_reset();				


	/* TFTLCD参数结构体初始化 */
	if(tft_lcd_id == ATK4342) {
		tft_lcd_dev.height = 272;	
		tft_lcd_dev.width = 480;
		tft_lcd_dev.vspw = 1;
		tft_lcd_dev.vbpd = 8;
		tft_lcd_dev.vfpd = 8;
		tft_lcd_dev.hspw = 1;
		tft_lcd_dev.hbpd = 40;
		tft_lcd_dev.hfpd = 5; 	
		tft_lcd_clk_init(27, 8, 8);		//初始化LCD时钟 10.1MHz
	} else if(tft_lcd_id == ATK4384) {
		tft_lcd_dev.height = 480;	
		tft_lcd_dev.width = 800;
		tft_lcd_dev.vspw = 3;
		tft_lcd_dev.vbpd = 32;
		tft_lcd_dev.vfpd = 13;
		tft_lcd_dev.hspw = 48;
		tft_lcd_dev.hbpd = 88;
		tft_lcd_dev.hfpd = 40;
		tft_lcd_clk_init(42, 4, 8);		//初始化LCD时钟 31.5MHz
	} else if(tft_lcd_id == ATK7084) {
		tft_lcd_dev.height = 480;	
		tft_lcd_dev.width = 800;
		tft_lcd_dev.vspw = 1;
		tft_lcd_dev.vbpd = 23;
		tft_lcd_dev.vfpd = 22;
		tft_lcd_dev.hspw = 1;
		tft_lcd_dev.hbpd = 46;
		tft_lcd_dev.hfpd = 210;	
		tft_lcd_clk_init(30, 3, 7);		//初始化LCD时钟 34.2MHz
	} else if(tft_lcd_id == ATK7016) {
		tft_lcd_dev.height = 600;	
		tft_lcd_dev.width = 1024;
		tft_lcd_dev.vspw = 3;
		tft_lcd_dev.vbpd = 20;
		tft_lcd_dev.vfpd = 12;
		tft_lcd_dev.hspw = 20;
		tft_lcd_dev.hbpd = 140;
		tft_lcd_dev.hfpd = 160;
		tft_lcd_clk_init(32, 3, 5);		//初始化LCD时钟 51.2MHz
	} else if(tft_lcd_id == ATK1018) {
		tft_lcd_dev.height = 800;	
		tft_lcd_dev.width = 1280;
		tft_lcd_dev.vspw = 3;
		tft_lcd_dev.vbpd = 10;
		tft_lcd_dev.vfpd = 10;
		tft_lcd_dev.hspw = 10;
		tft_lcd_dev.hbpd = 80;
		tft_lcd_dev.hfpd = 70;
		tft_lcd_clk_init(35, 3, 5);		//初始化LCD时钟 56MHz
	} else if(tft_lcd_id == ATKVGA) {  
		tft_lcd_dev.height = 768;	
		tft_lcd_dev.width = 1366;
		tft_lcd_dev.vspw = 3;
		tft_lcd_dev.vbpd = 24;
		tft_lcd_dev.vfpd = 3;
		tft_lcd_dev.hspw = 143;
		tft_lcd_dev.hbpd = 213;
		tft_lcd_dev.hfpd = 70;
		tft_lcd_clk_init(32, 3, 3);		//初始化LCD时钟 85MHz 
	}

	tft_lcd_dev.pixsize = 4;				//ARGB8888模式，每个像素4字节
	tft_lcd_dev.framebuffer = LCD_FRAMEBUF_ADDR;	
	tft_lcd_dev.backcolor = tft_lcd_bgcolor;	//背景色为白色
	tft_lcd_dev.forecolor = tft_lcd_pencolor;	//前景色为黑色
     
	/* 初始化ELCDIF的CTRL寄存器
     * bit [31] 	0 : 停止复位
     * bit [19] 	1 : 旁路计数器模式
     * bit [17] 	1 : LCD工作在dotclk模式
     * bit [15:14] 	00 : 输入数据不交换
     * bit [13:12] 	00 : CSC不交换
     * bit [11:10] 	11 : 24位总线宽度
     * bit [9:8]   	11 : 24位数据宽度,也就是RGB888
     * bit [5]    	1  : elcdif工作在主模式
     * bit [1]    	 0  : 所有的24位均有效
	 */
	 LCDIF->CTRL |= (1 << 19) | (1 << 17) | (0 << 14) | (0 << 12) |
	 				(3 << 10) | (3 << 8) | (1 << 5) | (0 << 1);
	/*
     * 初始化ELCDIF的寄存器CTRL1
     * bit [19:16]  : 0X7 ARGB模式下，传输24位数据，A通道不用传输
	 */	
	 LCDIF->CTRL1 = 0X7 << 16; 

	 /*
      * 初始化ELCDIF的寄存器TRANSFER_COUNT寄存器
      * bit [31:16]  : 高度
      * bit [15:0]   : 宽度
	  */
	LCDIF->TRANSFER_COUNT  = (tft_lcd_dev.height << 16) | (tft_lcd_dev.width << 0);

	/*
     * 初始化ELCDIF的VDCTRL0寄存器
     * bit [29] 0 : VSYNC输出
     * bit [28] 1 : 使能ENABLE输出
     * bit [27] 0 : VSYNC低电平有效
     * bit [26] 0 : HSYNC低电平有效
     * bit [25] 0 : DOTCLK上升沿有效
     * bit [24] 1 : ENABLE信号高电平有效
     * bit [21] 1 : DOTCLK模式下设置为1
     * bit [20] 1 : DOTCLK模式下设置为1
     * bit [17:0] : vsw参数
	 */
	LCDIF->VDCTRL0 = 0;	//先清零
	if(tft_lcd_id == ATKVGA) {   //VGA需要特殊处理
		LCDIF->VDCTRL0 = (0 << 29) | (1 << 28) | (0 << 27) |
					 (0 << 26) | (1 << 25) | (0 << 24) |
					 (1 << 21) | (1 << 20) | (tft_lcd_dev.vspw << 0);
	} else {
		LCDIF->VDCTRL0 = (0 << 29) | (1 << 28) | (0 << 27) |
					 (0 << 26) | (0 << 25) | (1 << 24) |
					 (1 << 21) | (1 << 20) | (tft_lcd_dev.vspw << 0);
	}

	/*
	 * 初始化ELCDIF的VDCTRL1寄存器
	 * 设置VSYNC总周期
	 */  
	LCDIF->VDCTRL1 = tft_lcd_dev.height + tft_lcd_dev.vspw + tft_lcd_dev.vfpd + tft_lcd_dev.vbpd;  //VSYNC周期
	 
	 /*
	  * 初始化ELCDIF的VDCTRL2寄存器
	  * 设置HSYNC周期
	  * bit[31:18] ：hsw
	  * bit[17:0]  : HSYNC总周期
	  */ 
	LCDIF->VDCTRL2 = (tft_lcd_dev.hspw << 18) | (tft_lcd_dev.width + tft_lcd_dev.hspw + tft_lcd_dev.hfpd + tft_lcd_dev.hbpd);

	/*
	 * 初始化ELCDIF的VDCTRL3寄存器
	 * 设置HSYNC周期
	 * bit[27:16] ：水平等待时钟数
	 * bit[15:0]  : 垂直等待时钟数
	 */ 
	LCDIF->VDCTRL3 = ((tft_lcd_dev.hbpd + tft_lcd_dev.hspw) << 16) | (tft_lcd_dev.vbpd + tft_lcd_dev.vspw);

	/*
	 * 初始化ELCDIF的VDCTRL4寄存器
	 * 设置HSYNC周期
	 * bit[18] 1 : 当使用VSHYNC、HSYNC、DOTCLK的话此为置1
	 * bit[17:0]  : 宽度
	 */ 
	
	LCDIF->VDCTRL4 = (1<<18) | (tft_lcd_dev.width);

	/*
     * 初始化ELCDIF的CUR_BUF和NEXT_BUF寄存器
     * 设置当前显存地址和下一帧的显存地址
	 */
	LCDIF->CUR_BUF = (uint32)tft_lcd_dev.framebuffer;
	LCDIF->NEXT_BUF = (uint32)tft_lcd_dev.framebuffer;


	tft_lcd_enable();			/* 使能LCD 	*/
	system_delay_ms(10);
	tft_lcd_clear(RGB888_WHITE);	/* 清屏 		*/
	
}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     读取TFT-LCD屏幕ID，
// 参数说明     void             
// 返回参数     uint16  	屏幕ID         
// 备注信息     LCD_DATA23=R7(M0);LCD_DATA15=G7(M1);LCD_DATA07=B7(M2);
// 				M2:M1:M0
//				0 :0 :0	//4.3寸480*272 RGB屏,ID=0X4342
//				0 :0 :1	//7寸800*480 RGB屏,ID=0X7084
//	 			0 :1 :0	//7寸1024*600 RGB屏,ID=0X7016
//  			1 :0 :1	//10.1寸1280*800,RGB屏,ID=0X1018
//				1 :0 :0	//4.3寸800*480 RGB屏,ID=0X4384
//-------------------------------------------------------------------------------------------------------------------
tft_lcd_id_enum tft_lcd_read_panelid(void)
{
	uint8 idx = 0;

	/* 配置屏幕ID信号线 */
	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_VSYNC_GPIO3_IO03}, 0, 0X10B0);

	/* 打开模拟开关 */
	gpio_init(P03_3,GPO,1);

	/* 读取ID值，设置G7 B7 R7为输入 */
	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_DATA07_GPIO3_IO12}, 0, 0xF080);	//B7(M2)
	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_DATA15_GPIO3_IO20}, 0, 0xF080);	//G7(M1)
	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_DATA23_GPIO3_IO28}, 0, 0xF080);	//R7(M0)

	gpio_init(P03_12,GPI,1);
	gpio_init(P03_20,GPI,1);
	gpio_init(P03_28,GPI,1);

	idx = (uint8)gpio_get_level(P03_28); 		//读取M0
	idx |= (uint8)gpio_get_level(P03_20) << 1;	// 读取M1
	idx |= (uint8)gpio_get_level(P03_12) << 2;	// 读取M2 

	if(idx==0)return ATK4342;		//4.3寸屏,480*272分辨率
	else if(idx==1)return ATK7084;	//7寸屏,800*480分辨率
	else if(idx==2)return ATK7016;	//7寸屏,1024*600分辨率
	else if(idx==4)return ATK4384;	//4寸屏,800*480分辨率
	else if(idx==5)return ATK1018;	//10.1寸屏,1280*800分辨率		
	else if(idx==7)return ATKVGA;   //VGA模块，1366*768分辨率
	else return 0;
}

/*
 * IO引脚: 	LCD_DATA00 -> LCD_B0
 *			LCD_DATA01 -> LCD_B1
 *			LCD_DATA02 -> LCD_B2
 *			LCD_DATA03 -> LCD_B3
 *			LCD_DATA04 -> LCD_B4
 *			LCD_DATA05 -> LCD_B5
 *			LCD_DATA06 -> LCD_B6
 *			LCD_DATA07 -> LCD_B7
 *
 *			LCD_DATA08 -> LCD_G0
 *			LCD_DATA09 -> LCD_G1
 *			LCD_DATA010 -> LCD_G2
 *			LCD_DATA011 -> LCD_G3
 *			LCD_DATA012 -> LCD_G4
 *			LCD_DATA012 -> LCD_G4
 *			LCD_DATA013 -> LCD_G5
 *			LCD_DATA014 -> LCD_G6
 *			LCD_DATA015 -> LCD_G7
 *
 *			LCD_DATA016 -> LCD_R0
 *			LCD_DATA017 -> LCD_R1
 *			LCD_DATA018 -> LCD_R2 
 *			LCD_DATA019 -> LCD_R3
 *			LCD_DATA020 -> LCD_R4
 *			LCD_DATA021 -> LCD_R5
 *			LCD_DATA022 -> LCD_R6
 *			LCD_DATA023 -> LCD_R7
 *
 *			LCD_CLK -> LCD_CLK
 *			LCD_VSYNC -> LCD_VSYNC
 *			LCD_HSYNC -> LCD_HSYNC
 *			LCD_DE -> LCD_DE
 *			LCD_BL -> GPIO1_IO08 
 */
 
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TFT-LCD GPIO初始化屏幕
// 参数说明     void             
// 返回参数     void  	         
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void tft_lcd_gpio_init(void)
{

	/* IO初始化复用功能 */ 
	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_DATA00_LCDIF_DATA00}, 0, 0xB9);
	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_DATA01_LCDIF_DATA01}, 0, 0xB9);
	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_DATA02_LCDIF_DATA02}, 0, 0xB9);
	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_DATA03_LCDIF_DATA03}, 0, 0xB9);
	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_DATA04_LCDIF_DATA04}, 0, 0xB9);
	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_DATA05_LCDIF_DATA05}, 0, 0xB9);
	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_DATA06_LCDIF_DATA06}, 0, 0xB9);
	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_DATA07_LCDIF_DATA07}, 0, 0xB9);
	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_DATA08_LCDIF_DATA08}, 0, 0xB9);
	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_DATA09_LCDIF_DATA09}, 0, 0xB9);
	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_DATA10_LCDIF_DATA10}, 0, 0xB9);
	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_DATA11_LCDIF_DATA11}, 0, 0xB9);
	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_DATA12_LCDIF_DATA12}, 0, 0xB9);
	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_DATA13_LCDIF_DATA13}, 0, 0xB9);
	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_DATA14_LCDIF_DATA14}, 0, 0xB9);
	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_DATA15_LCDIF_DATA15}, 0, 0xB9);
	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_DATA16_LCDIF_DATA16}, 0, 0xB9);
	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_DATA17_LCDIF_DATA17}, 0, 0xB9);
	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_DATA18_LCDIF_DATA18}, 0, 0xB9);
	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_DATA19_LCDIF_DATA19}, 0, 0xB9);
	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_DATA20_LCDIF_DATA20}, 0, 0xB9);
	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_DATA21_LCDIF_DATA21}, 0, 0xB9);
	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_DATA22_LCDIF_DATA22}, 0, 0xB9);
	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_DATA23_LCDIF_DATA23}, 0, 0xB9);

	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_CLK_LCDIF_CLK}, 0, 0xB9);
	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_ENABLE_LCDIF_ENABLE}, 0, 0xB9);
	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_HSYNC_LCDIF_HSYNC}, 0, 0xB9);
	iomuxc_set((iomuxc_value_t){IOMUXC_LCD_VSYNC_LCDIF_VSYNC}, 0, 0xB9);

	iomuxc_set((iomuxc_value_t){IOMUXC_UART1_CTS_B_GPIO1_IO18}, 0, 0xB9);

	/* GPIO初始化 */
	gpio_init(P01_8, GPO, 1);						/* 背光默认打开 */
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     TFT-LCD 时钟初始化
// 参数说明     loopDiv   loopDivider值          
// 参数说明     prediv    lcdifprediv值     
// 参数说明     div       lcdifdiv值   
// 返回参数     void  	         
// 备注信息     LCD时钟计算公式:	LCD CLK = 24 * loopDiv / prediv / div
//-------------------------------------------------------------------------------------------------------------------
void tft_lcd_clk_init(uint8 loopDiv, uint8 prediv, uint8 div)
{
	/* 先初始化video pll 
     * VIDEO PLL = OSC24M * (loopDivider + (denominator / numerator)) / postDivider
 	 *不使用小数分频器，因此denominator和numerator设置为0
 	 */
	CCM_ANALOG->PLL_VIDEO_NUM = 0;		//不使用小数分频器
	CCM_ANALOG->PLL_VIDEO_DENOM = 0;	

	/*
     * PLL_VIDEO寄存器设置
     * bit[13]:    1   使能VIDEO PLL时钟
     * bit[20:19]  2  设置postDivider为1分频
     * bit[6:0] : 32  设置loopDivider寄存器
	 */
	CCM_ANALOG->PLL_VIDEO =  (2 << 19) | (1 << 13) | (loopDiv << 0); 

	/*
     * MISC2寄存器设置
     * bit[31:30]: 0  VIDEO的post-div设置，时钟源来源于postDivider，1分频
	 */
	CCM_ANALOG->MISC2 &= ~(3 << 30);
	CCM_ANALOG->MISC2 = 0 << 30;

	/* LCD时钟源来源与PLL5，也就是VIDEO  PLL  */
	CCM->CSCDR2 &= ~(7 << 15);  	
	CCM->CSCDR2 |= (2 << 15);			//设置LCDIF_PRE_CLK使用PLL5

	/* 设置LCDIF_PRE分频 */
	CCM->CSCDR2 &= ~(7 << 12);		
	CCM->CSCDR2 |= (prediv - 1) << 12;	//设置分频

	/* 设置LCDIF分频 */
	CCM->CBCMR &= ~(7 << 23);					
	CCM->CBCMR |= (div - 1) << 23;				

	/* 设置LCD时钟源为LCDIF_PRE时钟 */
	CCM->CSCDR2 &= ~(7 << 9);					//清除原来的设置
	CCM->CSCDR2 |= (0 << 9);					//LCDIF_PRE时钟源选择LCDIF_PRE时钟
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     复位ELCDIF接口
// 参数说明     void                   
// 返回参数     void  	         
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void tft_lcd_reset(void)
{
	LCDIF->CTRL  = 1<<31; //强制复位
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     结束复位ELCDIF接口
// 参数说明     void                   
// 返回参数     void  	         
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void tft_lcd_no_reset(void)
{
	LCDIF->CTRL  = 0<<31; //取消强制复位
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     使能ELCDIF接口
// 参数说明     void                   
// 返回参数     void  	         
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void tft_lcd_enable(void)
{
	LCDIF->CTRL |= 1<<0; 
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     读取指定点的颜色值
// 参数说明     x     	x轴坐标             
// 参数说明     y      	y轴坐标                         
// 返回参数     uint32  读取到的指定点的颜色值        
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
inline uint32 tft_lcd_readpoint(uint16 x,uint16 y)
{ 
	return *(uint32*)((uint32)tft_lcd_dev.framebuffer + 
		   tft_lcd_dev.pixsize * (tft_lcd_dev.width * y + x));
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     清屏
// 参数说明     uint32     	颜色值                                    
// 返回参数     void          
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void tft_lcd_clear(uint32 color)
{
	uint32 num;
	uint32 i = 0; 

	uint32 *startaddr=(uint32*)tft_lcd_dev.framebuffer;	//指向帧缓存首地址
	num=(uint32)tft_lcd_dev.width * tft_lcd_dev.height;			//缓冲区总长度
	for(i = 0; i < num; i++)
	{
		startaddr[i] = color;
	}		
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     以指定的颜色填充一块矩形
// 参数说明     x0     	矩形起始点坐标X轴                                    
// 参数说明     y0     	矩形起始点坐标Y轴                                    
// 参数说明     x1     	矩形终止点坐标X轴                                    
// 参数说明     y1     	矩形终止点坐标Y轴                                    
// 参数说明     color   要填充的颜色                                    
// 返回参数     void          
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void tft_lcd_fill(uint16 x0, uint16 y0, uint16 x1, uint16 y1, uint32 color)
{ 
    uint16 x, y;

	if(x0 < 0) x0 = 0;
	if(y0 < 0) y0 = 0;
	if(x1 >= tft_lcd_dev.width) x1 = tft_lcd_dev.width - 1;
	if(y1 >= tft_lcd_dev.height) y1 = tft_lcd_dev.height - 1;
	
    for(y = y0; y <= y1; y++)
    {
        for(x = x0; x <= x1; x++)
			tft_lcd_drawpoint(x, y, color);
    }
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     设置显示字体
// 参数说明     font        字体大小, 可选6/8/12/16/24/32       
// 返回参数     void
// 使用示例     ips200_set_font(IPS200_8x16_FONT);
// 备注信息     字体可以随时自由设置 设置后生效 后续显示就是新的字体大小
//-------------------------------------------------------------------------------------------------------------------
void tft_lcd_set_font (tft_lcd_font_size_enum font)
{
    tft_lcd_display_font = font;
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     设置显示颜色
// 参数说明     pen             颜色格式 RGB888或者可以使用 common_font.h枚举值或者自行写入
// 参数说明     bgcolor         颜色格式 RGB888 或者可以使用 common_font.h枚举值或者自行写入
// 返回参数     void
// 备注信息     字体颜色和背景颜色也可以随时自由设置 设置后生效
//-------------------------------------------------------------------------------------------------------------------
void tft_lcd_set_color (const uint32 pen, const uint32 bgcolor)
{
    tft_lcd_pencolor = pen;
    tft_lcd_bgcolor = bgcolor;
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     画点函数
// 参数说明     x      x轴坐标             
// 参数说明     y      y轴坐标             
// 参数说明     color  颜色值              
// 返回参数     void  	         
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
inline void tft_lcd_drawpoint(uint16 x,uint16 y,uint32 color)
{ 
  	*(uint32*)((uint32)tft_lcd_dev.framebuffer + 
		             tft_lcd_dev.pixsize * (tft_lcd_dev.width * y+x))=color;
}
void tft_lcd_draw_bigpoint(uint16 x,uint16 y,uint32 color)
{
	tft_lcd_drawpoint(x, y, color);
	if(x < tft_lcd_dev.width - 1)	tft_lcd_drawpoint(x + 1, y, color);
	if(x < tft_lcd_dev.height - 1)	tft_lcd_drawpoint(x, y + 1, color);
	if(( x < tft_lcd_dev.height - 1 )&&(x < tft_lcd_dev.width - 1))	tft_lcd_drawpoint(x + 1, y + 1, color);

}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     tft lcd 画线
// 参数说明     x_start         坐标x方向的起点 
// 参数说明     y_start         坐标y方向的起点 
// 参数说明     x_end           坐标x方向的终点 
// 参数说明     y_end           坐标y方向的终点 
// 参数说明     color           颜色格式 RGB888 或者可以使用 common_font.h枚举值或者自行写入
// 返回参数     void
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void tft_lcd_draw_line (uint16 x_start, uint16 y_start, uint16 x_end, uint16 y_end, const uint32 color)
{
    int16 x_dir = (x_start < x_end ? 1 : -1);
    int16 y_dir = (y_start < y_end ? 1 : -1);
    float temp_rate = 0;
    float temp_b = 0;

    do
    {
        if(x_start != x_end)
        {
            temp_rate = (float)(y_start - y_end) / (float)(x_start - x_end);
            temp_b = (float)y_start - (float)x_start * temp_rate;
        }
        else
        {
            while(y_start != y_end)
            {
                tft_lcd_drawpoint(x_start, y_start, color);
                y_start += y_dir;
            }
            break;
        }
        if(func_abs(y_start - y_end) > func_abs(x_start - x_end))
        {
            while(y_start != y_end)
            {
                tft_lcd_drawpoint(x_start, y_start, color);
                y_start += y_dir;
                x_start = (int16)(((float)y_start - temp_b) / temp_rate);
            }
        }
        else
        {
            while(x_start != x_end)
            {
                tft_lcd_drawpoint(x_start, y_start, color);
                x_start += x_dir;
                y_start = (int16)((float)x_start * temp_rate + temp_b);
            }
        }
    }while(0);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     tft lcd 画矩形
// 参数说明     x_start         坐标x方向的起点 
// 参数说明     y_start         坐标y方向的起点 
// 参数说明     x_end           坐标x方向的终点 
// 参数说明     y_end           坐标y方向的终点 
// 参数说明     color           颜色格式 RGB888 或者可以使用 common_font.h枚举值或者自行写入
// 返回参数     void
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void tft_lcd_draw_rectangle(uint16 x_start, uint16 y_start, uint16 x_end, uint16 y_end, const uint16 color)
{
	tft_lcd_draw_line(x_start, y_start, x_end, y_end, color);
	tft_lcd_draw_line(x_start, y_start, x_end, y_end, color);
	tft_lcd_draw_line(x_start, y_start, x_end, y_end, color);
	tft_lcd_draw_line(x_start, y_start, x_end, y_end, color);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     tft lcd 画圆形
// 参数说明     x0         圆心坐标X轴 
// 参数说明     y0         圆心坐标Y轴 
// 参数说明     r           圆形半径 
// 参数说明     color           颜色格式 RGB888 或者可以使用 common_font.h枚举值或者自行写入
// 返回参数     void
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void lcd_draw_Circle(uint16 x0,uint16 y0,uint8 r, const uint16 color)
{
    int mx = x0, my = y0;
    int x = 0, y = r;
    int d = 1 - r;   
    while(y > x)    	/* y>x即第一象限的第1区八分圆 */
    {
        tft_lcd_drawpoint(x  + mx, y  + my, color);
        tft_lcd_drawpoint(y  + mx, x  + my, color);
        tft_lcd_drawpoint(-x + mx, y  + my, color);
        tft_lcd_drawpoint(-y + mx, x  + my, color);

        tft_lcd_drawpoint(-x + mx, -y + my, color);
        tft_lcd_drawpoint(-y + mx, -x + my, color);
        tft_lcd_drawpoint(x  + mx, -y + my, color);
        tft_lcd_drawpoint(y  + mx, -x + my, color);
        if( d < 0)
        {
            d = d + 2 * x + 3;
        }
        else
        {
            d= d + 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     tft_lcd 显示字符
// 参数说明     x       坐标x方向的起点 
// 参数说明     y       坐标y方向的起点 
// 参数说明     dat     需要显示的字符
// 返回参数     void
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void tft_lcd_showchar(uint16 x, uint16 y, const char dat)
{  							  
    uint8  temp, t1, t;
	uint16 y0 = y;
	uint8 csize = (tft_lcd_display_font / 8+ ((tft_lcd_display_font % 8) ? 1 : 0)) * (tft_lcd_display_font / 2);	//得到字体一个字符对应点阵集所占的字节数

	for(t = 0; t < csize; t++)
	{   
        // 减 32 因为是取模是从空格开始取的 空格在 ascii 中序号是 32
		if(tft_lcd_display_font == 12) temp = ascii_font_12x12[dat-32][t]; 
		else if(tft_lcd_display_font == 16) temp = ascii_font_16x16[dat-32][t];	
		else if(tft_lcd_display_font == 24) temp = ascii_font_24x24[dat-32][t];	
		else if(tft_lcd_display_font == 32) temp = ascii_font_32x32[dat-32][t];	            
		else return;									        //没有的字库
		for(t1 = 0; t1 < 8; t1++)
		{			    
			if(temp & 0x80) tft_lcd_drawpoint(x, y, tft_lcd_pencolor);
			else            tft_lcd_drawpoint(x, y, tft_lcd_bgcolor);
			temp <<= 1;
			y++;
			if(y >= tft_lcd_dev.height) return;			//超区域了
			if((y - y0) == tft_lcd_display_font)
			{
				y = y0;
				x++;
				if(x >= tft_lcd_dev.width) return;		//超区域了
				break;
			}
		}  	 
	}  		    	   	 	  
} 
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     tft_lcd 显示字符串
// 参数说明     x       坐标x方向的起点 
// 参数说明     y       坐标y方向的起点 
// 参数说明     dat[]   需要显示的字符串
// 返回参数     void
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void tft_lcd_show_string(uint16 x, uint16 y, const char dat[])
{         
    uint16 j = 0;
    while((dat[j] >= ' ') && (dat[j]<= '~'))
    {
        switch(tft_lcd_display_font)
        {
            case TFTLCD_12X12_FONT:
                tft_lcd_showchar(x + 12 * j/2, y, dat[j]);
                j ++;
                break;
            case TFTLCD_16X16_FONT:
                tft_lcd_showchar(x + 16 * j/2, y, dat[j]);
                j ++;
                break;
            case TFTLCD_24X24_FONT:
            	tft_lcd_showchar(x + 24 * j/2, y, dat[j]);
                j ++;
                break;
            case TFTLCD_32X32_FONT:
                tft_lcd_showchar(x + 32 * j/2, y, dat[j]);
                j ++;
                break;
        }
    }
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     tft_lcd 显示32位有符号 (去除整数部分无效的0)
// 参数说明     x               坐标x方向的起点 
// 参数说明     y               坐标y方向的起点 
// 参数说明     dat             需要显示的变量 数据类型 int32
// 参数说明     num             需要显示的位数 最高10位  不包含正负号
// 返回参数     void
// 使用示例     tft_lcd_show_int(0, 0, x, 3);                    // x 可以为 int32 int16 int8 类型
// 备注信息     负数会显示一个 ‘-’号   正数显示一个空格
//-------------------------------------------------------------------------------------------------------------------
void tft_lcd_show_int (uint16 x, uint16 y, const int32 dat, uint8 num)
{
    int32 dat_temp = dat;
    int32 offset = 1;
    char data_buffer[12];

    memset(data_buffer, 0, 12);
    memset(data_buffer, ' ', num+1);

    if(num < 10)
    {
        for(; num > 0; num --)
        {
            offset *= 10;
        }
        dat_temp %= offset;
    }
    func_int_to_str(data_buffer, dat_temp);
    tft_lcd_show_string(x, y, (const char *)&data_buffer);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     tft_lcd 显示32位无符号 (去除整数部分无效的0)
// 参数说明     x               坐标x方向的起点
// 参数说明     y               坐标y方向的起点
// 参数说明     dat             需要显示的变量 数据类型 uint32
// 参数说明     num             需要显示的位数 最高10位  不包含正负号
// 返回参数     void
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void tft_lcd_show_uint (uint16 x, uint16 y, const uint32 dat, uint8 num)
{
    uint32 dat_temp = dat;
    int32 offset = 1;
    char data_buffer[12];

    memset(data_buffer, 0, 12);
    memset(data_buffer, ' ', num);

    if(num < 10)
    {
        for(; num > 0; num --)
        {
            offset *= 10;
        }
        dat_temp %= offset;
    }
    func_uint_to_str(data_buffer, dat_temp);
    tft_lcd_show_string(x, y, (const char *)&data_buffer);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介      tft_lcd 显示浮点数(去除整数部分无效的0)
// 参数说明     x               坐标x方向的起点
// 参数说明     y               坐标y方向的起点 
// 参数说明     dat             需要显示的变量，数据类型float或double
// 参数说明     num             整数位显示长度   最高8位
// 参数说明     pointnum        小数位显示长度   最高6位
// 返回参数     void
// 备注信息     特别注意当发现小数部分显示的值与你写入的值不一样的时候，
//              可能是由于浮点数精度丢失问题导致的，这并不是显示函数的问题，
//              有关问题的详情，请自行百度学习   浮点数精度丢失问题。
//              负数会显示一个 ‘-’号   正数显示一个空格
//-------------------------------------------------------------------------------------------------------------------
void  tft_lcd_show_float (uint16 x, uint16 y, float dat, uint8 num, uint8 pointnum)
{
    float dat_temp = dat;
    float offset = 1.0;
    char data_buffer[17];

    memset(data_buffer, 0, 17);
    memset(data_buffer, ' ', num + pointnum + 2);

    if (num < 10)
    {
        for (; num > 0; num--)
        {
            offset *= 10;
        }
        dat_temp = dat_temp - ((int)dat_temp / (int)offset) * offset;
    }

    func_float_to_str(data_buffer, dat_temp, pointnum);
    tft_lcd_show_string(x, y, data_buffer);
}



