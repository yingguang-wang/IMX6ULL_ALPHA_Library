#include "device_tft5xx6.h"
#include "driver_gpio.h"
#include "driver_delay.h"
#include "driver_iic.h"
#include "driver_exit.h"
#include "printf.h"

struct ft5426_dev_config ft5426_dev;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     初始化TFT-LCD屏
// 参数说明     void             
// 返回参数     void           
// 备注信息     其实就是初始化FT5426
//-------------------------------------------------------------------------------------------------------------------
void ft5426_init(void)
{
	uint8 reg_value[2],i;
	ft5426_dev.initfalg = FT5426_INIT_NOTFINISHED;
	for( i = 0; i < 5; i++ )	// 避免编译器自动赋值
	{
		ft5426_dev.x[i] = 0;
		ft5426_dev.y[i] = 0;
	}
	ft5426_dev.point_num = 0;
    /* 初始化IIC2 IO*/
    iomuxc_set((iomuxc_value_t){IOMUXC_UART5_TX_DATA_I2C2_SCL}, 1, 0x70B0);
    iomuxc_set((iomuxc_value_t){IOMUXC_UART5_RX_DATA_I2C2_SDA}, 1, 0x70B0);
    /*初始化触摸屏中断IO和复位IO */ 
    iomuxc_set((iomuxc_value_t){IOMUXC_GPIO1_IO09_GPIO1_IO09}, 0, 0xF080);
    iomuxc_set((iomuxc_value_t){IOMUXC_SNVS_SNVS_TAMPER9_GPIO5_IO09}, 0, 0X10B0);

	// 中断IO初始化
    exti_init(P01_9,kGPIO_IntRisingOrFallingEdge);
	/* 复位IO初始化 */
    gpio_init(P05_9, GPO, 1);

	/* 初始化I2C */
	iic_init(I2C2, 100000);	

	/* 初始化FT5426 */
    gpio_low(P05_9);    // 复位FT5426
    system_delay_ms(20);
    gpio_high(P05_9);   // 停止复位FT5426
    system_delay_ms(20);

	ft5426_write_byte(FT5426_ADDR, FT5426_DEVICE_MODE, 0); 	/* 进入正常模式 				*/
	ft5426_write_byte(FT5426_ADDR, FT5426_IDG_MODE, 1); 	/* FT5426中断模式 			*/

	
	ft5426_read_len(FT5426_ADDR, FT5426_IDGLIB_VERSION, 2, reg_value);
	printf("Touch Frimware Version:%#X\r\n", ((uint16)reg_value[0] << 8) + reg_value[1]);
	
	ft5426_dev.initfalg = FT5426_INIT_FINISHED;	/* 标记FT5426初始化完成 */
	ft5426_dev.intflag = 0;
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     向FT5429写入数据
// 参数说明     addr        设备地址       
// 参数说明     reg         要写入的寄存器       
// 参数说明     data        要写入的数据       
// 返回参数     uint8       操作结果   
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 ft5426_write_byte(uint8 addr,uint8 reg, uint8 data)
{
    uint8 status=0;
    uint8 writedata=data;
    iic_transfer_t masterXfer;
	
    /* 配置I2C xfer结构体 */
   	masterXfer.slave_address = addr; 			// 设备地址
    masterXfer.direction = kI2C_Write;			// 写入数据 
    masterXfer.sub_address = reg;				// 要写入的寄存器地址
    masterXfer.sub_address_size = 1;			// 地址长度一个字节
    masterXfer.data = &writedata;				// 要写入的数据 
    masterXfer.data_size = 1;  					// 写入数据长度1个字节

    if(iic_master_transfer(I2C2, &masterXfer))
        status=1;
        
    return status;
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     从FT5426读取一个字节的数据
// 参数说明     addr        设备地址       
// 参数说明     reg         要读取的寄存器             
// 返回参数     uint8        读取到的数据
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 ft5426_read_byte(uint8 addr,uint8 reg)
{
	uint8 val=0;	
	iic_transfer_t masterXfer;	

	masterXfer.slave_address = addr;			// 设备地址 
    masterXfer.direction = kI2C_Read;			// 读取数据
    masterXfer.sub_address = reg;				// 要读取的寄存器地址
    masterXfer.sub_address_size = 1;			// 地址长度一个字节
    masterXfer.data = &val;						// 接收数据缓冲区
    masterXfer.data_size = 1;					// 读取数据长度1个字节
	iic_master_transfer(I2C2, &masterXfer);

	return val;
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     从FT5429读取多个字节的数据
// 参数说明     addr        设备地址       
// 参数说明     reg         要读取的开始寄存器地址             
// 参数说明     len         要读取的数据长度             
// 参数说明     buf         读取到的数据缓冲区             
// 返回参数     void        
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void ft5426_read_len(uint8 addr,uint8 reg,uint8 len,uint8 *buf)
{	
	iic_transfer_t masterXfer;	
	
	masterXfer.slave_address = addr;				// 设备地址 
    masterXfer.direction = kI2C_Read;			// 读取数据
    masterXfer.sub_address = reg;				// 要读取的寄存器地址
    masterXfer.sub_address_size = 1;				// 地址长度一个字节
    masterXfer.data = buf;						// 接收数据缓冲区 
    masterXfer.data_size = len;					// 读取数据长度1个字节

	iic_master_transfer(I2C2, &masterXfer);
} 
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     读取当前触摸点个数
// 参数说明     void                 
// 返回参数     void        
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void ft5426_read_tpnum(void)
{
	ft5426_dev.point_num = ft5426_read_byte(FT5426_ADDR, FT5426_TD_STATUS);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     读取当前所有触摸点的坐标
// 参数说明     void                 
// 返回参数     void        
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void ft5426_read_tpcoord(void)
{
	uint8 i = 0;
	uint8 type = 0;
	//uint8 id = 0;
	uint8 pointbuf[FT5426_XYCOORDREG_NUM];
	
	ft5426_dev.point_num = ft5426_read_byte(FT5426_ADDR, FT5426_TD_STATUS);

	/*
  	 * 从寄存器FT5426_TOUCH1_XH开始，连续读取30个寄存器的值，这30个寄存器
  	 * 保存着5个点的触摸值，每个点占用6个寄存器。
	 */
	ft5426_read_len(FT5426_ADDR, FT5426_TOUCH1_XH, FT5426_XYCOORDREG_NUM, pointbuf);
		
	for(i = 0; i < ft5426_dev.point_num ; i++)
	{
		uint8 *buf = &pointbuf[i * 6];
		/* 以第一个触摸点为例，寄存器TOUCH1_XH(地址0X03),各位描述如下：
		 * bit7:6  Event flag  0:按下 1:释放 2：接触 3：没有事件
		 * bit5:4  保留
		 * bit3:0  X轴触摸点的11~8位。
		 */
		ft5426_dev.x[i] = ((buf[2] << 8) | buf[3]) & 0x0fff;
        /* 以第一个触摸点为例，寄存器TOUCH1_YH(地址0X05),各位描述如下：
		 * bit7:4  Touch ID  触摸ID，表示是哪个触摸点
		 * bit3:0  Y轴触摸点的11~8位。
		 */
		ft5426_dev.y[i] = ((buf[0] << 8) | buf[1]) & 0x0fff;
		/* 获取触摸类型 */
		type = buf[0] >> 6;	
		
		//id = (buf[2] >> 4) & 0x0f;
		
		if(type == FT5426_TOUCH_EVENT_DOWN || type == FT5426_TOUCH_EVENT_ON )/* 按下 	*/
		{
		
		} else  
        {	/* 释放 */	
			
		}
	}	
}