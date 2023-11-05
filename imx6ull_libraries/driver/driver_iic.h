#ifndef __DRIVER_IIC__H__
#define __DRIVER_IIC__H__

#include "common_typedef.h"
#include "MCIMX6Y2.h"

/* 相关宏定义 */
#define I2C_STATUS_OK				(0)     //正常状态
#define I2C_STATUS_BUSY				(1)     //繁忙状态
#define I2C_STATUS_IDLE				(2)     //空闲状态
#define I2C_STATUS_NAK				(3)     //没有接收到从机应答信号状态
#define I2C_STATUS_ARBITRATIONLOST	(4)     //仲裁丢失状态
#define I2C_STATUS_TIMEOUT			(5)     //等待超时
#define I2C_STATUS_ADDRNAK			(6)

static const uint16 s_iicDividerTable[] = 
{
    30,  32,  36,  42,  48,  52,  60,  72,  80,   88,   104,  128,  144,  160,  192,  240,
    288, 320, 384, 480, 576, 640, 768, 960, 1152, 1280, 1536, 1920, 2304, 2560, 3072, 3840,
    22,  24,  26,  28,  32,  36,  40,  44,  48,   56,   64,   72,   80,   96,   112,  128,
    160, 192, 224, 256, 320, 384, 448, 512, 640,  768,  896,  1024, 1280, 1536, 1792, 2048
};

typedef enum iic_direction
{
    kI2C_Write = 0x0, 		// 主机向从机写数据
    kI2C_Read = 0x1,  		// 主机从从机读数据
} iic_direction_t;

typedef struct iic_transfer
{
    uint8 slave_address;      	        // 7位从机地址
    iic_direction_t direction; 		    // 传输方向
    uint32 sub_address;       		    // 寄存器地址
    uint8 sub_address_size;    	        // 寄存器地址长度
    uint8 *volatile data;    	        // 数据缓冲区
    volatile uint32 data_size;  	    // 数据缓冲区长度
}iic_transfer_t;


//==================================================IIC 基础函数====================================================
void iic_init(I2C_Type *base, uint32 baudRate);
uint8 iic_master_start (I2C_Type *base, uint8 address,  iic_direction_t direction);
uint8 iic_master_repeated_start(I2C_Type *base, uint8 address, iic_direction_t direction);
uint8 iic_check_and_clear_error(I2C_Type *base, uint32 status);
uint8 iic_master_stop(I2C_Type *base);

void  iic_master_write_data(I2C_Type *base, const uint8 *buf, uint32 size);
void  iic_master_read_data(I2C_Type *base, uint8 *buf, uint32 size);

uint8 iic_master_transfer(I2C_Type *base, struct iic_transfer *xfer);
//==================================================IIC 基础函数====================================================


#endif  //!__DRIVER_IIC__H_