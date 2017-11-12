#ifndef _I2C_H_
#define _I2C_H_
#include "stm32f10x.h"


struct _acc
{
	int16_t x;
	int16_t y;
	int16_t z;
};
extern struct _acc acc;
extern struct _acc filter_acc;

struct _gyro
{
	int16_t x;
	int16_t y;
	int16_t z;
};
extern struct _gyro gyro;
extern struct _gyro filter_gyro;
/******************************************************************************
							全局函数声明
*******************************************************************************/ 
void I2C_Int(void);
void MPU6050_Compose(void);
uint8_t InitMPU6050(void);
uint8_t MPU6050_SequenceRead(void);
/******************************************************************************
							定义MPU6050寄存器地址
*******************************************************************************/ 
#define	SMPLRT_DIV		0x19	
#define	CONFIG2			0x1A	
#define	GYRO_CONFIG		0x1B	
#define	ACCEL_CONFIG	0x1C	
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I		0x75	//IIC地址寄存器的地址(默认数值0x68，只读!!!)

//定义器件在IIC总线中的从地址
//I2C器件地址是7bit而不是8bit，而模拟I2C用8bit传送地址，因此是0xD0（器件AD0引脚接地）
#define	MPU6050Address	0xD0

#endif

