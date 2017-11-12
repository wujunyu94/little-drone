#ifndef __PPM_H__ 
#define __PPM_H__ 
#include "sys.h"  
extern u8 safety;

void ppm_Init(u16 arr,u16 psc);  
void saftycontrol(void);
void deadarea_if(void);

extern int AUX,AUY,AUZ,duty_ref;

#endif
