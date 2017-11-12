#ifndef _IMU_H_
#define _IMU_H_
#include "stdbool.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_rcc.h"
extern float angle_roll;
extern float angle_pitch;

/* float结构体 */
struct _SI_float
{
	float x;
	float y;
	float z;
};
extern struct _SI_float SI_acc;	
extern struct _SI_float SI_gyro;

/* 姿态解算--角度值 */
struct _out_angle
{
	float yaw;
	float roll;
	float pitch;
};
extern struct _out_angle out_angle;
extern struct _out_angle control_angle;
extern struct _out_angle last_angle;
extern struct _out_angle control_gyro;
extern struct _out_angle last_gyro;

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);
void Get_Eulerian_Angle(struct _out_angle *angle);

#endif
