#include "ppm.h" 
#define lock_on  1 
#define lock_off 0
int AUX,AUY,AUZ,duty_ref;
u16 time1=0;
u16 ppm_count[11];
u8 t=0;
u8 safety=1;

void ppm_Init(u16 arr,u16 psc)  
{      
		GPIO_InitTypeDef GPIO_InitStructure;	
		EXTI_InitTypeDef EXTI_InitStructure;   
	  NVIC_InitTypeDef NVIC_InitStructure;
	
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);    //??TIM2??             
  
    TIM_TimeBaseStructure.TIM_Period = arr; //??????????   
    TIM_TimeBaseStructure.TIM_Prescaler =psc;   //????     
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //??????:TDTS = Tck_tim  
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM??????  
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //??TIM_TimeBaseInitStruct?????????TIMx???????       
      
    TIM_Cmd(TIM4,ENABLE );    //?????2  
   
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);   //GPIOE.2 ????????????,?????    
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource12);
    EXTI_InitStructure.EXTI_Line=EXTI_Line12;    
	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     
	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //????????  
	  EXTI_InitStructure.EXTI_LineCmd = ENABLE;    
	  EXTI_Init(&EXTI_InitStructure);
		EXTI_GenerateSWInterrupt(EXTI_Line12);
	
	  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;     //??????????    
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //????? 2,     
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;   //???? 2    
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    //???????? 
	  NVIC_Init(&NVIC_InitStructure);
}

void EXTI15_10_IRQHandler(void) 
{                           
  if(EXTI_GetITStatus(EXTI_Line12)!=RESET)//?????????????     
	{ 
	 if(PAin(12)==1)
	 {
		 //GPIOA->BSRR = GPIO_Pin_3;
		 TIM_SetCounter(TIM4,0);
		 //time1=0;
	 }
	 else
	 {
		 //GPIOA->BRR = GPIO_Pin_3;
     time1=TIM_GetCounter(TIM4);
		 if(time1>5000)
		 {
			 t=0;
			 duty_ref=ppm_count[3]-450;//586-1412
			 deadarea_if();
			 saftycontrol();
		 }
		 ppm_count[t]=time1;
		 t++;
	 }		     
	}
 	 EXTI_ClearITPendingBit(EXTI_Line12);	
}

void saftycontrol(void)
{
	if(ppm_count[5]>900)//1414_998_582
	{
    safety=lock_on;
	}
	else if(ppm_count[5]<600&&ppm_count[5]>500)
	{
    safety=lock_off;
	}
}

void deadarea_if(void)
{
	if(ppm_count[1]<998&&ppm_count[1]>992)
			 {
				 AUX=993;
			 }
			 else
			 {
				 AUX=ppm_count[1];//580_993_1408
			 }
			 if(ppm_count[2]<993&&ppm_count[2]>985)
			 {
				 AUY=989;
			 }
			 else
			 {
         AUY=ppm_count[2];//579_989_1404
			 }
			 if(ppm_count[4]<1000&&ppm_count[4]>992)
			 {
				 AUZ=996;
			 }
			 else
			 {			 
			   AUZ=ppm_count[4];//584_996_1404
			 }
}
