#include "allheader.h"
int16_t TEMP[2];
extern u32 time;
extern u8 flag_usart=0;
/******************************************************************************
函数原型：	void Task_1000HZ(void)
功    能：	主循环中运行频率为1000HZ任务
*******************************************************************************/ 
void Task_1000HZ(void)
{
      MPU6050_SequenceRead();
	    MPU6050_Compose();
	    
		  ACC_IIR_Filter(&acc,&filter_acc);//对acc做IIR滤波 
			Gyro_Filter(&gyro,&filter_gyro);//对gyro做窗口滤波

	    //temp[4]=filter_gyro.x;
	    //temp[5]=filter_gyro.y;
			Get_Radian(&filter_gyro,&SI_gyro);//角速度数据转为弧度
			IMUupdate(SI_gyro.x,SI_gyro.y,SI_gyro.z,filter_acc.x,filter_acc.y,filter_acc.z);//姿态解算
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
函数原型：	void Task_500HZ(void)
功    能：	主循环中运行频率为500HZ任务
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
函数原型：	void Task_250HZ(void)
功    能：	主循环中运行频率为250HZ任务
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
函数原型：	void Task_20HZ(void)
功    能：	主循环中运行频率为20HZ任务
*******************************************************************************/ 
void Task_20HZ(void)
{
    GPIOB->BSRR = GPIO_Pin_3;
	  Voltage_Measure();
	  delay_us(20);
		GPIOB->BRR = GPIO_Pin_3;
}

/******************************************************************************
函数原型：	void Task_CNT(void)
功    能：	任务周期计数
*******************************************************************************/ 
void Task_CNT(void)
{
	cnt.ms50++;
  cnt.ms500++;
  cnt.ms2++;
  cnt.ms4++;	
}

/******************************************************************************
函数原型：	void Led_Blink(void)
功    能：	任务周期计数
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
