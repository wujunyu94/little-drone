//#include "Control.h"
//#include "struct_all.h"
#include "spi_nrf24l01.h"
#include "Tasks.h"

/******************************************************************************
							��������
*******************************************************************************/ 
uint8_t NRF24L01_RXDATA[32];//nrf24l01���յ�������
uint8_t NRF24L01_TXDATA[32];//nrf24l01��Ҫ���͵�����
static uint8_t TX_ADDRESS[5]= {0x1A,0x3B,0x5C,0x7D,0x9E};//���ص�ַ
static uint8_t RX_ADDRESS[5]= {0x1A,0x3B,0x5C,0x7D,0x9E};//���յ�ַ
uint16_t Nrf_Erro;
extern uint16_t duty_ref;

/******************************************************************************
����ԭ��:	void SPI1_Init(void)
��������:	��ʼ��SPI����
*******************************************************************************/ 
void SPI1_Init(void)
{
	SPI_InitTypeDef SPI_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
 NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	//����SCK,MISO,MOSI����  
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7; 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //���츴��
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	//����MISO
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6; 
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
	//����CE����
	GPIO_InitStructure.GPIO_Pin = NRF_CE_Pin; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
	GPIO_Init(NRF_CE_GPIO, &GPIO_InitStructure);
	//����CSN����
	GPIO_InitStructure.GPIO_Pin = NRF_CSN_Pin; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
	GPIO_Init(NRF_CSN_GPIO, &GPIO_InitStructure);	
	//����IRQ����
	GPIO_InitStructure.GPIO_Pin = NRF_IRQ_Pin; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //��������
	GPIO_Init(NRF_IRQ_GPIO, &GPIO_InitStructure);	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource8);
    EXTI_InitStructure.EXTI_Line=EXTI_Line8;
    EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;//�ⲿ�ж�
    EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;//�½��ش���
    EXTI_InitStructure.EXTI_LineCmd=ENABLE;
    EXTI_Init(&EXTI_InitStructure);
		//Nrf2401�ж�
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	NRF_CSN_H;	//��ֹNRF����
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //˫��ȫ˫�� 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //��ģʽ 
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //���ݴ�С8λ 
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; //ʱ�Ӽ��ԣ�����ʱΪ�� 
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; //��1��������Ч��������Ϊ����ʱ�� 
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;  //NSS�ź���������� 
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; //8��Ƶ��9MHz 
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //��λ��ǰ 
	SPI_InitStructure.SPI_CRCPolynomial = 7; 
	SPI_Init(SPI1, &SPI_InitStructure); 
	   
	SPI_Cmd(SPI1, ENABLE);//ʹ�� SPI1
	
	//PrintString("\r\n SPI1     ��ʼ����ɣ�");
}
/******************************************************************************
����ԭ��:	uint8_t SPI_RW(uint8_t data) 
��������:	SPI���߶�д
�� �� ֵ��	����SPI���߶�ȡ����
*******************************************************************************/ 
uint8_t SPI_RW(uint8_t data) 
{ 
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);//��SPI���ͻ������ǿ�ʱ�ȴ�  
	SPI_I2S_SendData(SPI1, data);//ͨ��SPI���߷���һ�ֽ�����
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);//��SPI���ջ�����Ϊ��ʱ�ȴ�
	return SPI_I2S_ReceiveData(SPI1);
}

/******************************************************************************
����ԭ�ͣ�	uint8_t NRF_Write_Reg(uint8_t reg, uint8_t value)
��    �ܣ�	NRFд�Ĵ���
�� �� ֵ��	NRFд�Ĵ�������ֵ
*******************************************************************************/
uint8_t NRF_Write_Reg(uint8_t reg, uint8_t value)
{
	uint8_t status;
	NRF_CSN_L;		//ѡͨNRF����
	status = SPI_RW(reg);//д�Ĵ�����ַ
	SPI_RW(value);	//д����
	NRF_CSN_H;		//��ֹNRF����
	return 	status;
}

