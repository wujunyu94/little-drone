#ifndef _CONTROL_H_
#define _CONTROL_H_
#include "stm32f10x.h"
#include "allheader.h"

struct _pid
{
	float kp;
	float ki;
	float kd;
	float integral;
	float dif;
	float output;
};
extern struct _pid yaw;
extern struct _pid pitch;
extern struct _pid roll;
extern struct _pid gyro_pitch;
extern struct _pid gyro_roll;
extern struct _pid gyro_yaw;


/******************************************************************************
							全局变量声明
*******************************************************************************/ 
extern uint8_t Rc_Lock;//1上锁；0解锁
extern int16_t throttle1,throttle2,throttle3,throttle4;
extern uint8_t Lock_on,Last_Lock_on;
/******************************************************************************
							全局函数声明
*******************************************************************************/ 
void pid_wjy_inside(struct _SI_float *gyro);
void pid_wjy_outside(struct _out_angle *angle);
#endif
