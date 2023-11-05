#ifndef _COMMON_FRONT_H_
#define _COMMON_FRONT_H_

#include "common_typedef.h"
//-------常用颜色----------
typedef enum
{
    RGB565_WHITE = (0xFFFF),                                                        // 白色
    RGB565_BLACK = (0x0000),                                                        // 黑色
    RGB565_BLUE = (0x001F),                                                         // 蓝色
    RGB565_PURPLE = (0xF81F),                                                       // 紫色
    RGB565_PINK = (0xFE19),                                                         // 粉色
    RGB565_RED = (0xF800),                                                          // 红色
    RGB565_MAGENTA = (0xF81F),                                                      // 品红
    RGB565_GREEN = (0x07E0),                                                        // 绿色
    RGB565_CYAN = (0x07FF),                                                         // 青色
    RGB565_YELLOW = (0xFFE0),                                                       // 黄色
    RGB565_BROWN = (0xBC40),                                                        // 棕色
    RGB565_GRAY = (0x8430),                                                         // 灰色

    RGB565_39C5BB = (0x3616),
    RGB565_66CCFF = (0x665F),
}rgb565_color_enum;
//采用取色链接:https://blog.csdn.net/neo_Wordsworth/article/details/120622519
typedef enum 
{
    RGB888_WHITE =      (0x00FFFFFF),                                           // 白色
    RGB888_BLACK =      (0x00000000),                                           // 黑色
    RGB888_BLUE =       (0x000000FF),                                           // 蓝色
    RGB888_PURPLE =     (0x00800080),                                           // 紫色
    RGB888_PINK =       (0x00FFC0CB),                                           // 粉色
    RGB888_RED =        (0x00FF0000),                                           // 红色
    RGB888_MAGENTA =    (0x00FF00FF),                                           // 品红
    RGB888_GREEN =      (0x0000FF00),                                           // 绿色
    RGB888_CYAN =       (0x0000FFFF),                                           // 青色
    RGB888_YELLOW =     (0x00FFFF00),                                           // 黄色
    RGB888_BROWN =      (0x00A52A2A),                                           // 棕色
    RGB888_GRAY =       (0x00808080),                                           // 灰色    
}rgb888_color_enum;

typedef enum 
{
    RGB888_DIMGRAY =        (0x00696B4D),   // 昏灰
    RGB888_DARKGRAY =       (0x00A9A9A9),   // 暗灰
    RGB888_SILVER =         (0x00C0C0C0),   // 银色
    RGB888_LIGHTGRAY =      (0x00D3D3D3),   // 亮灰色
    RGB888_GAINSBORO =      (0x00DCDCDC),   // 庚斯博罗灰
    RGB888_WHITESMOKE =     (0x00F5F5F5),   // 白烟色
    RGB888_SNOW =           (0x00FFFAFA),   // 雪色
    RGB888_IRONGRAY =       (0x00625B57),   // 铁灰色
}rgb888_color_black_enum;

typedef enum {
    RGB888_MAROON =         (0x00800000),   // 栗色
    RGB888_DARKRED =        (0x008B0000),   // 暗红
    RGB888_STRONGRED =      (0x00E60000),   // 鲜红
    RGB888_PERSIMMON =      (0x00FF4D40),   // 柿子橙
    RGB888_MISTYROSE =      (0x00FFE4E1),   // 雾玫瑰色
    RGB888_SALMON =         (0x00FA8072),   // 鲑红
    RGB888_SCARLET =        (0x00FF2400),   // 腥红
    RGB888_TOMATO =         (0x00FF6347),   // 蕃茄红
    RGB888_DARKSALMON =     (0x00E9967A),   // 暗鲑红
    RGB888_CORAL =          (0x00FF7F50),   // 珊瑚红
    RGB888_ORANGERED =      (0x00FF4500),   // 橙红
    RGB888_LIGHTSALMON =    (0x00FFA07A),   // 亮鲑红
    RGB888_VERMILION =      (0x00FF4D00)    // 朱红
} rgb888_color_red_enum;