/******************************************************************************
����ԭ�ͣ�	uint8_t NRF_Read_Reg(uint8_t reg)
��    �ܣ�	NRF���Ĵ���
�� �� ֵ��	�Ĵ�������
*******************************************************************************/
uint8_t NRF_Read_Reg(uint8_t reg)
{
	uint8_t reg_val;
	NRF_CSN_L;		//ѡͨNRF���� 
	SPI_RW(reg);	//д�Ĵ�����ַ
	reg_val = SPI_RW(0);//��ȡ�üĴ�����������
	NRF_CSN_H;		//��ֹNRF���� 
    return 	reg_val;
}

/******************************************************************************
����ԭ�ͣ�	uint8_t NRF_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uchars)
��    �ܣ�	NRFд������
�� �� ֵ��	NRFд����������ֵ
*******************************************************************************/
uint8_t NRF_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uchars)
{
	uint8_t i;
	uint8_t status;
	NRF_CSN_L;	//ѡͨNRF���� 
	status = SPI_RW(reg);//д�Ĵ�����ַ 
	for(i=0; i<uchars; i++)
	{
		SPI_RW(pBuf[i]);//д���� 
	}
	NRF_CSN_H;	//��ֹNRF����
    return 	status;	
}

/******************************************************************************
����ԭ�ͣ�	uint8_t NRF_Read_Buff(uint8_t reg, uint8_t *pBuf, uint8_t uchars)
��    �ܣ�	NRF��������
�� �� ֵ��	����������
*******************************************************************************/
uint8_t NRF_Read_Buff(uint8_t reg, uint8_t *pBuf, uint8_t uchars)
{
	uint8_t i;
	uint8_t status;
	NRF_CSN_L;	//ѡͨNRF���� 
	status = SPI_RW(reg);//д�Ĵ�����ַ
	for(i=0; i<uchars; i++)
	{
		pBuf[i] = SPI_RW(0);//��ȡ��������	
	}
	NRF_CSN_H;	//��ֹNRF����
    return 	status;
}

/******************************************************************************
����ԭ�ͣ�	static void NRF24L01_Set_TX(void)
��    �ܣ�	��NRF24L01����Ϊ����ģʽ
*******************************************************************************/
static void NRF24L01_Set_TX(void)
{
	NRF_CE_L;
	NRF_Write_Reg(NRF_WRITE_REG + CONFIG,0x0E);//����
	NRF_CE_H;
}

/******************************************************************************
����ԭ�ͣ�	static void NRF24L01_Set_RX(void)
��    �ܣ�	��NRF24L01����Ϊ����ģʽ
*******************************************************************************/
void NRF24L01_Set_RX(void)
{
	NRF_CE_L;
	NRF_Write_Reg(NRF_WRITE_REG + CONFIG,0x0F);//����
	NRF_CE_H;
}

/******************************************************************************
����ԭ�ͣ�	void NRF_Send_TX(uint8_t * tx_buf, uint8_t len)
��    �ܣ�	NRF2401�������ݰ�
*******************************************************************************/
void NRF_Send_TX(uint8_t * tx_buf, uint8_t len)
{		
	NRF24L01_Set_TX();
	NRF_CE_L;//�������ģʽ1	
	NRF_Write_Buf(WR_TX_PLOAD, tx_buf, len);//װ������
	NRF_CE_H;//����CEΪ�ߣ��������䡣CE�ߵ�ƽ����ʱ����СΪ10us
}

/******************************************************************************
����ԭ�ͣ�	void NRF24L01_Check(void)
��    �ܣ�	���NRF�����Ƿ�����
*******************************************************************************/
void NRF24L01_Check(void)
{ 
	uint8_t ok;
	uint8_t buf[5]; 
	uint8_t i; 
	//д��5���ֽڵĵ�ַ 
	NRF_Write_Buf(NRF_WRITE_REG+TX_ADDR,TX_ADDRESS,5); 
	//����д��ĵ�ַ 
	NRF_Read_Buff(TX_ADDR,buf,5); 
	//�Ƚ�
	for(i=0;i<5;i++) 
	{ 
		if(buf[i]!=TX_ADDRESS[i]) 
			break; 
	} 
	if(i==5)
	{
		ok=1;
	}
		//PrintString("\r\n NRF24L01 ��ʼ���ɹ���");
	else
	{
		ok=0;
	}
		//PrintString("\r\n NRF24L01 ��ʼ��ʧ�ܣ�");
}

