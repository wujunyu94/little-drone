#include "allheader.h"

struct _cnt  cnt;
int16_t temp[6];
int main(void)
{	
                                                                                                                                                                                                                   	SystemInit();			//ϵͳ��ʼ��
	delay_init(72);	     	//��ʱ��ʼ��
	delay_ms(2000);
	NVIC_Configuration(); 	//�жϳ�ʼ��
	LED_Init();
 	uart_init(115200);		//���ڳ�ʼ��
	TIM2_PWM_Init(999,3);
	TIM1_Init(0xFFFF,720-1);
	ppm_Init(0xFFFF,72-1);
	I2C_Int();
	InitMPU6050();
	TIM3_Int_Init(999,71);//1KHZ�ж�
	ADC1_Init();
	
	Calculate_FilteringCoefficient(0.001f,6.f);//����IIR�˲�������
	while(1)
	{	
    if(flag_usart)
		{
			flag_usart=0;
			vcan_sendware(temp,sizeof(temp));
		}
	}
}

void TIM3_IRQHandler(void)   //TIM3??
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //??TIM3????????
		{
		  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //??TIMx?????? 
      Task_CNT();			
      Task_1000HZ();
			if(cnt.ms2==2)
			{
				cnt.ms2=0;
				Task_500HZ();
			}
			if(cnt.ms4==4)
			{
				cnt.ms4=0;
				Task_250HZ();
			}
			if(cnt.ms50==50)
			{
				cnt.ms50=0;				
				Task_20HZ();
			}
      Led_Blink();
		}
}
