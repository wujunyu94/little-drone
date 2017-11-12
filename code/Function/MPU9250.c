#include "MPU9250.h"
#include "stdio.h"

typedef struct{
	short Accel[3];//Accel X,Y,Z
	short Gyro[3];//Gyro X,Y,Z
	short Mag[3];	//Mag X,Y,Z	
}MPU_value;

MPU_value mpu_value;          //9������
unsigned char BUF[6];       //�������ݻ�����
u8 a=0;



/***************************************************************/
//SPIx 
//TxData:����һ���ֽ�
//����ֵ:data
/***************************************************************/
static u8 SPI1_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //�ȴ�SPI���ͱ�־λ��
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(SPI1, TxData); //��������
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //�ȴ�SPI���ձ�־λ��
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI1); //��������					    
}
/***************************************************************/
//SPI����
//reg: addr
//value:����
/***************************************************************/
u8 MPU9250_Write_Reg(u8 reg,u8 value)
{
	u8 status;
	MPU_9250_ENABLE;   //	MPU9250_CS=0;  //ƬѡMPU9250
	status=SPI1_ReadWriteByte(reg); //����reg��ַ
	SPI1_ReadWriteByte(value);//��������
	MPU_9250_DISENABLE;//	MPU9250_CS=1;  //ʧ��MPU9250
	return(status);//
}
//---------------------------------------------------------------//
//SPI��ȡ
//reg: addr
u8 MPU9250_Read_Reg(u8 reg)
{
	  u8 reg_val;
		MPU_9250_ENABLE;//	MPU9250_CS=0;  //ƬѡMPU9250
	  SPI1_ReadWriteByte(reg|0x80); //reg��ַ+������
	  reg_val=SPI1_ReadWriteByte(0xff);//��������
		MPU_9250_DISENABLE;//	MPU9250_CS=1;  //ʧ��MPU9250
	return(reg_val);
}

