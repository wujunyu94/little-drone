#ifndef _TIMER_H__ 
#define _TIMER_H__ 
#include "sys.h"  

void TIM1_Init(u16 arr,u16 psc);
void TIM3_Int_Init(u16 arr,u16 psc);
void TIM2_PWM_Init(u16 arr,u16 psc);  
#endif
