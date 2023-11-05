#ifndef _DRIVER_UART_H_


#include "fsl_uart.h"
#include "common_typedef.h"

//====================================================串口 基础函数====================================================
void	uart_write_byte(UART_Type* base, uint8 dat);
void	uart_write_buffer(UART_Type* base, const uint8* buff, uint32 len);
void	uart_write_string(UART_Type* base, uint8 *str);

uint8	uart_read_byte(UART_Type* base);
uint8	uart_query_byte(UART_Type* base, uint8* dat);

void uart_init(UART_Type* base, uint32 baud);

//====================================================串口 基础函数====================================================

#endif // !_DRIVER_UART_H_
