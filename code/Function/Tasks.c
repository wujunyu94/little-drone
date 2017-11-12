#include "allheader.h"
int16_t TEMP[2];
extern u32 time;
extern u8 flag_usart=0;
/******************************************************************************
����ԭ�ͣ�	void Task_1000HZ(void)
��    �ܣ�	��ѭ��������Ƶ��Ϊ1000HZ����
*******************************************************************************/ 
void Task_1000HZ(void)
{
      MPU6050_SequenceRead();
	    MPU6050_Compose();
	    
		  ACC_IIR_Filter(&acc,&filter_acc);//��acc��IIR�˲� 
			Gyro_Filter(&gyro,&filter_gyro);//��gyro�������˲�

	    //temp[4]=filter_gyro.x;
	    //temp[5]=filter_gyro.y;
			Get_Radian(&filter_gyro,&SI_gyro);//���ٶ�����תΪ����
			IMUupdate(SI_gyro.x,SI_gyro.y,SI_gyro.z,filter_acc.x,filter_acc.y,filter_acc.z);//��̬����
			//Get_Eulerian_Angle(&out_angle);
	    if(safety)
			{
				TIM2->CCR1 = 0;
	      TIM2->CCR2 = 0;
	      TIM2->CCR3 = 0;
	      TIM2->CCR4 = 0;
			}

}

/******************************************************************************
����ԭ�ͣ�	void Task_500HZ(void)
��    �ܣ�	��ѭ��������Ƶ��Ϊ500HZ����
*******************************************************************************/ 
void Task_500HZ(void)
{
	if(!safety){
	pid_wjy_inside(&SI_gyro);
	}
	temp[0]=filter_acc.x;
	temp[1]=filter_acc.y;
	temp[2]=filter_acc.z;
	temp[3]=filter_gyro.x;
	temp[4]=filter_gyro.y;
	temp[5]=filter_gyro.z;

}

/******************************************************************************
����ԭ�ͣ�	void Task_250HZ(void)
��    �ܣ�	��ѭ��������Ƶ��Ϊ250HZ����
*******************************************************************************/ 
void Task_250HZ(void)
{
	if(!safety){
	Get_Eulerian_Angle(&out_angle);
	pid_wjy_outside(&out_angle);
	flag_usart=1;
	}
}

/******************************************************************************
����ԭ�ͣ�	void Task_20HZ(void)
��    �ܣ�	��ѭ��������Ƶ��Ϊ20HZ����
*******************************************************************************/ 
void Task_20HZ(void)
{
    GPIOB->BSRR = GPIO_Pin_3;
	  Voltage_Measure();
	  delay_us(20);
		GPIOB->BRR = GPIO_Pin_3;
}

/******************************************************************************
����ԭ�ͣ�	void Task_CNT(void)
��    �ܣ�	�������ڼ���
*******************************************************************************/ 
void Task_CNT(void)
{
	cnt.ms50++;
  cnt.ms500++;
  cnt.ms2++;
  cnt.ms4++;	
}

/******************************************************************************
����ԭ�ͣ�	void Led_Blink(void)
��    �ܣ�	�������ڼ���
*******************************************************************************/ 
void Led_Blink(void)
{
	  if(cnt.ms500==250)
	  {
			cnt.ms500=0;
			if(Battery<380)
			{
				PBout(2)=1;
			}
			else if(safety)
	    {	    
		    PBout(2)=!PBout(2);
			}
			else
	    {
		    PBout(2)=0;
	    }
			
	  }
}
