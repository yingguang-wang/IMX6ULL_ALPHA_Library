#include "driver_spi.h"
#include "printf.h"


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 软件复位
// 参数说明     *base    要使用的SPI，(ECSPI1, ECSPI2, ECSP3, ECSPI4)  	
// 参数说明     void     	
// 返回参数     void          
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
static inline void ECSPI_SoftwareReset(ECSPI_Type *base)
{
    /* Disables the block and resets the internal logic with the exception of the ECSPI control register */
    base->CONREG &= ~ECSPI_CONREG_EN_MASK;
    /* Software reset can not reset the control register, so clear the control register manually */
    base->CONREG = 0x0U;
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 波特率计算
// 参数说明     *base          要使用的SPI，(ECSPI1, ECSPI2, ECSP3, ECSPI4)  	
// 参数说明     baudRate_Bps   波特率  	
// 参数说明     srcClock_Hz    时钟源  	
// 参数说明     void     	
// 返回参数     void          
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void ECSPI_SetBaudRate(ECSPI_Type *base, uint32_t baudRate_Bps, uint32_t srcClock_Hz)
{
    uint8_t bestPreDividerValue = 0U, preDividerValue = 0U;
    uint8_t bestPostDividerValue = 0U, postDividerValue = 0U;
    uint32_t realBaudrate = 0U;
    uint32_t diff = 0xFFFFFFFFU;
    uint32_t min_diff = 0xFFFFFFFFU;

    for (preDividerValue = 0; (preDividerValue < 16) && diff; preDividerValue++)
    {
        for (postDividerValue = 0; (postDividerValue < 16) && diff; postDividerValue++)
        {
            realBaudrate = (srcClock_Hz / (preDividerValue + 1)) >> postDividerValue;
            if (realBaudrate > baudRate_Bps)
            {
                diff = realBaudrate - baudRate_Bps;
                if (diff < min_diff)
                {
                    min_diff = diff;
                    bestPreDividerValue = preDividerValue;
                    bestPostDividerValue = postDividerValue;
                }
            }
            else
            {
                diff = baudRate_Bps - realBaudrate;
                if (diff < min_diff)
                {
                    min_diff = diff;
                    bestPreDividerValue = preDividerValue;
                    bestPostDividerValue = postDividerValue;
                }
            }
        }
    }

    base->CONREG |= ECSPI_CONREG_PRE_DIVIDER(bestPreDividerValue) | ECSPI_CONREG_POST_DIVIDER(bestPostDividerValue);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 通道获取
// 参数说明     *base            要使用的SPI，(ECSPI1, ECSPI2, ECSP3, ECSPI4)  	
// 返回参数     uint8          
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
spi_channel_enum spi_get_channel(ECSPI_Type *base)
{
    if     (base == ECSPI1)  return 0;
    else if(base == ECSPI2)  return 0;
    else if(base == ECSPI3)  return 0;
    else                          return 0;
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 接口 8bit数据传输 发送与接收数据是同时进行的
// 参数说明     *base           要使用的SPI，(ECSPI1, ECSPI2, ECSP3, ECSPI4)  	
// 参数说明     data            数据
// 返回参数     void          
// 备注信息     软件拉高CS片选信号，使用函数前需拉低CS，使用后需拉高CS
//-------------------------------------------------------------------------------------------------------------------
uint8 spi_transfer_8bit(ECSPI_Type *base, const uint8 data)
{
	base->CONREG &= ~(3 << 18);
	base->CONREG |= (spi_get_channel(base) << 18);

  	while((base->STATREG & (1 << 0)) == 0){}                      // 等待发送FIFO为空
		base->TXDATA = (uint32)data;
	while((base->STATREG & (1 << 3)) == 0){}                     // 等待接收FIFO有数据
		return (uint8)base->RXDATA;
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介       SPI 8bit 数组传输 发送与接收数据是同时进行的
// 参数说明       *base           要使用的SPI，(ECSPI1, ECSPI2, ECSP3, ECSPI4) 
// 参数说明       write_buffer    发送的数据缓冲区地址
// 参数说明       read_buffer     发送数据时接收到的数据的存储地址(不需要接收则传 NULL)
// 参数说明       len             缓冲区长度
// 返回参数       void
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void spi_transfer_8bit_array (ECSPI_Type *base, const uint8 *write_buffer, uint8 *read_buffer, uint32 len)
{
    base->CONREG &= ~(3 << 18);
	base->CONREG |= (spi_get_channel(base) << 18);

    do
    {
        
        if(write_buffer != NULL)
        {
            while((base->STATREG & (1 << 0)) == 0){}                   // 等待发送FIFO为空
                base->TXDATA = (uint32)*write_buffer++;            // 将发送的数据写入缓冲区
        }
        else
        {
            while((base->STATREG & (1 << 0)) == 0){}                   // 等待发送FIFO为空
                base->TXDATA = (uint32)0xff;            // 将发送的数据写入缓冲区
        }
        if(read_buffer != NULL)
        {
            while((base->STATREG & (1 << 3)) == 0){}               // 等待接收FIFO有数据
		    *read_buffer++ = (uint8) base->RXDATA;
        }
        else
        {
            while((base->STATREG & (1 << 3)) == 0){}               // 等待接收FIFO有数据
        }
    }while(-- len);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介       SPI 接口 16bit 数据传输 发送与接收数据是同时进行的
// 参数说明       *base           要使用的SPI，(ECSPI1, ECSPI2, ECSP3, ECSPI4)  	
// 参数说明       data            数据
// 返回参数       void
// 备注信息       软件拉高CS片选信号，使用函数前需拉低CS，使用后需拉高CS
//-------------------------------------------------------------------------------------------------------------------
uint16 spi_transfer_16bit (ECSPI_Type *base, const uint16 data)
{
    uint16 rxdata;
    base->CONREG &= ~(3 << 18);
	base->CONREG |= (spi_get_channel(base) << 18);

    while((base->STATREG & (1 << 0)) == 0){}                         // 等待发送FIFO为空
        base->TXDATA = (uint32)((data & 0xFF00) >> 8);              // 将发送的数据写入缓冲区

	while((base->STATREG & (1 << 3)) == 0){}                         // 等待接收FIFO有数据
		rxdata = (uint16)base->RXDATA;
        
    while((base->STATREG & (1 << 0)) == 0){}                         // 等待发送FIFO为空
        base->TXDATA = (uint32)(data & 0x00FF);                      // 将发送的数据写入缓冲区

	while((base->STATREG & (1 << 3)) == 0){}                    // 等待接收FIFO有数据
		rxdata = (data << 8) |(uint16)base->RXDATA;

    return rxdata;                                                // 返回接收到的数据

}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介       SPI 16bit 数据传输 发送与接收数据是同时进行的
// 参数说明       *base           要使用的SPI，(ECSPI1, ECSPI2, ECSP3, ECSPI4) 
// 参数说明       write_buffer    发送的数据缓冲区地址
// 参数说明       read_buffer     发送数据时接收到的数据的存储地址(不需要接收则传 NULL)
// 参数说明       len             缓冲区长度
// 返回参数       void
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void spi_transfer_16bit_array (ECSPI_Type *base, const uint16 *write_buffer, uint16 *read_buffer, uint32 len)
{
    base->CONREG &= ~(3 << 18);
	base->CONREG |= (spi_get_channel(base) << 18);

    do
    {
        while((base->STATREG & (1 << 0)) == 0){}                 // 等待发送FIFO为空
            base->TXDATA = (uint32)((*write_buffer & 0xFF00) >> 8);     // 将发送的数据写入缓冲区

        if(read_buffer != NULL)
        {
             while((base->STATREG & (1 << 3)) == 0){}                // 等待接收FIFO有数据
		    *read_buffer = (uint16)base->RXDATA & 0x00FF;  
        }
        else
        {
             while((base->STATREG & (1 << 3)) == 0){}                // 等待接收FIFO有数据
        }

        while((base->STATREG & (1 << 0)) == 0){}                 // 等待发送FIFO为空
            base->TXDATA = (uint32)(*write_buffer & 0x00FF);           // 将发送的数据写入缓冲区
            
        if(read_buffer != NULL)
        {
            while((base->STATREG & (1 << 3)) == 0){}                // 等待接收FIFO有数据
		    *read_buffer = (*read_buffer << 8) |(uint16)base->RXDATA;
        }
        else
        {
            while((base->STATREG & (1 << 3)) == 0){}                // 等待接收FIFO有数据
        }

        write_buffer ++;
        read_buffer ++;
    }while(-- len);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介       SPI 接口向传感器的寄存器传输 8bit 数据
// 参数说明       *base           要使用的SPI，(ECSPI1, ECSPI2, ECSP3, ECSPI4) 
// 参数说明       register_name   寄存器地址
// 参数说明       data            数据
// 返回参数       uint8           接受的数据
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8 spi_transfer_8bit_register (ECSPI_Type *base, const uint8 register_name, const uint8 data)
{
    spi_transfer_8bit(base, register_name);
	return spi_transfer_8bit(base, data);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介       SPI 接口向传感器的寄存器传输 8bit 数组
// 参数说明       *base           要使用的SPI，(ECSPI1, ECSPI2, ECSP3, ECSPI4) 
// 参数说明       register_name   寄存器地址
// 参数说明       *data           数据存放缓冲区
// 参数说明       len             缓冲区长度
// 返回参数       void
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void spi_transfer_8bit_registers (ECSPI_Type *base, const uint8 register_name, uint8 *data, uint32 len)
{
    spi_transfer_8bit(base, register_name);
    spi_transfer_8bit_array(base, NULL, data, len);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介       SPI 接口向传感器的寄存器传输 16bit 数据
// 参数说明       *base           要使用的SPI，(ECSPI1, ECSPI2, ECSP3, ECSPI4) 
// 参数说明       register_name   寄存器地址
// 参数说明       data            数据
// 返回参数       void
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void spi_transfer_16bit_register (ECSPI_Type *base, const uint16 register_name, const uint16 data)
{
    spi_transfer_16bit(base, register_name);
    spi_transfer_16bit(base, data);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介       SPI 接口向传感器的寄存器传输 16bit 数组
// 参数说明       *base           要使用的SPI，(ECSPI1, ECSPI2, ECSP3, ECSPI4) 
// 参数说明       register_name   寄存器地址
// 参数说明       *data           数据存放缓冲区
// 参数说明       len             缓冲区长度
// 返回参数       void
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void spi_transfer_16bit_registers (ECSPI_Type *base, const uint16 register_name, const uint16 *data, uint32 len)
{
    spi_transfer_16bit(base, register_name);
    spi_transfer_16bit_array(base, data, NULL, len);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     SPI 初始化
// 参数说明     *base            要使用的SPI，(ECSPI1, ECSPI2, ECSP3, ECSPI4)  	
// 参数说明     baudRatebase     波特率	
// 返回参数     void          
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void spi_init(ECSPI_Type *base, uint32 baudRate)
{
    /* Reset control register to default value */
    ///ECSPI_SoftwareReset(base);
    /* 配置CONREG寄存器
	 * bit0 : 		1 	使能ECSPI
	 * bit3 : 		1	当向TXFIFO写入数据以后立即开启SPI突发。
	 * bit[7:4] : 	0001 SPI通道0主模式，根据实际情况选择，
	 *            	   	开发板上的ICM-20608接在SS0上，所以设置通道0为主模式
	 * bit[19:18]:	00 	选中通道0(其实不需要，因为片选信号我们我们自己控制)
	 * bit[31:20]:	0x7	突发长度为8个bit。 
	 */
    base->CONREG = 0; // 先清除控制寄存器
	base->CONREG |= (1 << 0) | (1 << 3) | (1 << 4 << spi_get_channel(base)) | (7 << 20);
    /*
     * ECSPI通道0设置,即设置CONFIGREG寄存器
     * bit0:	0 通道0 PHA为0
     * bit4:	0 通道0 SCLK高电平有效
     * bit8: 	0 通道0 片选信号 当SMC为1的时候此位无效
     * bit12：	0 通道0 POL为0
     * bit16：	0 通道0 数据线空闲时高电平
     * bit20:	0 通道0 时钟线空闲时低电平
	 */
	base->CONFIGREG = 0; 		/* 设置通道寄存器 */
    /*  
     * ECSPI设置采样周期
     * bit[14:0] :	0X2000  采样等待周期，比如当SPI时钟为10MHz的时候
     *  		    0X2000就等于1/10000 * 0X2000 = 0.8192ms，也就是连续
     *          	读取数据的时候每次之间间隔0.8ms
     * bit15	 :  0  采样时钟源为SPI CLK
     * bit[21:16]:  0  片选延时，可设置为0~63
	 */
	base->PERIODREG = 0X2000;		/* 设置采样周期寄存器 */
   	/*
     * ECSPI的SPI时钟配置，SPI的时钟源来源于pll3_sw_clk/8=480/8=60MHz
     * 通过设置CONREG寄存器的PER_DIVIDER(bit[11:8])和POST_DIVEDER(bit[15:12])来
     * 对SPI时钟源分频，获取到我们想要的SPI时钟：
     * SPI CLK = (SourceCLK / PER_DIVIDER) / (2^POST_DIVEDER)
     * 比如我们现在要设置SPI时钟为6MHz，那么PER_DIVEIDER和POST_DEIVIDER设置如下：
     * PER_DIVIDER = 0X9。
     * POST_DIVIDER = 0X0。
     * SPI CLK = 60000000/(0X9 + 1) = 60000000=6MHz
	 */
#if 0
    base->CONREG &= ~((0XF << 12) | (0XF << 8));	// 清除PER_DIVDER和POST_DIVEDER以前的设置
	base->CONREG |= (0X9 << 12);					// 设置SPI CLK = 6MHz
#else
    ECSPI_SetBaudRate(base, baudRate, 60000000);
#endif
}