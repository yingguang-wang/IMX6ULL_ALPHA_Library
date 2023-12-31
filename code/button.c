#include "button.h"
#include "driver_delay.h"
//*********************************************************存储按键状态*********************************************
int key_last_status[5] = { 0 };     //按键上一次状态
int key_current_status[5] = { 0 };  //按键当前状态
//*********************************************************存储按键状态*********************************************


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     按键状态保存
// 参数说明     mode 按键扫描方式 1:扫描后归零 0:扫描后保持状态,手动归零(不推荐使用)
// 返回参数     void
// 备注信息     手动归零方式  key_current_status[i] = 0;
//-------------------------------------------------------------------------------------------------------------------
void Key_Scan_Save(uint8 mode)
{
    sint16 i= 0;
    key_scanner();
    if (key_get_state(KEY_1) == 2)
    {
        key_current_status[1] = 1;
    }

    for (i = 1; i < KEY_NUMBER+1; i++)
    {
        key_last_status[i] = key_current_status[i];
        if (mode)
        {
            key_current_status[i] = 0;
        }
    }
}
