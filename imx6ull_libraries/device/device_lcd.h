#ifndef _DRIVER_LCD_H_
#define _DRIVER_LCD_H_

#include "common_front.h"

/* LCD显存地址 */
#define LCD_FRAMEBUF_ADDR	(0x89000000)
typedef enum
{
    ATK4342     =       0X4342,     //4.3寸480*272 
    ATK4384     =       0X4384,     //4.3寸800*480 
    ATK7084     =       0X7084,     //7寸800*480
    ATK7016     =       0X7016,     //7寸1024*600
    ATK1018     =       0X1018,     //10.1寸1280*800
    ATKVGA      =       0xff00,     //VGA
}tft_lcd_id_enum;

/* LCD控制参数结构体 */
struct tft_lcd_value
{
	uint16 height;		    // LCD屏幕高度 
	uint16 width;		    // LCD屏幕宽度 
	uint8 pixsize;		    // LCD每个像素所占字节大小 
	uint16 vspw;            // VSYNC 信号宽度，也就是 VSYNC 信号持续时间，单位为 1 行的时间
	uint16 vbpd;            // 帧同步信号后肩
	uint16 vfpd;            // 帧同步信号前肩
	uint16 hspw;            // HSYNC 信号宽度
	uint16 hbpd;            // 同步信号后肩
	uint16 hfpd;            // 行同步信号前肩
	uint32 framebuffer; 	// LCD显存首地址 
	uint32 forecolor;		// 前景色
	uint32 backcolor;		// 背景色
	uint32 id;  			// 屏幕ID
};

typedef enum
{
    TFTLCD_12X12_FONT                   = 12,                                    // 12x12    字体 
    TFTLCD_16X16_FONT                   = 16,                                    // 16x16    字体 
    TFTLCD_24X24_FONT                   = 24,                                    // 24x24    字体 
    TFTLCD_32X32_FONT                   = 32,                                    // 32x32    字体 

}tft_lcd_font_size_enum;

#define TFTLCD_DEFAULT_PENCOLOR         (RGB888_DARKSEAGREEN    	)                        // 默认的画笔颜色
#define TFTLCD_DEFAULT_BGCOLOR          (RGB888_DARKSEAGREEN  		)                        // 默认的背景颜色
#define TFTLCD_DEFAULT_DISPLAY_FONT     (TFTLCD_24X24_FONT	)                      // 默认的字体模式
//===================================================TFTLCD 基础函数==================================================

inline void tft_lcd_drawpoint           (uint16 x,uint16 y,uint32 color);
inline uint32 tft_lcd_readpoint         (uint16 x,uint16 y);
void tft_lcd_clear                      (uint32 color);
void tft_lcd_fill                       (uint16 x0, uint16 y0, uint16 x1, uint16 y1, uint32 color);

void    tft_lcd_set_font                 (tft_lcd_font_size_enum font);
void    tft_lcd_set_color                (const uint32 pen, const uint32 bgcolor);
inline void tft_lcd_drawpoint            (uint16 x,uint16 y,uint32 color);
void tft_lcd_draw_bigpoint				 (uint16 x,uint16 y,uint32 color);
void    tft_lcd_draw_line                (uint16 x_start, uint16 y_start, uint16 x_end, uint16 y_end, const uint32 color);

void    tft_lcd_show_char                (uint16 x, uint16 y, const char dat);
void    tft_lcd_show_string              (uint16 x, uint16 y, const char dat[]);
void    tft_lcd_show_int                 (uint16 x, uint16 y, const int32 dat, uint8 num);
void    tft_lcd_show_uint                (uint16 x, uint16 y, const uint32 dat, uint8 num);
void    tft_lcd_show_float               (uint16 x, uint16 y, float dat, uint8 num, uint8 pointnum);

void    tft_lcd_init(void);
tft_lcd_id_enum  tft_lcd_read_panelid(void);
void    tft_lcd_gpio_init(void);
void    tft_lcd_clk_init(uint8 loopDiv, uint8 prediv, uint8 div);
void    tft_lcd_reset(void);
void    tft_lcd_no_reset(void);
void    tft_lcd_enable(void);
//===================================================TFTLCD 基础函数==================================================

#endif // !_DRIVER_LCD_H_