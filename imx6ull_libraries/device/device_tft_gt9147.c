#include "device_tft_gt9147.h"
#include "driver_gpio.h"
#include "driver_delay.h"
#include "driver_iic.h"
#include "driver_exit.h"
#include "printf.h"


struct 	gt9147_dev_config gt9147_dev;
const 	uint16 GT9147_TPX_TBL[5]={GT_TP1_REG,GT_TP2_REG,GT_TP3_REG,GT_TP4_REG,GT_TP5_REG};
int 	gt_init_fail = 0;

/*
 * GT9147配置参数表
 * 第一个字节为版本号(0X61),必须保证新的版本号大于等于GT9147内部
 * flash原有版本号,才会更新配置.
 */
uint8 GT9147_CFG_TBL[]=
{ 
	0x41,0xe0,0x01,0x10,0x01,0x05,0x0d,0x00,0x01,0x08,
	0x28,0x05,0x50,0x32,0x03,0x05,0x00,0x00,0xff,0xff,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x89,0x28,0x0a,
	0x17,0x15,0x31,0x0d,0x00,0x00,0x02,0x9b,0x03,0x25,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x32,0x00,0x00,
	0x00,0x0f,0x94,0x94,0xc5,0x02,0x07,0x00,0x00,0x04,
	0x8d,0x13,0x00,0x5c,0x1e,0x00,0x3c,0x30,0x00,0x29,
	0x4c,0x00,0x1e,0x78,0x00,0x1e,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x08,0x0a,0x0c,0x0e,0x10,0x12,0x14,0x16,
	0x18,0x1a,0x00,0x00,0x00,0x00,0x1f,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0x00,0x02,0x04,0x05,0x06,0x08,0x0a,0x0c,
	0x0e,0x1d,0x1e,0x1f,0x20,0x22,0x24,0x28,0x29,0xff,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,
};
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     初始化触摸屏
// 参数说明     void    
// 返回参数     void   
// 备注信息     其实就是初始化GT9147
//-------------------------------------------------------------------------------------------------------------------
void gt9147_init(void)
{
    uint8 temp[7],i,gpio_init_status; 

	gt9147_dev.initfalg = GT9147_INIT_NOTFINISHED;
	/* 避免编译器自动赋值 */
	for( i = 0; i < 5; i++ )
	{	
		gt9147_dev.x[i] = 0;
		gt9147_dev.y[i] = 0;
	}
	gt9147_dev.point_num = 0;
	/* 初始化IIC2 IO*/
    iomuxc_set((iomuxc_value_t){IOMUXC_UART5_TX_DATA_I2C2_SCL}, 1, 0x70B0);
    iomuxc_set((iomuxc_value_t){IOMUXC_UART5_RX_DATA_I2C2_SDA}, 1, 0x70B0);
    /*初始化触摸屏中断IO和复位IO */ 
    iomuxc_set((iomuxc_value_t){IOMUXC_GPIO1_IO09_GPIO1_IO09}, 0, 0x10B0);
    iomuxc_set((iomuxc_value_t){IOMUXC_SNVS_SNVS_TAMPER9_GPIO5_IO09}, 0, 0X10B0);

	// 中断IO初始化
    //exti_init(P01_9,kGPIO_NoIntmode);
	gpio_init(P01_9, GPO, 1);
	/* 复位IO初始化 */
    gpio_init(P05_9, GPO, 1);

	/* 初始化I2C */
	iic_init(I2C2, 100000);	

	/* 初始化GT9147，要严格按照GT9147时序要求 */
    gpio_low(P05_9);    	// 复位FT5426
    system_delay_ms(10);
    gpio_high(P05_9);   	// 停止复位FT5426
    system_delay_ms(10);
	gpio_low(P01_9);    	// 拉低INT引脚
    system_delay_ms(100);

    gt9147_read_len(GT9147_ADDR, GT_PID_REG, 6, temp);			// 读取产品ID
	temp[6] = temp[4];
    temp[4] = 0;
	printf("CTP ID:%s\r\n", temp);	    /* 打印ID */
	printf("Default Ver:%#x\r\n",((temp[5]<<8) | temp[6]));   	// 打印固件版本

	/* 重新设置中断IO，配置为中断功能 */
    iomuxc_set((iomuxc_value_t){IOMUXC_GPIO1_IO09_GPIO1_IO09}, 0, 0x0080);

	if(temp[0] != 0x31)
	{
		temp[0]=gt9147_read_byte(GT9147_ADDR, 0x804D) & 0x3;     // 获取中断模式
		printf("9xx\r\n");
	}	
	else 
		temp[0]=gt9147_read_byte(GT9147_ADDR, 0x8056) & 0x3;
	switch(temp[0]) 
	{
		case 0x0:
			printf("InterruptMode:IntRisingEdge\r\n");
			gpio_init_status = kGPIO_IntRisingEdge;
			break;
		case 0x1:
			printf("InterruptMode:IntFallingEdge\r\n");
			gpio_init_status = kGPIO_IntFallingEdge;		
			break;
		case 0x2:
			printf("InterruptMode:IntLowLevel\r\n");
			gpio_init_status = kGPIO_IntLowLevel;		
			break;
		case 0x3:
			printf("InterruptMode:IntHighLevel\r\n");
			gpio_init_status = kGPIO_IntHighLevel;		
			break;
		default : printf("InterruptMode: Error\r\n");
			gpio_init_status = kGPIO_IntRisingOrFallingEdge;	
			break;
	}

	if(gt9147_write_byte(GT9147_ADDR, GT_CTRL_REG, 0x02))
	{
		gt_init_fail = 1;  //gt系列初始化失败
		printf("Error\r\n");
		goto done;
	}
	system_delay_ms(10);
	gt9147_write_byte(GT9147_ADDR, GT_CTRL_REG, 0);	
	exti_init(P01_9,gpio_init_status);
	system_delay_ms(10);
				
    gt9147_dev.initfalg = GT9147_INIT_FINISHED;	// 标记GT9147初始化完成 
	gt9147_dev.intflag = 0;
done:
	system_delay_ms(10);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     向GT9147写入数据
// 参数说明     addr        设备地址       
// 参数说明     reg         要写入的寄存器       
// 参数说明     data        要写入的数据       
// 返回参数     uint8       操作结果   
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 gt9147_write_byte(uint8 addr, uint32 reg, uint8 data)
{
    uint8 status=0;
    uint8 writedata=data;
    iic_transfer_t masterXfer;
	
    /* 配置I2C xfer结构体 */
   	masterXfer.slave_address = addr; 			// 设备地址 
    masterXfer.direction = kI2C_Write;			// 写入数据 
    masterXfer.sub_address = reg;				// 要写入的寄存器地址
    masterXfer.sub_address_size = 2;			// 地址长度一个字节 
    masterXfer.data = &writedata;				// 要写入的数据
    masterXfer.data_size = 1;  					// 写入数据长度1个字节

    if(iic_master_transfer(I2C2, &masterXfer))
        status=1;
        
    return status;
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     从GT9147读取一个字节的数据
// 参数说明     addr        设备地址       
// 参数说明     reg         要读取的寄存器             
// 返回参数     uint8        读取到的数据
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 gt9147_read_byte(uint8 addr,uint32 reg)
{
	uint8 val=0;	
	iic_transfer_t masterXfer;	

	masterXfer.slave_address = addr;			// 设备地址 
    masterXfer.direction = kI2C_Read;			// 读取数据
    masterXfer.sub_address = reg;				// 要读取的寄存器地址
    masterXfer.sub_address_size = 2;			// 地址长度一个字节
    masterXfer.data = &val;						// 接收数据缓冲区
    masterXfer.data_size = 1;					// 读取数据长度1个字节
	iic_master_transfer(I2C2, &masterXfer);

	return val;
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     从GT9147读取多个字节的数据
// 参数说明     addr        设备地址       
// 参数说明     reg         要读取的开始寄存器地址             
// 参数说明     len         要读取的数据长度             
// 参数说明     buf         读取到的数据缓冲区             
// 返回参数     void        
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void gt9147_read_len(uint8 addr,uint32 reg, uint32 len, uint8 *buf)
{	
	iic_transfer_t masterXfer;	
	
	masterXfer.slave_address = addr;			// 设备地址 
    masterXfer.direction = kI2C_Read;			// 读取数据
    masterXfer.sub_address = reg;				// 要读取的寄存器地址
    masterXfer.sub_address_size = 2;			// 地址长度一个字节
    masterXfer.data = buf;						// 接收数据缓冲区 
    masterXfer.data_size = len;					// 读取数据长度1个字节

	iic_master_transfer(I2C2, &masterXfer);
} 

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     向GT9147多个寄存器写入数据
// 参数说明     addr        设备地址       
// 参数说明     reg         要写入的开始寄存器地址             
// 参数说明     len         要写入的数据长度             
// 参数说明     *buf         写入到的数据缓冲区             
// 返回参数     void        
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void gt9147_write_len( uint8 addr, uint32 reg, uint32 len,  uint8 *buf)
{	
	iic_transfer_t masterXfer;	
	
	masterXfer.slave_address = addr;			// 设备地址 
    masterXfer.direction = kI2C_Write;			// 读取数据
    masterXfer.sub_address = reg;				// 要读取的寄存器地址
    masterXfer.sub_address_size = 2;			// 地址长度一个字节
    masterXfer.data = buf;						// 接收数据缓冲区 
    masterXfer.data_size = len;					// 读取数据长度1个字节

	iic_master_transfer(I2C2, &masterXfer);
} 
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     发送GT9147配置参数
// 参数说明     mode        0:参数不保存到flash  1:参数保存到flash                   
// 返回参数     void        
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void gt9147_send_cfg( uint8 mode)
{
	uint8 buf[2];
	uint32 i = 0;

	buf[0] = 0;
	buf[1] = mode;	// 是否写入到GT9147 FLASH?  即是否掉电保存 
	for(i = 0; i < (sizeof(GT9147_CFG_TBL)); i++) // 计算校验和 
        buf[0] += GT9147_CFG_TBL[i];            
    buf[0] = (~buf[0]) + 1;

    /* 发送寄存器配置 */
    gt9147_write_len(GT9147_ADDR, GT_CFGS_REG, sizeof(GT9147_CFG_TBL), GT9147_CFG_TBL);
    gt9147_write_len(GT9147_ADDR, GT_CHECK_REG, 2, buf);// 写入校验和,配置更新标记 
} 

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     读取当前所有触摸点的坐标
// 参数说明     void                
// 返回参数     void        
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void gt9147_read_tpcoord(void)
{
	uint8 buf[4];
	uint8 i = 0;
	uint8 regvalue = 0;

	regvalue = gt9147_read_byte(GT9147_ADDR, GT_GSTID_REG);
	gt9147_write_byte(GT9147_ADDR, GT_GSTID_REG, 0x00);
	gt9147_dev.point_num = regvalue & 0XF; // 计算读取了多少个点
	/* 读取当前所有的触摸坐标值 */
	for(i = 0; i < gt9147_dev.point_num; i++)
	{
		gt9147_read_len(GT9147_ADDR, GT9147_TPX_TBL[i], 4, buf);	// 读取坐标值
		gt9147_dev.x[i] = ((u16)buf[1] << 8) + buf[0];
		gt9147_dev.y[i] = (((u16)buf[3] << 8) + buf[2]);				
	} 
}
