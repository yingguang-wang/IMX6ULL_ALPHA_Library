#include "driver_iic.h"

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IIC 设置波特率对应值
// 参数说明     *base       	要使用的IIC, 由I2C_Type决定
// 参数说明     baudRate_Bps    波特率
// 参数说明     srcClock_Hz    	使用的时钟速率
// 返回参数     void        
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void iic_master_set_baudRate(I2C_Type *base, uint32 baudRate_Bps, uint32 srcClock_Hz)
{
    uint32 computedRate;
    uint32 absError;
    uint32 bestError = UINT32_MAX;
    uint32 bestIcr = 0u;
    uint8_t i;

    /* Scan table to find best match. */
    for (i = 0u; i < sizeof(s_iicDividerTable) / sizeof(s_iicDividerTable[0]); ++i)
    {
        computedRate = srcClock_Hz / s_iicDividerTable[i];
        absError = baudRate_Bps > computedRate ? (baudRate_Bps - computedRate) : (computedRate - baudRate_Bps);

        if (absError < bestError)
        {
            bestIcr = i;
            bestError = absError;

            /* If the error is 0, then we can stop searching because we won't find a better match. */
            if (absError == 0)
            {
                break;
            }
        }
    }

    /* Set frequency register based on best settings. */
    base->IFDR = I2C_IFDR_IC(bestIcr);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IIC 初始化
// 参数说明     *base       要使用的IIC, (I2C1, I2C2, I2C3, I2C4)
// 参数说明     baudRate    波特率
// 返回参数     void        
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void iic_init(I2C_Type *base, uint32 baudRate)
{
    /* 配置I2C */
    base->I2CR &= ~(I2C_I2CR_IEN_MASK); // 要访问I2C的寄存器，首先需要先关闭I2C

    /* Clear all flags. */
    base->I2SR = (uint8_t)I2C_I2SR_IAL_MASK | I2C_I2SR_IIF_MASK;
    
    /* 设置波特率为100K
     * I2C的时钟源来源于IPG_CLK_ROOT=66Mhz
 	 * IC2 时钟 = PERCLK_ROOT/dividison(IFDR寄存器)
	 * 设置寄存器IFDR,IFDR寄存器参考IMX6UL参考手册P1260页，表29-3，
	 * 根据表29-3里面的值，挑选出一个还是的分频数，比如本例程我们
	 * 设置I2C的波特率为100K， 因此当分频值=66000000/100000=660.
	 * 在表29-3里面查找，没有660这个值，但是有640，因此就用640,
	 * 即寄存器IFDR的IC位设置为0X15
	 */
    //base->IFDR = 0X15 << 0;
    iic_master_set_baudRate(base, baudRate, 66000000U);

    /*
     * 设置寄存器I2CR，开启I2C
     * bit[7] : 1 使能I2C,I2CR寄存器其他位其作用之前，此位必须最先置1
	 */
    base->I2CR = I2C_I2CR_IEN(1);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IIC 发送重新开始信号
// 参数说明     *base       要使用的IIC, 由I2C_Type决定
// 参数说明     address     设备地址
// 参数说明     direction   方向 
// 返回参数     uint8        0 正常 其他值 出错
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
uint8 iic_master_repeated_start(I2C_Type *base, uint8 address,  iic_direction_t direction)
{
	/* I2C忙并且工作在从模式,跳出 */
	if(base->I2SR & (1 << 5) && (((base->I2CR) & (1 << 5)) == 0))		
		return I2C_STATUS_BUSY;

	/*
     * 设置寄存器I2CR
     * bit[4]: 1 发送
     * bit[2]: 1 产生重新开始信号
	 */
	base->I2CR |=  (1 << 4) | (1 << 2);

	/*
     * 设置寄存器I2DR
     * bit[7:0] : 要发送的数据，这里写入从设备地址
     *            参考资料:IMX6UL参考手册P1249
	 */ 
	base->I2DR = ((uint32)address << 1) | ((direction == kI2C_Read)? 1 : 0);
	
	return I2C_STATUS_OK;
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IIC START 信号
// 参数说明     *base       要使用的IIC, 由"MCIMX6Y2.h"的I2C_Type决定
// 参数说明     address     设备地址
// 返回参数     uint8       0 正常 其他值 出错
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
uint8 iic_master_start (I2C_Type *base, uint8 address,  iic_direction_t direction)
{
    if(base->I2SR & (1 << 5))   return I2C_STATUS_BUSY;	/* I2C忙 */

	/*
     * 设置寄存器I2CR
     * bit[5]: 1 主模式
     * bit[4]: 1 发送
	 */
	base->I2CR |=  (1 << 5) | (1 << 4);

	/*
     * 设置寄存器I2DR
     * bit[7:0] : 要发送的数据，这里写入从设备地址
     *            参考资料:IMX6UL参考手册P1249
	 */ 
	base->I2DR = ((uint32)address << 1) | ((direction == kI2C_Read)? 1 : 0);
	return I2C_STATUS_OK;                                         // SCL 再拉低
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IIC 检查并清除错误
// 参数说明     *base      要使用的IIC, 由"MCIMX6Y2.h"的I2C_Type决定
// 参数说明     status     状态
// 返回参数     uint8      状态结果
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
uint8 iic_check_and_clear_error(I2C_Type *base, uint32 status)
{
	/* 检查是否发生仲裁丢失错误 */
	if(status & (1<<4))
	{
		base->I2SR &= ~(1<<4);		// 清除仲裁丢失错误位

		base->I2CR &= ~(1 << 7);	// 先关闭I2C
		base->I2CR |= (1 << 7);		// 重新打开I2C
		return I2C_STATUS_ARBITRATIONLOST;
	} 
	else if(status & (1 << 0))     	// 没有接收到从机的应答信号
	{
		return I2C_STATUS_NAK;		// 返回NAK(No acknowledge)
	}
	return I2C_STATUS_OK;
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IIC STOP 信号
// 参数说明     *base       要使用的IIC, 由"MCIMX6Y2.h"的I2C_Type决定
// 返回参数     uint8       状态结果
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
uint8 iic_master_stop(I2C_Type *base)
{
	uint32 timeout = 0xffff;

	/*
	 * 清除I2CR的bit[5:3]这三位
	 */
	base->I2CR &= ~((1 << 5) | (1 << 4) | (1 << 3));

	/* 等待忙结束 */
	while((base->I2SR & (1 << 5)))
	{
		timeout--;
		if(timeout == 0)	// 超时跳出
			return I2C_STATUS_TIMEOUT;
	}
	return I2C_STATUS_OK;
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IIC 发送数据
// 参数说明     *base      要使用的IIC, 由"MCIMX6Y2.h"的I2C_Type决定
// 参数说明     *buf       要发送的数据
// 参数说明     size       要发送的数据大小
// 返回参数     void      
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void iic_master_write_data(I2C_Type *base, const uint8 *buf, uint32 size)
{
	/* 等待传输完成 */
	while(!(base->I2SR & (1 << 7))); 
	
	base->I2SR &= ~(1 << 1); 	            // 清除标志位
	base->I2CR |= 1 << 4;		            // 发送数据
	
	while(size--)
	{
		base->I2DR = *buf++; 	            // 将buf中的数据写入到I2DR寄存器
		
		while(!(base->I2SR & (1 << 1))); 	// 等待传输完成
		base->I2SR &= ~(1 << 1);			// 清除标志位

		/* 检查ACK */
		if(iic_check_and_clear_error(base, base->I2SR))
			break;
	}
	
	base->I2SR &= ~(1 << 1);
	iic_master_stop(base); 	// 发送停止信号
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IIC 读取数据
// 参数说明     *base      要使用的IIC, 由"MCIMX6Y2.h"的I2C_Type决定
// 参数说明     *buf       读取到数据
// 参数说明     size       要读取的数据大小,如果只接收一个字节数据的话发送NACK(非应答)信号
// 返回参数     void      
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void iic_master_read_data(I2C_Type *base, uint8 *buf, uint32 size)
{
	vuint8 dummy = 0;
	dummy++; 	/* 防止编译报错 */
	
	/* 等待传输完成 */
	while(!(base->I2SR & (1 << 7))); 
	
	base->I2SR &= ~(1 << 1); 				//清除中断挂起位
	base->I2CR &= ~((1 << 4) | (1 << 3));	//接收数据
	
	/* 如果只接收一个字节数据的话发送NACK信号 */
	if(size == 1)
        base->I2CR |= (1 << 3);

	dummy = base->I2DR; /* 假读 */
	
	while(size--)
	{
		while(!(base->I2SR & (1 << 1))); 	//等待传输完成
		base->I2SR &= ~(1 << 1);			// 清除标志位

	 	if(size == 0)
        {
        	iic_master_stop(base); 			// 发送停止信号 
        }

        if(size == 1)
        {
            base->I2CR |= (1 << 3);
        }
		*buf++ = base->I2DR;
	}
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IIC 数据传输，包括读和写
// 参数说明     *base      要使用的IIC, 由"MCIMX6Y2.h"的I2C_Type决定
// 参数说明     *buf       传输结构体
// 参数说明     uint8      传输结果,0 成功，其他值 失败;
// 返回参数     void      
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 iic_master_transfer(I2C_Type *base, iic_transfer_t *xfer)
{
	uint8 ret = 0;     //传输状态
	iic_direction_t direction = xfer->direction;	

	base->I2SR &= ~((1 << 1) | (1 << 4));			// 清除标志位

	/* 等待传输完成 */
	while(!((base->I2SR >> 7) & 0X1)){}; 

	/* 如果是读的话，要先发送寄存器地址，所以要先将方向改为写 */
    if ((xfer->sub_address_size > 0) && (xfer->direction == kI2C_Read))
    {
        direction = kI2C_Write;
    }

	ret = iic_master_start(base, xfer->slave_address, direction); // 发送开始信号
    if(ret)
    {	
		return ret;
	}

	while(!(base->I2SR & (1 << 1))){};			                // 等待传输完成 

    ret = iic_check_and_clear_error(base, base->I2SR);	        // 检查是否出现传输错误 
    if(ret)
    {
      	iic_master_stop(base); 						            // 发送出错，发送停止信号
        return ret;
    }
	
    /* 发送寄存器地址 */
    if(xfer->sub_address_size)
    {
        do
        {
			base->I2SR &= ~(1 << 1);			                // 清除标志位
            xfer->sub_address_size--;				            // 地址长度减一
            //向I2DR寄存器写入子地址
            base->I2DR =  ((xfer->sub_address) >> (8 * xfer->sub_address_size)); 
  
			while(!(base->I2SR & (1 << 1)));  	                // 等待传输完成

            /* 检查是否有错误发生 */
            ret = iic_check_and_clear_error(base, base->I2SR);
            if(ret)
            {
             	iic_master_stop(base); 				            // 发送停止信号
             	return ret;
            }  
        } while ((xfer->sub_address_size > 0) && (ret == I2C_STATUS_OK));

        if(xfer->direction == kI2C_Read) 		                // 读取数据 
        {
            base->I2SR &= ~(1 << 1);			                // 清除中断挂起位
            iic_master_repeated_start(base, xfer->slave_address, kI2C_Read); // 发送重复开始信号和从机地址
    		while(!(base->I2SR & (1 << 1))){};                   // 等待传输完成

            /* 检查是否有错误发生 */
			ret = iic_check_and_clear_error(base, base->I2SR);
            if(ret)
            {
             	ret = I2C_STATUS_ADDRNAK;
                iic_master_stop(base); 		                    // 发送停止信号
                return ret;  
            }
           	          
        }
    }	


    /* 发送数据 */
    if ((xfer->direction == kI2C_Write) && (xfer->data_size > 0))
    {
    	iic_master_write_data(base, xfer->data, xfer->data_size);
	}

    /* 读取数据 */
    if ((xfer->direction == kI2C_Read) && (xfer->data_size > 0))
    {
       	iic_master_read_data(base, xfer->data, xfer->data_size);
	}
	return 0;	
}
