#include "timer.h"  
#include "sys.h"  
#include "usart.h" 
#include "echo.h" 
u8 TIM2CH1_CAPTURE_STA=0; //??????    
u32 TIM2CH1_CAPTURE_VAL;//????? 
u32 time;
void TRIG_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;	
		EXTI_InitTypeDef EXTI_InitStructure;   
	  NVIC_InitTypeDef NVIC_InitStructure;
	
	  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3; //PA0 ??????    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //PA0 ??   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);   //GPIOE.2 ????????????,?????    
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);
    EXTI_InitStructure.EXTI_Line=EXTI_Line4;    
	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     
	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //????????  
	  EXTI_InitStructure.EXTI_LineCmd = ENABLE;    
	  EXTI_Init(&EXTI_InitStructure);
		EXTI_GenerateSWInterrupt(EXTI_Line4);
	
	  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;     //??????????    
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //????? 2,     
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;   //???? 2    
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    //???????? 
	  NVIC_Init(&NVIC_InitStructure);
}

void EXTI4_IRQHandler(void) 
{                           
  if(EXTI_GetITStatus(EXTI_Line4)!=RESET)//?????????????     
	{ 
	 if(PBin(4)==1)
	 {
		 //GPIOA->BSRR = GPIO_Pin_3;
		 TIM_SetCounter(TIM1,0);
		 time=0;
	 }
	 else
	 {
		 //GPIOA->BRR = GPIO_Pin_3;
     time=TIM_GetCounter(TIM1);		 
	 }		     
	}
 	 EXTI_ClearITPendingBit(EXTI_Line4);	
}
