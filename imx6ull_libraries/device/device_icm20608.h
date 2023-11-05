#ifndef __DEVICE_ICM20608__H__
#define __DEVICE_ICM20608__H__

#include "driver_gpio.h"
#include "driver_spi.h"

//====================================================硬件 SPI 驱动====================================================
#define ICM20608_SPI_SPEED          (6 * 1000 * 1000)                          // 硬件 SPI 速率
#define ICM20608_SPI                ECSPI3                                   // 硬件 SPI 号
#define ICM20608_SPC_PIN            IOMUXC_UART2_RX_DATA_ECSPI3_SCLK         // 硬件 SPI SCK 引脚
#define ICM20608_SDI_PIN            IOMUXC_UART2_CTS_B_ECSPI3_MOSI           // 硬件 SPI MOSI 引脚
#define ICM20608_SDO_PIN            IOMUXC_UART2_RTS_B_ECSPI3_MISO           // 硬件 SPI MISO 引脚
//====================================================硬件 SPI 驱动====================================================
#define ICM20608_CS_PIN             (P01_20)                                    // CS 片选引脚
#define ICM20608_CS_PIN_IOMUXC      IOMUXC_UART2_TX_DATA_GPIO1_IO20           // CS 片选引脚模式
#define ICM20608_CS(x)              ((x) ? (gpio_high(ICM20608_CS_PIN)) : (gpio_low(ICM20608_CS_PIN)))
#define ICM20608_TIMEOUT_COUNT      (0x00FF)                                    // ICM20608 超时计数


#define ICM20608G_ID			0XAF	/* ID值 */
#define ICM20608D_ID			0XAE	/* ID值 */

#define ICM20608_DEV_ADDR           (0x69)                  // SA0接地：0x68 SA0上拉：0x69 模块默认上拉
#define ICM20608_SPI_W              (~0x80)
#define ICM20608_SPI_R              (0x80)

/* ICM20608寄存器 
 *复位后所有寄存器地址都为0，除了
 *Register 107(0X6B) Power Management 1 	= 0x40
 *Register 117(0X75) WHO_AM_I 				= 0xAF或0xAE
 */
//================================================定义 ICM20608 内部地址================================================
/* 陀螺仪和加速度自测(出产时设置，用于与用户的自检输出值比较） */
#define	ICM20608_SELF_TEST_X_GYRO		0x00
#define	ICM20608_SELF_TEST_Y_GYRO		0x01
#define	ICM20608_SELF_TEST_Z_GYRO		0x02
#define	ICM20608_SELF_TEST_X_ACCEL		0x0D
#define	ICM20608_SELF_TEST_Y_ACCEL		0x0E
#define	ICM20608_SELF_TEST_Z_ACCEL		0x0F
/* 陀螺仪静态偏移 */
#define	ICM20608_XG_OFFS_USRH			0x13
#define	ICM20608_XG_OFFS_USRL			0x14
#define	ICM20608_YG_OFFS_USRH			0x15
#define	ICM20608_YG_OFFS_USRL			0x16
#define	ICM20608_ZG_OFFS_USRH			0x17
#define	ICM20608_ZG_OFFS_USRL			0x18

#define	ICM20608_SMPLRT_DIV			    0x19
#define	ICM20608_CONFIG				    0x1A
#define	ICM20608_GYRO_CONFIG			0x1B
#define	ICM20608_ACCEL_CONFIG			0x1C
#define	ICM20608_ACCEL_CONFIG2			0x1D
#define	ICM20608_LP_MODE_CFG			0x1E
#define	ICM20608_ACCEL_WOM_THR			0x1F
#define	ICM20608_FIFO_EN				0x23
#define	ICM20608_FSYNC_INT				0x36
#define	ICM20608_INT_PIN_CFG			0x37
#define	ICM20608_INT_ENABLE			    0x38
#define	ICM20608_INT_STATUS			    0x3A
/* 加速度输出 */
#define	ICM20608_ACCEL_XOUT_H			0x3B
#define	ICM20608_ACCEL_XOUT_L			0x3C
#define	ICM20608_ACCEL_YOUT_H			0x3D
#define	ICM20608_ACCEL_YOUT_L			0x3E
#define	ICM20608_ACCEL_ZOUT_H			0x3F
#define	ICM20608_ACCEL_ZOUT_L			0x40
/* 温度输出 */
#define	ICM20608_TEMP_OUT_H			    0x41
#define	ICM20608_TEMP_OUT_L			    0x42
/* 陀螺仪输出 */
#define	ICM20608_GYRO_XOUT_H			0x43
#define	ICM20608_GYRO_XOUT_L			0x44
#define	ICM20608_GYRO_YOUT_H			0x45
#define	ICM20608_GYRO_YOUT_L			0x46
#define	ICM20608_GYRO_ZOUT_H			0x47
#define	ICM20608_GYRO_ZOUT_L			0x48

