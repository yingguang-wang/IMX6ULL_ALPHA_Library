#include "device_ap3216c.h"
#include "driver_delay.h"
#include "driver_gpio.h"
#include "driver_iic.h"

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     初始化AP3216C
// 参数说明     void       	
// 返回参数     uint8      0 成功，其他值 错误代码      
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 ap3216c_init(void)
{
	uint8 data = 0;

    iomuxc_set((iomuxc_value_t){IOMUXC_UART4_TX_DATA_I2C1_SCL}, 1, 0x70B0);
    iomuxc_set((iomuxc_value_t){IOMUXC_UART4_RX_DATA_I2C1_SDA}, 1, 0x70B0);

	iic_init(I2C1,100000);		// 初始化I2C1

	/* 初始化AP3216C */
	ap3216c_write_one_byte(AP3216C_ADDR, AP3216C_SYSTEMCONG, 0X04);	// 复位AP3216C
	system_delay_ms(50);											// AP33216C复位至少10ms 
	ap3216c_write_one_byte(AP3216C_ADDR, AP3216C_SYSTEMCONG, 0X03);	// 开启ALS、PS+IR 
	data = ap3216c_read_one_byte(AP3216C_ADDR, AP3216C_SYSTEMCONG);	// 读取刚刚写进去的0X03
	if(data == 0X03)
		return 0;	/* AP3216C正常 	*/
	else 
		return 1;	/* AP3216C失败 	*/
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     向AP3216C写入数据
// 参数说明     addr      设备地址
// 参数说明     reg       要写入的寄存器
// 参数说明     data      要写入的数据
// 返回参数     uint8     操作结果     
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 ap3216c_write_one_byte(uint8 addr,uint8 reg, uint8 data)
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

    if(iic_master_transfer(I2C1, &masterXfer))
        status=1;       
    return status;
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     从AP3216C读取一个字节的数据
// 参数说明     addr      设备地址
// 参数说明     reg       要读取的寄存器
// 返回参数     uint8     读取到的数据     
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 ap3216c_read_one_byte(uint8 addr,uint8 reg)
{
	uint8 val=0;
	
	iic_transfer_t masterXfer;	
	masterXfer.slave_address = addr;				// 设备地址
    masterXfer.direction = kI2C_Read;			// 读取数据
    masterXfer.sub_address = reg;				// 要读取的寄存器地址
    masterXfer.sub_address_size = 1;				// 地址长度一个字节 
    masterXfer.data = &val;						// 接收数据缓冲区
    masterXfer.data_size = 1;					// 读取数据长度1个字节

	iic_master_transfer(I2C1, &masterXfer);

	return val;
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     读取AP3216C的数据，读取原始数据，包括ALS,PS和IR, 注意！
// 参数说明     ir      ir数据
// 参数说明     ps      ps数据
// 参数说明     als     als数据
// 返回参数     void          
// 备注信息     如果同时打开ALS,IR+PS的话两次数据读取的时间间隔要大于112.5ms
//-------------------------------------------------------------------------------------------------------------------
void ap3216c_read_data(uint16 *ir, uint16 *ps, uint16 *als)
{
    uint8 buf[6];
    uint8 i;

	/* 循环读取所有传感器数据 */
    for(i = 0; i < 6; i++)	
    {
        buf[i] = ap3216c_read_one_byte(AP3216C_ADDR, AP3216C_IRDATALOW + i);	
    }
	
    if(buf[0] & 0X80) 	// IR_OF位为1,则数据无效
		*ir = 0;					
	else 				// 读取IR传感器的数据
		*ir = ((uint16)buf[1] << 2) | (buf[0] & 0X03); 			
	
	*als = ((uint16)buf[3] << 8) | buf[2];	// 读取ALS传感器的数据 
	
    if(buf[4] & 0x40)	// IR_OF位为1,则数据无效 
		*ps = 0;    													
	else 				// 读取PS传感器的数据
		*ps = ((uint16)(buf[5] & 0X3F) << 4) | (buf[4] & 0X0F); 	
}