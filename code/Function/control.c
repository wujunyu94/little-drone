#include "allheader.h"

int16_t throttle1,throttle2,throttle3,throttle4;
int16_t last_throttle1,last_throttle2,last_throttle3,last_throttle4;
uint8_t Lock_on,Last_Lock_on;

struct _pid yaw;
struct _pid pitch;
struct _pid roll;
struct _pid gyro_pitch;
struct _pid gyro_roll;
struct _pid gyro_yaw;


#define angle_max 	 		15.0f	
#define angle_integral_max  4000.0f	
#define gyro_max 	 		50.0f	
#define gyro_integral_max 	5000.0f
/******************************************************************************
函数原型：	void pid_wjy_outside(struct _out_angle *angle)
功    能：	PID控制器(外环)
*******************************************************************************/ 
void pid_wjy_outside(struct _out_angle *angle)
{
	//control_angle.yaw = out_angle.yaw;
	control_angle.roll  =  (AUX-993)/6 + angle->roll;
	control_angle.pitch =  (AUY-989)/6 + angle->pitch;	
	
	if(control_angle.roll >  angle_max)	//ROLL
		roll.integral +=  angle_max;
	if(control_angle.roll < -angle_max)
		roll.integral += -angle_max;
	else
		roll.integral += control_angle.roll;
	
	if(roll.integral >  angle_integral_max)
	   roll.integral =  angle_integral_max;
	if(roll.integral < -angle_integral_max)
	   roll.integral = -angle_integral_max;
	
	if(control_angle.pitch >  angle_max)//PITCH
	   pitch.integral +=  angle_max;
	if(control_angle.pitch < -angle_max)
	   pitch.integral += -angle_max;
	else
		pitch.integral += control_angle.pitch;

	if(pitch.integral >  angle_integral_max)
	   pitch.integral =  angle_integral_max;
	if(pitch.integral < -angle_integral_max)
	   pitch.integral = -angle_integral_max;
	
	roll.dif  = control_angle.roll -last_angle.roll;
	pitch.dif = control_angle.pitch-last_angle.pitch;	

//   if(control_angle.yaw >  angle_max)//yaw
// 	   yaw.integral +=  angle_max;
// 	if(control_angle.yaw < -angle_max)
// 	   yaw.integral += -angle_max;
// 	else
// 		yaw.integral += control_angle.yaw;

// 	if(yaw.integral >  angle_integral_max)
// 	   yaw.integral =  angle_integral_max;
// 	if(yaw.integral < -angle_integral_max)
// 	   yaw.integral = -angle_integral_max;
	
	if(duty_ref<100)//油门较小时，积分清零
	{
		roll.integral  = 0;
		pitch.integral = 0;
		//yaw.integral = 0;
	}
//	temp[3] =(AUZ-996)/4;
//  temp[4] = roll.integral;
//	temp[5] = pitch.integral;
	
	roll.output  = 2.75f *control_angle.roll  + 0.025f *roll.integral  + 2.5f * roll.dif;//+ 0.02f *roll.integral     - 0.05f *roll.integral 
	pitch.output = 2.75f *control_angle.pitch + 0.025f *pitch.integral + 2.5f * pitch.dif;//+ 0.02f *pitch.integral  - 0.05f *pitch.integral
	
	last_angle.roll =control_angle.roll;
	last_angle.pitch=control_angle.pitch;	
}

/******************************************************************************
函数原型：	void pid_wjy_inside(stru4ct _SI_float *gyro)
功    能：	PID控制器(内环)
*******************************************************************************/ 
void pid_wjy_inside(struct _SI_float *gyro)
{
	control_gyro.roll  = roll.output + gyro->y*Radian_to_Angle;
	control_gyro.pitch = pitch.output - gyro->x*Radian_to_Angle;
	control_gyro.yaw   = -gyro->z*Radian_to_Angle - (AUZ-996)/4; 
	//control_gyro.yaw   = - gyro->z*Radian_to_Angle;
	
//	gyro_yaw.integral += control_gyro.yaw;
//	
//	if(gyro_yaw.integral >  gyro_integral_max)
//	   gyro_yaw.integral =  gyro_integral_max;
//	if(gyro_yaw.integral < -gyro_integral_max)
//	   gyro_yaw.integral = -gyro_integral_max;
	
		gyro_roll.output  = 2.5f * control_gyro.roll  + 2.5f  * (control_gyro.roll  - last_gyro.roll );//0.2
		gyro_pitch.output = 2.5f * control_gyro.pitch + 2.5f  * (control_gyro.pitch - last_gyro.pitch);//0.2
		gyro_yaw.output   = 6.0f * control_gyro.yaw   + 5.5f  * (control_gyro.yaw   - last_gyro.yaw  );//1.5
	
	  last_gyro.roll =control_gyro.roll;
	  last_gyro.pitch=control_gyro.pitch;
	  last_gyro.yaw  =control_gyro.yaw;
	
		throttle1=duty_ref + gyro_roll.output - gyro_pitch.output + gyro_yaw.output;
		throttle2=duty_ref - gyro_roll.output - gyro_pitch.output - gyro_yaw.output;
		throttle3=duty_ref + gyro_roll.output + gyro_pitch.output - gyro_yaw.output;
		throttle4=duty_ref - gyro_roll.output + gyro_pitch.output + gyro_yaw.output;
		
    throttle1=0.3*last_throttle1+0.7*throttle1;
		throttle2=0.3*last_throttle2+0.7*throttle2;
		throttle3=0.3*last_throttle3+0.7*throttle3;
		throttle4=0.3*last_throttle4+0.7*throttle4;
	
			if(throttle1>985)	throttle1=985;//950为电机pwm最大值          
	    if(throttle1<50)		throttle1=50;
	    if(throttle2>985)	throttle2=985;
	    if(throttle2<50)		throttle2=50;
	    if(throttle3>985)	throttle3=985;
	    if(throttle3<50)		throttle3=50;
	    if(throttle4>985)	throttle4=985;
	    if(throttle4<50)		throttle4=50;	
			
//			  temp[2] = throttle1; 
//    	  temp[3] = throttle2;
//	      temp[4] = throttle3;
//				temp[5] = throttle4;
			
        TIM2->CCR1 = throttle1; 
    	  TIM2->CCR2 = throttle2;
	      TIM2->CCR3 = throttle3;
				TIM2->CCR4 = throttle4;


		last_throttle1=throttle1;
		last_throttle1=throttle1;
		last_throttle1=throttle1;
		last_throttle1=throttle1;
}
