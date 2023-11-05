#include "device_icm20608.h"
#include "printf.h"
#include "driver_delay.h"

int16 icm20608_gyro_x = 0, icm20608_gyro_y = 0, icm20608_gyro_z = 0;                    // 三轴陀螺仪数据       gyro (陀螺仪)
int16 icm20608_acc_x  = 0, icm20608_acc_y  = 0, icm20608_acc_z  = 0;                    // 三轴加速度计数据     acc (accelerometer 加速度计)
int16 icm20608_temp;                                                                    // 芯片温度

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ICM20608 写寄存器
// 参数说明     reg             寄存器地址
// 参数说明     data            数据
// 返回参数     void
// 使用示例     icm20608_write_register(ICM20608_PWR_MGMT_1, 0x80);
// 备注信息     ICM20608在使用SPI接口的时候寄存器地址只有低7位有效,
//             寄存器地址最高位是读/写标志位读的时候要为1，写的时候要为0。
//-------------------------------------------------------------------------------------------------------------------
static void icm20608_write_register(uint8 reg, uint8 data)
{
	ICM20608_CS(0);						/* 使能SPI传输			*/
    spi_transfer_8bit_register(ICM20608_SPI, reg & ICM20608_SPI_W, data);
	ICM20608_CS(1);						/* 禁止SPI传输			*/
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ICM20608 读寄存器
// 参数说明     reg             寄存器地址
// 返回参数     uint8           数据
// 使用示例     icm20608_read_register(ICM20608_WHO_AM_I);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static uint8 icm20608_read_register(uint8 reg)
{
    uint8 data = 0;
    ICM20608_CS(0);
    data = spi_transfer_8bit_register(ICM20608_SPI, reg | ICM20608_SPI_R, 0xFF);
    ICM20608_CS(1);
    return data;
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ICM20608 读数据
// 参数说明     reg             寄存器地址
// 参数说明     data            数据缓冲区
// 参数说明     len             数据长度
// 返回参数     void
// 使用示例     icm20608_read_registers(ICM20608_ACCEL_XOUT_H, dat, 6);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static void icm20608_read_registers(uint8 reg, uint8 *data, uint32 len)
{
    //uint8 i;
    ICM20608_CS(0);
		
    spi_transfer_8bit_registers(ECSPI3, reg | ICM20608_SPI_R, data, len);   

    ICM20608_CS(1);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 ICM20608 加速度计数据
// 参数说明     void
// 返回参数     void
// 使用示例     icm20608_get_acc();                                             // 执行该函数后，直接查看对应的变量即可
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void icm20608_get_acc (void)
{
    uint8 dat[8];

    icm20608_read_registers(ICM20608_ACCEL_XOUT_H, dat, 8);
    icm20608_acc_x = (int16)(((uint16)dat[0] << 8 | dat[1]));
    icm20608_acc_y = (int16)(((uint16)dat[2] << 8 | dat[3]));
    icm20608_acc_z = (int16)(((uint16)dat[4] << 8 | dat[5]));
    icm20608_temp   = (int16)(((uint16)dat[6] << 8 | dat[7])); 
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 ICM20608 陀螺仪数据
// 参数说明     void
// 返回参数     void
// 使用示例     icm20608_get_gyro();                                            // 执行该函数后，直接查看对应的变量即可
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void icm20608_get_gyro (void)
{
    uint8 dat[6];

    icm20608_read_registers(ICM20608_GYRO_XOUT_H, dat, 6);
    icm20608_gyro_x = (int16)(((uint16)dat[0] << 8 | dat[1]));
    icm20608_gyro_y = (int16)(((uint16)dat[2] << 8 | dat[3]));
    icm20608_gyro_z = (int16)(((uint16)dat[4] << 8 | dat[5]));
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     将 ICM20608 加速度计数据转换为实际物理数据
// 参数说明     gyro_value              // 任意轴的加速度计数据
// 返回参数     void
// 使用示例     float data = icm20608_acc_transition(icm20608_acc_x_acc_x);  //单位为 g(m/s^2)
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
float icm20608_acc_transition (int16 acc_value)
{
    float acc_data = 0;
    switch(ICM20608_ACC_SAMPLE)
    {
        case 0x00: acc_data = (float)acc_value / 16384; break;      // 0x00 加速度计量程为:±2g          获取到的加速度计数据 除以16384      可以转化为带物理单位的数据，单位：g(m/s^2)
        case 0x08: acc_data = (float)acc_value / 8192;  break;      // 0x08 加速度计量程为:±4g          获取到的加速度计数据 除以8192       可以转化为带物理单位的数据，单位：g(m/s^2)
        case 0x10: acc_data = (float)acc_value / 4096;  break;      // 0x10 加速度计量程为:±8g          获取到的加速度计数据 除以4096       可以转化为带物理单位的数据，单位：g(m/s^2)
        case 0x18: acc_data = (float)acc_value / 2048;  break;      // 0x18 加速度计量程为:±16g         获取到的加速度计数据 除以2048       可以转化为带物理单位的数据，单位：g(m/s^2)
        default: break;
    }
    return acc_data;
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     将 ICM20608 陀螺仪数据转换为实际物理数据
// 参数说明     gyro_value              // 任意轴的陀螺仪数据
// 返回参数     void
// 使用示例     float data = icm20608_gyro_transition(imu660ra_gyro_x);  // 单位为°/s
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
float icm20608_gyro_transition (int16 gyro_value)
{
    float gyro_data = 0;
    switch(ICM20608_GYR_SAMPLE)
    {
        case 0x00: gyro_data = (float)gyro_value / 131.2f;  break;  //  0x00 陀螺仪量程为:±250 dps     获取到的陀螺仪数据除以131           可以转化为带物理单位的数据，单位为：°/s
        case 0x08: gyro_data = (float)gyro_value / 65.6f;   break;  //  0x08 陀螺仪量程为:±500 dps     获取到的陀螺仪数据除以65.5          可以转化为带物理单位的数据，单位为：°/s
        case 0x10: gyro_data = (float)gyro_value / 32.8f;   break;  //  0x10 陀螺仪量程为:±1000dps     获取到的陀螺仪数据除以32.8          可以转化为带物理单位的数据，单位为：°/s
        case 0x18: gyro_data = (float)gyro_value / 16.4f;   break;  //  0x18 陀螺仪量程为:±2000dps     获取到的陀螺仪数据除以16.4          可以转化为带物理单位的数据，单位为：°/s
        default: break;
    }
    return gyro_data;
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     初始化 ICM20608
// 参数说明     void
// 返回参数     uint8           1-初始化失败 0-初始化成功
// 使用示例     icm20608_init();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8 icm20608_init(void)
{	
    uint8 val = 0x0, return_state = 0;
    uint16 timeout_count = 0;

    iomuxc_set((iomuxc_value_t){ICM20608_SPC_PIN}, 0, 0x10B1);
    iomuxc_set((iomuxc_value_t){ICM20608_SDI_PIN}, 0, 0x10B1);
    iomuxc_set((iomuxc_value_t){ICM20608_SDO_PIN}, 0, 0x10B1);   
    iomuxc_set((iomuxc_value_t){ICM20608_CS_PIN_IOMUXC}, 0, 0x10B0);
	gpio_init(ICM20608_CS_PIN, GPO, 0);

	spi_init(ICM20608_SPI,ICM20608_SPI_SPEED);	

	icm20608_write_register(ICM20608_PWR_MGMT_1, 0x80);		            // 复位，复位后为0x40,睡眠模式
	system_delay_ms(50);
    icm20608_write_register(ICM20608_PWR_MGMT_1, 0x01);                 // 时钟设置
    system_delay_ms(50);

    do
    {                                                                       // 等待复位成功
        val = icm20608_read_register(ICM20608_WHO_AM_I);
        printf("icm20608 id = %#X\r\n", val);
        if(timeout_count ++ > ICM20608_TIMEOUT_COUNT)
        {

                printf("icm20608 reset error.\r\n");
                return_state = 1;
                break;
        }
    }while(val != ICM20608G_ID && val != ICM20608D_ID);
    if(1 == return_state)
    {
        		return 1;   // ICM20608 自检出错并超时退出了
    }
	printf("icm20608 id = %#X\r\n", val);

    icm20608_write_register(ICM20608_PWR_MGMT_2,     0x00);                 // 开启陀螺仪和加速度计
    icm20608_write_register(ICM20608_CONFIG,         0x04);                 // 176HZ 1KHZ
    icm20608_write_register(ICM20608_ACCEL_CONFIG2, 0x04);                  // 21.2Hz 1KHZ
    //ICM20608_CONFIG,ICM20608_ACCEL_CONFIG2寄存器,低通滤波,3-dB BW
    // value   GYRO_BW(Hz) Rate  | ACC——BW(Hz)   Rate    |Temperature Sensor BW(Hz)  
    //  0x00      250      8KHZ  |   218.1       1KHZ    |4000
    //  0x01      176      1KHZ  |   218.1       1KHZ    |188
    //  0x02      93       1KHZ  |   99.0        1KHZ    |98
    //  0x03      41       1KHZ  |   44.8        1KHZ    |42
    //  0x04      20       1KHZ  |   21.2        1KHZ    |20
    //  0x05      10       1KHZ  |   10.2        1KHZ    |10
    //  0x06      5        1KHZ  |   5.1         1KHZ    |5
    //  0x07      3281     8KHZ  |   420.0       1KHZ    |4000
    icm20608_write_register(ICM20608_SMPLRT_DIV,     0x00);                 // 采样速率 SAMPLE_RATE = INTERNAL_SAMPLE_RATE / (1 + SMPLRT_DIV)
    icm20608_write_register(ICM20608_GYRO_CONFIG,    ICM20608_GYR_SAMPLE);  // ±2000 dps
    icm20608_write_register(ICM20608_ACCEL_CONFIG,   ICM20608_ACC_SAMPLE);  // ±8g
    icm20608_write_register(ICM20608_LP_MODE_CFG, 0x00); 	// 关闭低功耗
	icm20608_write_register(ICM20608_FIFO_EN, 0x00);		    // 关闭FIFO	

	return 0;
}