#define	ICM20608_SIGNAL_PATH_RESET		0x68
#define	ICM20608_ACCEL_INTEL_CTRL 		0x69
#define	ICM20608_USER_CTRL				0x6A
#define	ICM20608_PWR_MGMT_1			    0x6B
#define	ICM20608_PWR_MGMT_2			    0x6C
#define	ICM20608_FIFO_COUNTH			0x72
#define	ICM20608_FIFO_COUNTL			0x73
#define	ICM20608_FIFO_R_W				0x74
#define	ICM20608_WHO_AM_I 				0x75
/* 加速度静态偏移 */
#define	ICM20608_XA_OFFSET_H			0x77
#define	ICM20608_XA_OFFSET_L			0x78
#define	ICM20608_YA_OFFSET_H			0x7A
#define	ICM20608_YA_OFFSET_L			0x7B
#define	ICM20608_ZA_OFFSET_H			0x7D
#define	ICM20608_ZA_OFFSET_L 			0x7E

#define ICM20608_ACC_SAMPLE         (0x18)                                      // 加速度计量程
// 设置为:0x00 加速度计量程为:±2g          获取到的加速度计数据 除以16384      可以转化为带物理单位的数据，单位：g(m/s^2)
// 设置为:0x08 加速度计量程为:±4g          获取到的加速度计数据 除以8192       可以转化为带物理单位的数据，单位：g(m/s^2)
// 设置为:0x10 加速度计量程为:±8g          获取到的加速度计数据 除以4096       可以转化为带物理单位的数据，单位：g(m/s^2)
// 设置为:0x18 加速度计量程为:±16g         获取到的加速度计数据 除以2048       可以转化为带物理单位的数据，单位：g(m/s^2)

#define ICM20608_GYR_SAMPLE         (0x18)                                      // 陀螺仪量程
// 设置为:0x00 陀螺仪量程为:±250 dps     获取到的陀螺仪数据除以131           可以转化为带物理单位的数据，单位为：°/s
// 设置为:0x08 陀螺仪量程为:±500 dps     获取到的陀螺仪数据除以65.5          可以转化为带物理单位的数据，单位为：°/s
// 设置为:0x10 陀螺仪量程为:±1000dps     获取到的陀螺仪数据除以32.8          可以转化为带物理单位的数据，单位为：°/s
// 设置为:0x18 陀螺仪量程为:±2000dps     获取到的陀螺仪数据除以16.4          可以转化为带物理单位的数据，单位为：°/s
//================================================定义 ICM20608 内部地址================================================

//===============================================声明 ICM20608 数据存储变量==============================================
extern int16 icm20608_gyro_x, icm20608_gyro_y, icm20608_gyro_z;                                // 三轴陀螺仪数据
extern int16 icm20608_acc_x,  icm20608_acc_y,  icm20608_acc_z;                                 // 三轴加速度计数据
extern int16 icm20608_temp;        // 芯片温度(((float)(icm20608_dev_temp) - 25 ) / 326.8 + 25) * 100;
//===============================================声明 ICM20608 数据存储变量==============================================

//==================================================ICM20608 基础函数==================================================
void    icm20608_get_acc            (void);                                     // 获取 ICM20608 加速度计数据
void    icm20608_get_gyro           (void);                                     // 获取ICM20608陀螺仪数据
float   icm20608_gyro_transition    (int16 gyro_value);                         // 将 ICM20608 陀螺仪数据转换为实际物理数据
float   icm20608_acc_transition     (int16 acc_value);                          // 将 ICM20608 加速度计数据转换为实际物理数据
uint8   icm20608_init               (void);                                     // 初始化 ICM20608
//==================================================ICM20608 基础函数==================================================

#endif  //!__DEVICE_ICM20608__H__