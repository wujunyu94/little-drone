#include "timer.h"  
#include "sys.h"  
#include "usart.h"  

void TIM2_PWM_Init(u16 arr,u16 psc)  
{    
    GPIO_InitTypeDef GPIO_InitStructure;  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;  
    TIM_OCInitTypeDef  TIM_OCInitStructure;  
  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//   
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);  
                                                                              
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3; //TIM_CH2  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
  
      
    TIM_TimeBaseStructure.TIM_Period = arr;   //???????????????????????????80K  
    TIM_TimeBaseStructure.TIM_Prescaler =psc; //??????TIMx??????????????  
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //??????:TDTS = Tck_tim  
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM??????  
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //??TIM_TimeBaseInitStruct?????????TIMx???????  
  
   
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //???????:TIM????????2  
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //??????  
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高

    TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 OC1
	  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCR2上的预装载寄存器
		
		TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 OC1
	  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCR2上的预装载寄存器
		
		TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 OC1
	  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCR2上的预装载寄存器
		
		TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 OC1
	  TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCR2上的预装载寄存器
      
    TIM_Cmd(TIM2, ENABLE);  //??TIM1      
}  

void TIM1_Init(u16 arr,u16 psc)  
{      
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);    //??TIM2??             
           
    TIM_TimeBaseStructure.TIM_Period = arr; //??????????   
    TIM_TimeBaseStructure.TIM_Prescaler =psc;   //????     
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //??????:TDTS = Tck_tim  
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM??????  
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //??TIM_TimeBaseInitStruct?????????TIMx???????       
      
    TIM_Cmd(TIM1,ENABLE );    //?????2  
   
}

void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //????
	
	TIM_TimeBaseStructure.TIM_Period = arr; //???????????????????????????	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //??????TIMx???????????
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //??????:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM??????
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //??????????TIMx???????
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //?????TIM3??,??????
 
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3??
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //?????0?
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //????3?
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ?????
	NVIC_Init(&NVIC_InitStructure);  //???NVIC???
 
	TIM_Cmd(TIM3, ENABLE);  //??TIMx					 
}