typedef enum {
    RGB888_GOLD =           (0x00FFD700),   // 金色
    RGB888_LIGHTGOLDENROD = (0x00FAFAD2),   // 亮金菊黄
    RGB888_LEMONCHIFFON =   (0x00FFFACD),   // 柠檬绸黄
    RGB888_PALEGOLDENROD =  (0x00EEE8AA),   // 苍麒麟黄
    RGB888_MELLOWYELLOW =   (0x00F8DE7E),   // 成熟的黄色
    RGB888_YELLOWGREEN =    (0x009ACD32),   // 黄绿色
    RGB888_CITRON =         (0x00B5E61D),   // 青柠黄
    RGB888_CANARYYELLOW =   (0x00FFFF99),   // 金丝雀黄
    RGB888_ORANGE =         (0x00FFA500),   // 橙色
    RGB888_DARKORANGE =     (0x00FF8C00),   // 暗橙色
    RGB888_TANGERINE =      (0x00FFA07A),   // 橙红色
} rgb888_color_yellow_enum;

typedef enum {
    RGB888_FOLIAGEGREEN =    (0x0073B839),   // 叶绿
    RGB888_FRESHLEAVES =     (0x0099FF4D),   // 嫩绿
    RGB888_BRIGHTGREEN =     (0x0066FF00),   // 明绿
    RGB888_COBALTGREEN =     (0x0066FF59),   // 钴绿
    RGB888_HONEYDEW =        (0x00F0FFF0),   // 蜜瓜绿
    RGB888_DARKSEAGREEN =    (0x008FBC8F),   // 暗海绿
    RGB888_LIGHTGREEN =      (0x0090EE90),   // 亮绿
    RGB888_PALEGREEN =       (0x0098FB98),   // 灰绿
    RGB888_IVYGREEN =        (0x0036BF36),   // 常春藤绿
    RGB888_FORESTGREEN =     (0x00228B22),   // 森林绿
    RGB888_LIMEGREEN =       (0x0032CD32),   // 柠檬绿
    RGB888_DARKGREEN =       (0x00006400),   // 暗绿
    RGB888_LIME =            (0x0000FF00),   // 鲜绿色
    RGB888_MALACHITE =       (0x0022C32E),   // 孔雀石绿
    RGB888_MINT =            (0x0016982B),   // 薄荷绿
    RGB888_CELADONGREEN =    (0x0073E68C)    // 青瓷绿
} rgb888_color_green_enum;

typedef enum {
    RGB888_MEDIUMBLUE =         (0x000000CD),   // 中蓝
    RGB888_DARKBLUE =           (0x0000008B),   // 暗蓝
    RGB888_NAVYBLUE =           (0x00000080),   // 藏青
    RGB888_MIDNIGHTBLUE =       (0x00191970),   // 午夜蓝
    RGB888_PERIWINKLE =         (0x00CCCCFF),   // 长春花色
    RGB888_LAVENDER =           (0x00E6E6FA),   // 薰衣草紫
    RGB888_GHOSTWHITE =         (0x00F8F8FF),   // 幽灵白
    RGB888_LAPISLAZULI =        (0x000D33FF),   // 天青石蓝
    RGB888_ULTRAMARINE =        (0x000033FF),   // 极浓海蓝
    RGB888_DARKMINERALBLUE =    (0x0024367D),   // 暗矿蓝
    RGB888_ROYALBLUE =          (0x004169E1),   // 品蓝
    RGB888_CERULEANBLUE =       (0x002A52BE),   // 蔚蓝
    RGB888_INTERNATIONALKLEIN = (0x00002FA7),   // 国际奇连蓝
    RGB888_SAPPHIRE =           (0x00082567),   // 蓝宝石色
    RGB888_DARKPOWDERBLUE =     (0x00003399),   // 暗婴儿粉蓝
    RGB888_SALVIABLUE =         (0x004D80E6),   // 鼠尾草蓝
    RGB888_CORNFLOWERBLUE =     (0x006495ED)    // 矢车菊蓝
} rgb888_color_blue_enum;

extern const uint8 ascii_font_12x12[95][12];
extern const uint8 ascii_font_16x16[95][16];
extern const uint8 ascii_font_24x24[95][36];
extern const uint8 ascii_font_32x32[95][128]; 

extern const uint8 ascii_font_8x16[][16];
extern const uint8 ascii_font_6x8[][6];



#endif // !_COMMON_FRONT_H_
