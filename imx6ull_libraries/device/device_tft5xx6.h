#ifndef __DEVICE_TFT5XX6__H__
#define __DEVICE_TFT5XX6__H__

#include "common_typedef.h"
//================================================定义 FT5426 内部地址================================================
#define FT5426_ADDR				        0X38	// FT5426设备地址 

#define FT5426_DEVICE_MODE		        0X00 	// 模式寄存器 	 
#define FT5426_IDGLIB_VERSION	        0XA1 	// 固件版本寄存器 
#define FT5426_IDG_MODE			        0XA4	// 中断模式	
#define FT5426_TD_STATUS		        0X02	// 触摸状态寄存器 
#define FT5426_TOUCH1_XH		        0X03	// 触摸点坐标寄存器,一个触摸点用4个寄存器存储坐标数据
										 
#define FT5426_XYCOORDREG_NUM	        30		// 触摸点坐标寄存器数量 
#define FT5426_INIT_FINISHED	        1		// 触摸屏初始化完成
#define FT5426_INIT_NOTFINISHED	        0		// 触摸屏初始化未完成 

#define FT5426_TOUCH_EVENT_DOWN			0x00	// 按下
#define FT5426_TOUCH_EVENT_UP			0x01	// 释放 
#define FT5426_TOUCH_EVENT_ON			0x02	// 接触 
#define FT5426_TOUCH_EVENT_RESERVED		0x03	// 没有事件
//================================================定义 FT5426 内部地址================================================

//===============================================声明 FT5426 结构体==============================================
struct ft5426_dev_config
{	
	uint8 initfalg;		// 触摸屏初始化状态
	uint8 intflag;		// 标记中断有没有发生
	uint8 point_num;	// 触摸点
	uint16 x[5];		// X轴坐标 
	uint16 y[5];		// Y轴坐标

}ft5426_dev_config;

extern struct ft5426_dev_config ft5426_dev;
//===============================================声明 FT5426 结构体==============================================

//==================================================FT5426 基础函数==================================================
void ft5426_init            (void);

uint8 ft5426_write_byte     (uint8 addr,uint8 reg, uint8 data);
uint8 ft5426_read_byte      (uint8 addr,uint8 reg);
void ft5426_read_len        (uint8 addr,uint8 reg,uint8 len,uint8 *buf);
void ft5426_read_tpnum      (void);
void ft5426_read_tpcoord    (void);
//==================================================FT5426 基础函数==================================================

#endif  //!__DEVICE_TFT5XX6__H__