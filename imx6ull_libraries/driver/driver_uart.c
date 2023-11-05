#include "driver_uart.h"
#include "fsl_iomuxc.h"
#include "buzzer.h"
#include "driver_gpio.h"

iomuxc_value_t iomuxc_tx;
iomuxc_value_t iomuxc_rx;
//-------------------------------------------------------------------------------------------------------------------
//  函数简介      串口初始化
//  参数说明      base           串口模块号(UART1,UART2,UART3,UART4,UART5)
//  参数说明      baud           串口波特率
//  返回参数      uint32         实际波特率
//  备注信息
//-------------------------------------------------------------------------------------------------------------------
void uart_init(UART_Type* base, uint32 baud)
{
    uart_config_t uart_config;
    uart_config.baudRate_Bps = baud;
    uart_config.parityMode = kUART_ParityDisabled;
    uart_config.dataBitsCount = kUART_EightDataBits;
    uart_config.stopBitCount = kUART_OneStopBit;
	uart_config.txFifoWatermark = 2;
	uart_config.rxFifoWatermark = 1;
    uart_config.enableAutoBaudRate = false;
    uart_config.enableTx = true;
    uart_config.enableRx = true;
    if (base == UART1)
    {
        iomuxc_tx = (iomuxc_value_t){ IOMUXC_UART1_TX_DATA_UART1_TX };
        iomuxc_rx = (iomuxc_value_t){ IOMUXC_UART1_RX_DATA_UART1_RX };
    }
    else if (base == UART2)
    {
        iomuxc_tx = (iomuxc_value_t){ IOMUXC_UART2_TX_DATA_UART2_TX };
        iomuxc_rx = (iomuxc_value_t){ IOMUXC_UART2_RX_DATA_UART2_RX };
    }
    else if (base == UART3)
    {
        iomuxc_tx = (iomuxc_value_t){ IOMUXC_UART3_TX_DATA_UART3_TX };
        iomuxc_rx = (iomuxc_value_t){ IOMUXC_UART3_RX_DATA_UART3_RX };
    }
    else if (base == UART4)
    {
        iomuxc_tx = (iomuxc_value_t){ IOMUXC_UART4_TX_DATA_UART4_TX };
        iomuxc_rx = (iomuxc_value_t){ IOMUXC_UART4_RX_DATA_UART4_RX };
    }
    else if (base == UART5)
    {
        iomuxc_tx = (iomuxc_value_t){ IOMUXC_UART5_TX_DATA_UART5_TX };
        iomuxc_rx = (iomuxc_value_t){ IOMUXC_UART5_RX_DATA_UART5_RX };
    }

	iomuxc_set(iomuxc_tx,0,0x10B0);
	iomuxc_set(iomuxc_rx,0,0x10B0);

	UART_Init(base, &uart_config, 80000000);	
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介       串口发送写入
// 参数说明       base          串口模块号(UART1,UART2,UART3,UART4,UART5)
// 参数说明       dat           需要发送的字节
// 返回参数       void
// 使用示例       uart_write_byte(UART1, 0xA5);                    // 往串口1的发送缓冲区写入0xA5，写入后仍然会发送数据，但是会减少CPU在串口的执行时
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void uart_write_byte(UART_Type* base, uint8 dat)
{
    while (((base->USR2 >> 3) & 0X01) == 0);/* 等待上一次发送完成 */
    base->UTXD = dat & UART_UTXD_TX_DATA_MASK;
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介       串口发送数组
// 参数说明       base          串口模块号(UART1,UART2,UART3,UART4,UART5)
// 参数说明       *buff         要发送的数组地址
// 参数说明       len           发送长度
// 返回参数       void
// 使用示例       uart_write_buffer(UART1, &a[0], 5);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void uart_write_buffer(UART_Type* base, const uint8* buff, uint32 len)
{
    while (len)
    {
        uart_write_byte(base, *buff);
        len--;
        buff++;
    }
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介       串口发送字符串
// 参数说明       base          串口模块号(UART1,UART2,UART3,UART4,UART5)
// 参数说明       *str          要发送的字符串地址
// 返回参数       void
// 使用示例       uart_write_string(UART1, "IMX6ULL");
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void uart_write_string(UART_Type* base, uint8 *str)
{
    while (*str)
    {
        uart_write_byte(base, *str++);
    }
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介       读取串口接收的数据（whlie等待）
// 参数说明       base          串口模块号(UART1,UART2,UART3,UART4,UART5)
// 参数说明       *dat          接收数据的地址
// 返回参数       uint8         接收的数据
// 使用示例       uint8 dat = uart_read_byte(UART1);             // 接收 UART_1 数据  存在在 dat 变量里
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8 uart_read_byte(UART_Type* base)
{
    while ((UART1->USR2 & 0x1) == 0);/* 等待接收完成 */
    return (uint8_t)((base->URXD & UART_URXD_RX_DATA_MASK) >> UART_URXD_RX_DATA_SHIFT);				/* 返回接收到的数据 */
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介       读取串口接收的数据（查询接收）
// 参数说明       base           串口模块号(UART1,UART2,UART3,UART4,UART5)
// 参数说明       *dat           接收数据的地址
// 返回参数       uint8          1：接收成功   0：未接收到数据
// 使用示例       uint8 dat; uart_query_byte(UART_1, &dat);       // 接收 UART_1 数据  存在在 dat 变量里
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8 uart_query_byte(UART_Type* base, uint8* dat)
{
    uint8 return_num = 0;
    if ((UART1->USR2 & 0x1) == 0)
    {
        *dat = (uint8_t)((base->URXD & UART_URXD_RX_DATA_MASK) >> UART_URXD_RX_DATA_SHIFT);
        return_num = 1;
    }
    return return_num;
}
/*防止编译器报错*/
int raise(void)
{
    return 0;
}