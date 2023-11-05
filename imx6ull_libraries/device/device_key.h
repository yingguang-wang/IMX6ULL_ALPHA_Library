#ifndef _DEVICE_KEY_H_
#define _DEVICE_KEY_H_

#include "driver_gpio.h"

// 定义按键顺序对应下方 key_index_enum 枚举体中定义的顺序
// 如果新增按键 那么需要同步在下方 key_index_enum 枚举体中新增按键
#define KEY_LIST                      {P01_18}

#define KEY_RELEASE_LEVEL           (GPIO_HIGH)                                 // 按键的默认状态 也就是按键释放状态的电平
#define KEY_MAX_SHOCK_PERIOD        (10     )                               // 按键消抖检测时长 低于这个时长的信号会被认为是杂波抖动
#define KEY_LONG_PRESS_PERIOD       (2000     )                                // 最小长按时长

//=================================================定义 按键 参数结构体===============================================
typedef enum
{
    KEY_1,
    KEY_NUMBER,
}key_index_enum;                                                                // 按键索引 对应上方定义的按键引脚个数 默认定义四个按键

typedef enum
{
    KEY_RELEASE = 0,                                                                // 按键释放状态
    KEY_CHECK_SHOCK,                                                            // 按键消抖状态
    KEY_SHORT_PRESS,                                                            // 按键短按状态
    KEY_LONG_PRESS,                                                             // 按键长按状态
}key_state_enum;
//=================================================定义 按键 参数结构体===============================================

//====================================================按键 基础函数=================================================
void            key_scanner(void);                                         // 按键状态扫描
key_state_enum  key_get_state(key_index_enum key_n);                         // 获取按键状态
uint8           key_init(uint32 period);                                // 按键初始化
//====================================================按键 基础函数=================================================

#endif // !_DEVICE_KEY_H_
