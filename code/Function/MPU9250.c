#include "MPU9250.h"
#include "stdio.h"

typedef struct{
	short Accel[3];//Accel X,Y,Z
	short Gyro[3];//Gyro X,Y,Z
	short Mag[3];	//Mag X,Y,Z	
}MPU_value;

MPU_value mpu_value;          //9轴数据
unsigned char BUF[6];       //接收数据缓存区
u8 a=0;



/***************************************************************/
//SPIx 
//TxData:发送一个字节
//返回值:data
/***************************************************************/
static u8 SPI1_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //等待SPI发送标志位空
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(SPI1, TxData); //发送数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //等待SPI接收标志位空
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI1); //接收数据					    
}
/***************************************************************/
//SPI发送
//reg: addr
//value:数据
/***************************************************************/
u8 MPU9250_Write_Reg(u8 reg,u8 value)
{
	u8 status;
	MPU_9250_ENABLE;   //	MPU9250_CS=0;  //片选MPU9250
	status=SPI1_ReadWriteByte(reg); //发送reg地址
	SPI1_ReadWriteByte(value);//发送数据
	MPU_9250_DISENABLE;//	MPU9250_CS=1;  //失能MPU9250
	return(status);//
}
//---------------------------------------------------------------//
//SPI读取
//reg: addr
u8 MPU9250_Read_Reg(u8 reg)
{
	  u8 reg_val;
		MPU_9250_ENABLE;//	MPU9250_CS=0;  //片选MPU9250
	  SPI1_ReadWriteByte(reg|0x80); //reg地址+读命令
	  reg_val=SPI1_ReadWriteByte(0xff);//任意数据
		MPU_9250_DISENABLE;//	MPU9250_CS=1;  //失能MPU9250
	return(reg_val);
}