/******************************************************************************
����ԭ�ͣ�	void NRF24L01_Init(uint8_t Chanal,uint8_t Mode)
��    �ܣ�	NRF24L01��ʼ��
��    ����	Chanal��RFͨ��
*******************************************************************************/
void NRF24L01_Init(uint8_t Chanal,uint8_t Mode)
{
	NRF_CE_L;
		
	NRF_Write_Reg(FLUSH_TX,0xff);//��շ��ͻ�����
	NRF_Write_Reg(FLUSH_RX,0xff);//��ս��ջ�����
	NRF_Write_Buf(NRF_WRITE_REG + TX_ADDR,   TX_ADDRESS,5); //дTX�ڵ��ַ  	
	NRF_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0,RX_ADDRESS,5);	//дRX�ڵ��ַ 

	NRF_Write_Reg(NRF_WRITE_REG + EN_AA,     0x01); //ʹ��ͨ��0���Զ�Ӧ�� 
	NRF_Write_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);	//ʹ��ͨ��0�Ľ��յ�ַ 
	NRF_Write_Reg(NRF_WRITE_REG + SETUP_RETR,0x1a);	//�����Զ��ط����ʱ��:500us;����Զ��ط�����:10�� 
	NRF_Write_Reg(NRF_WRITE_REG + RF_CH,   Chanal);	//����RFͨ��ΪCHANAL
	NRF_Write_Reg(NRF_WRITE_REG + RX_PW_P0,    16);	//����ͨ��0����Ч���ݿ��
	NRF_Write_Reg(NRF_WRITE_REG + RF_SETUP,  0x0b); //����TX�������,18db����,2Mbps,���������濪��
	
	if(Mode==TX)
		NRF_Write_Reg(NRF_WRITE_REG + CONFIG,0x0E);//����
	else if(Mode==RX)
		NRF_Write_Reg(NRF_WRITE_REG + CONFIG,0x0F);//����
	
	NRF_CE_H;
}

/******************************************************************************
����ԭ�ͣ�	void NRF24L01_IRQ(void)
��    �ܣ�	NRF24L01�ж�
*******************************************************************************/
void NRF24L01_IRQ(void)	
{
	uint8_t status = NRF_Read_Reg(NRF_READ_REG + NRFRegSTATUS);
	
	if(status & (1<<RX_DR))//�����ж�
	{	
		uint8_t rx_len = NRF_Read_Reg(R_RX_PL_WID);
		if(rx_len==16)
		{
			NRF_Read_Buff(RD_RX_PLOAD,NRF24L01_RXDATA,rx_len);//��ȡ����FIFO����
		  if(NRF24L01_RXDATA[0]=='$'&&NRF24L01_RXDATA[1]=='M'&&NRF24L01_RXDATA[2]=='>'&&NRF24L01_RXDATA[3]==18)
      {
				//duty_ref=NRF24L01_RXDATA[4]*1.5+450;
//				AUX=NRF24L01_RXDATA[5];
//				AUY=NRF24L01_RXDATA[6];
//				Lock_on=NRF24L01_RXDATA[7];
//        AUZ=NRF24L01_RXDATA[8];
				
      }

		}
			Nrf_Erro = 0;
			NRF_Write_Reg(NRF_WRITE_REG + NRFRegSTATUS, 0xff);//����жϱ�־λ				
	}
	else{
	NRF_Write_Reg(FLUSH_RX,0xff);//��ս��ջ�����
	NRF_Write_Reg(NRF_WRITE_REG + NRFRegSTATUS, 0xff);//����жϱ�־λ	
	}
}

/******************************************************************************
����ԭ�ͣ�	void Nrf_Connect(void)
��    �ܣ�	NRF24L01���Ӻ���
*******************************************************************************/
void Nrf_Connect(void)//1KHZ
{
	Nrf_Erro ++;
	if(Nrf_Erro>12)
	{
		NRF24L01_IRQ();
	}
	if(Nrf_Erro>=1000)   //0.5sδ����nrf���� ,�������ᣬ���ͣת����ֹ���ⷢ��
	{			
		Nrf_Erro = 1000;
		//duty_ref=0;
    TIM2->CCR1 = 0;
    TIM2->CCR2 = 0;
	  TIM2->CCR3 = 0;
 	  TIM2->CCR4 = 0;
	}
}
