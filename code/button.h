#ifndef _BUTTON_H
#define _BUTTON_H

#include "driver_gpio.h"
#include "device_key.h"


//*********************************************************存储按键状态*********************************************
extern int key_last_status[5];//按键上一次状态
extern int key_current_status[5];//按键当前状态
//*********************************************************存储按键状态*********************************************
// 
//*********************************************************保存按键键值*********************************************
extern void Key_Scan_Save(uint8 mode);
//*********************************************************保存按键键值*********************************************



#endif
