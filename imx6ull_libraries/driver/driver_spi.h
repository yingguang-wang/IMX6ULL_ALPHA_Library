#ifndef __DRIVER_SPI__H__
#define __DRIVER_SPI__H__

#include "common_typedef.h"
#include "MCIMX6Y2.h"

/* SPI通道 */
typedef enum
{
    SPI_CHANNEL_0 = 0U,
    SPI_CHANNEL_1,
    SPI_CHANNEL_2,
    SPI_CHANNEL_3,
}spi_channel_enum;

//====================================================SPI 基础函数====================================================
uint8       spi_transfer_8bit(ECSPI_Type *base, const uint8 data);
uint16      spi_transfer_16bit (ECSPI_Type *base, const uint16 data);

void        spi_transfer_8bit_array (ECSPI_Type *base, const uint8 *write_buffer, uint8 *read_buffer, uint32 len);
void        spi_transfer_16bit_array (ECSPI_Type *base, const uint16 *write_buffer, uint16 *read_buffer, uint32 len);

uint8       spi_transfer_8bit_register (ECSPI_Type *base, const uint8 register_name, const uint8 data);
void        spi_transfer_8bit_registers (ECSPI_Type *base, const uint8 register_name, uint8 *data, uint32 len);

void        spi_transfer_16bit_register (ECSPI_Type *base, const uint16 register_name, const uint16 data);
void        spi_transfer_16bit_registers (ECSPI_Type *base, const uint16 register_name, const uint16 *data, uint32 len);

void        spi_init(ECSPI_Type *base, uint32 baudRate);
spi_channel_enum spi_get_channel(ECSPI_Type *base);
//====================================================SPI 基础函数====================================================



#endif  //!__DRIVER_SPI__H__