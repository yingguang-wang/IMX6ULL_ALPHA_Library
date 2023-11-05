#ifndef __DEVICE_AP3216C__H__
#define __DEVICE_AP3216C__H__

#include "common_typedef.h"

#define AP3216C_ADDR    	0X1E	// AP3216C器件地址

/* AP3316C寄存器 */
#define AP3216C_SYSTEMCONG	0x00	// 配置寄存器
#define AP3216C_INTSTATUS	0X01	// 中断状态寄存器 
#define AP3216C_INTCLEAR	0X02	// 中断清除寄存器 
#define AP3216C_IRDATALOW	0x0A	// IR数据低字节 
#define AP3216C_IRDATAHIGH	0x0B	// IR数据高字节 
#define AP3216C_ALSDATALOW	0x0C	// ALS数据低字节
#define AP3216C_ALSDATAHIGH	0X0D	// ALS数据高字节
#define AP3216C_PSDATALOW	0X0E	// PS数据低字节
#define AP3216C_PSDATAHIGH	0X0F	// PS数据高字节 

//==================================================== AP3216C 基础函数=================================================
uint8   ap3216c_init(void);
uint8   ap3216c_read_one_byte(uint8 addr,uint8 reg);
uint8   ap3216c_write_one_byte(uint8 addr,uint8 reg, uint8 data);
void    ap3216c_read_data(uint16 *ir, uint16 *ps, uint16 *als);
//==================================================== AP3216C 基础函数=================================================

#endif  //!__DEVICE_AP3216C__H__