#ifndef __DEVICE_TFT_GT9147__H__
#define __DEVICE_TFT_GT9147__H__

#include "common_typedef.h"

//================================================定义 GT9147 内部地址================================================
#define GT9147_ADDR				0X14	// GT9147设备地址 

#define GT_CTRL_REG 	        0X8040  // GT9147控制寄存器 
#define GT_MODSW_REG 	        0X804D  // GT9147模式切换寄存器 
#define GT_CFGS_REG 	        0X8047  // GT9147配置起始地址寄存器
#define GT1151_CFGS_REG 	    0X8050  // GT1151配置起始地址寄存器 
#define GT_CHECK_REG 	        0X80FF  // GT9147校验和寄存器
#define GT_PID_REG 		        0X8140  // GT9147产品ID寄存器 

#define GT_GSTID_REG 	        0X814E  // GT9147当前检测到的触摸情况 
#define GT_TP1_REG 		        0X8150  // 第一个触摸点数据地址 
#define GT_TP2_REG 		        0X8158	// 第二个触摸点数据地址
#define GT_TP3_REG 		        0X8160  // 第三个触摸点数据地址
#define GT_TP4_REG 		        0X8168  // 第四个触摸点数据地址
#define GT_TP5_REG 		        0X8170	// 第五个触摸点数据地址 
										 
#define GT9147_XYCOORDREG_NUM	30		// 触摸点坐标寄存器数量 
#define GT9147_INIT_FINISHED	1		// 触摸屏初始化完成 
#define GT9147_INIT_NOTFINISHED	0		// 触摸屏初始化未完成

//================================================定义 GT9147 内部地址================================================

//===============================================声明 GT9147 结构体==============================================
struct gt9147_dev_config
{	
	uint8 initfalg;		// 触摸屏初始化状态
	uint8 intflag;		// 标记中断有没有发生
	uint8 point_num;	// 触摸点 
	uint16 x[5];		// X轴坐标
	uint16 y[5];		// Y轴坐标
}gt9147_dev_config;

extern struct gt9147_dev_config gt9147_dev;
//===============================================声明 GT9147 结构体==============================================
extern int gt_init_fail;    // 标记GT9147初始化失败 
//==================================================GT9147 基础函数==================================================
void    gt9147_init         (void);

uint8   gt9147_write_byte   (uint8 addr,uint32 reg, uint8 data);
uint8   gt9147_read_byte    (uint8 addr,uint32 reg);
void    gt9147_read_len     (uint8 addr,uint32 reg, uint32 len, uint8 *buf);
void    gt9147_write_len    (uint8 addr,uint32 reg, uint32 len, uint8 *buf);
void    gt9147_read_tpnum   (void);
void    gt9147_read_tpcoord (void);
void    gt9147_send_cfg     (uint8 mode);
void    gt9147_read_tpcoord (void);
//==================================================GT9147 基础函数==================================================

#endif  //!__DEVICE_TFT_GT9147__H__