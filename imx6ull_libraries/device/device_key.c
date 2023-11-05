
#include "device_key.h"
#include "driver_gpio.h"

static uint32               scanner_period = 0;                                 // 按键的扫描周期
static uint32               key_press_time[KEY_NUMBER];                         // 按键信号持续时长
static key_state_enum       key_state[KEY_NUMBER];                              // 按键状态

static const gpio_pin_enum  key_index[KEY_NUMBER] = KEY_LIST;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     按键状态扫描
// 参数说明     void
// 返回参数     void
// 使用示例     key_scanner();
// 备注信息     这个函数放在主循环或者 PIT 中断中
//-------------------------------------------------------------------------------------------------------------------
void key_scanner(void)
{
    uint8 i = 0;
    for (i = 0; i < KEY_NUMBER; i++)
    {
        switch (key_state[i])
        {
        case KEY_RELEASE:
            if (KEY_RELEASE_LEVEL != gpio_get_level(key_index[i]))
            {
                if (++key_press_time[i] >= KEY_MAX_SHOCK_PERIOD / scanner_period)
                {
                    key_state[i] = KEY_SHORT_PRESS;
                }
                else
                {
                    key_state[i] = KEY_CHECK_SHOCK;
                }
            }
            break;
        case KEY_CHECK_SHOCK:
            if (KEY_RELEASE_LEVEL != gpio_get_level(key_index[i]))
            {
                if (++key_press_time[i] >= KEY_MAX_SHOCK_PERIOD / scanner_period)
                {
                    key_state[i] = KEY_SHORT_PRESS;
                }
            }
            else
            {
                key_state[i] = KEY_RELEASE;
                key_press_time[i] = 0;
            }
            break;
        case KEY_SHORT_PRESS:
            if (++key_press_time[i] >= KEY_LONG_PRESS_PERIOD / scanner_period)
            {
                key_state[i] = KEY_LONG_PRESS;
            }
            if (KEY_RELEASE_LEVEL == gpio_get_level(key_index[i]))
            {
                key_state[i] = KEY_RELEASE;
                key_press_time[i] = 0;
            }
            break;
        case KEY_LONG_PRESS:
            if (KEY_RELEASE_LEVEL == gpio_get_level(key_index[i]))
            {
                key_state[i] = KEY_RELEASE;
                key_press_time[i] = 0;
            }
            break;

        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取按键状态
// 参数说明     key_n           按键索引
// 返回参数     key_state_enum  按键状态
// 使用示例     key_get_state(KEY_1);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
key_state_enum key_get_state(key_index_enum key_n)
{
    return key_state[key_n];
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     按键初始化
// 参数说明     period          按键扫描周期 以毫秒为单位
// 返回参数     void
// 使用示例     key_init(10);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8 key_init(uint32 period)
{
    uint8 loop_temp = 0;

    for (loop_temp = 0; loop_temp < KEY_NUMBER; loop_temp++)
    {
        gpio_init(key_index[loop_temp], GPI, GPIO_HIGH);
        key_state[loop_temp] = KEY_RELEASE;
    }
    scanner_period = period;
    return 0;
}
