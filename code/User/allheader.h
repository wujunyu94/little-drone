#ifndef __ALLHEADER_H__
#define __ALLHEADER_H__	

#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "echo.h" 
#include "IMU.h"
#include "Maths.h"
#include "Filter.h"
#include "Tasks.h"
#include "MPU9250.h"
#include "ppm.h" 
#include "led.h"
#include "SPI.h"
//#include "spi_nrf24l01.h"
#include "control.h"
#include "i2c.h"
#include "DMA_ADC.h"

struct _cnt
{
	uint8_t ms50;
	uint8_t ms2;
	uint8_t ms4;
  uint16_t ms500;
};
extern struct _cnt cnt;

extern int16_t temp[6];
#endif
