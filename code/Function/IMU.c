#include "allheader.h"
#include "math.h"


struct _SI_float  SI_acc;	//���ٶ����ݣ�m/s2��
struct _SI_float  SI_gyro;	//���ٶ����ݣ�rad��
struct _out_angle out_angle;//��̬����-�Ƕ�ֵ
struct _out_angle control_angle;
struct _out_angle last_angle;
struct _out_angle control_gyro;
struct _out_angle last_gyro;

float angle_roll;
float angle_pitch;

// ==================================================================================
// ����:
// ���붨��'halfT 'Ϊ���ڵ�һ�룬�Լ��˲����Ĳ���Kp��Ki
// ��Ԫ��'q0', 'q1', 'q2', 'q3'����Ϊȫ�ֱ���
// ��Ҫ��ÿһ���������ڵ���'IMUupdate()'����
// ���������ݵ�λ�ǻ���/�룬���ٶȼƵĵ�λ�޹���Ҫ����Ϊ�ᱻ�淶��
// ==================================================================================
#define Kp 	1.0f    // ��������
#define Ki 	0.001f  // ���ֳ���
#define halfT 0.0005f//������
#define T	0.001f  // ����Ϊ1ms
// ==================================================================================
// ��������
float q0 = 1, q1 = 0, q2 = 0, q3 = 0;     	// ��Ԫ��
float exInt = 0, eyInt = 0, ezInt = 0;    	// �������ۼ�ֵ 
float delta2=0;

// ==================================================================================
// ����ԭ�ͣ�void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az) 
// ��        �ܣ������˲�������̬����
// ��        �룺���������ݼ����ٶȼ�����
// ==================================================================================
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az) 
{
	float norm;
	float vx, vy, vz;
	float ex, ey, ez;
	
  //��Ԫ�����֣���õ�ǰ����̬
	float q0_last = q0;	
	float q1_last = q1;	
	float q2_last = q2;	
	float q3_last = q3;	

	//�Ѽ��ٶȼƵ���ά����ת�ɵ�λ����
	norm = invSqrt(ax*ax + ay*ay + az*az);
	ax = ax * norm;
	ay = ay * norm;
	az = az * norm;

	//�����������ٶȷ����ڷ���������ϵ�еı�ʾ��Ϊ��Ԫ����ʾ����ת����ĵ�����
	vx = 2*(q1*q3 - q0*q2);
	vy = 2*(q0*q1 + q2*q3);
	vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;

	//���ٶȼƶ�ȡ�ķ������������ٶȷ���Ĳ�ֵ����������˼���
	ex = ay*vz - az*vy;
	ey = az*vx - ax*vz;
	ez = ax*vy - ay*vx;

	//����ۻ���������ֳ������
	exInt = exInt + ex*Ki;
	eyInt = eyInt + ey*Ki;
	ezInt = ezInt + ez*Ki;

	//�ò���������PI����������ƫ�����������ݶ����е�ƫ����	
	gx = gx + Kp*ex + exInt;
	gy = gy + Kp*ey + eyInt;
	gz = gz + Kp*ez + ezInt;


	//һ�׽����㷨
//	q0 = q0_last + (-q1_last*gx - q2_last*gy - q3_last*gz)*halfT;
//	q1 = q1_last + ( q0_last*gx + q2_last*gz - q3_last*gy)*halfT;
//	q2 = q2_last + ( q0_last*gy - q1_last*gz + q3_last*gx)*halfT;
//	q3 = q3_last + ( q0_last*gz + q1_last*gy - q2_last*gx)*halfT; 

//	//���׽����㷨
//	delta2 = (gx*gx + gy*gy + gz*gz)*T*T;
//	q0 = q0_last*(1-delta2/8) + (-q1_last*gx - q2_last*gy - q3_last*gz)*halfT;
//	q1 = q1_last*(1-delta2/8) + ( q0_last*gx + q2_last*gz - q3_last*gy)*halfT;
//	q2 = q2_last*(1-delta2/8) + ( q0_last*gy - q1_last*gz + q3_last*gx)*halfT;
//	q3 = q3_last*(1-delta2/8) + ( q0_last*gz + q1_last*gy - q2_last*gx)*halfT;

	//���׽����㷨
	delta2 = (gx*gx + gy*gy + gz*gz)*T*T;
	q0 = q0_last*(1-delta2/8) + (-q1_last*gx - q2_last*gy - q3_last*gz)*T*(0.5 - delta2/48);
	q1 = q1_last*(1-delta2/8) + ( q0_last*gx + q2_last*gz - q3_last*gy)*T*(0.5 - delta2/48);
	q2 = q2_last*(1-delta2/8) + ( q0_last*gy - q1_last*gz + q3_last*gx)*T*(0.5 - delta2/48);
	q3 = q3_last*(1-delta2/8) + ( q0_last*gz + q1_last*gy - q2_last*gx)*T*(0.5 - delta2/48);

//	//�Ľ׽����㷨
//	float delta2 = (gx*gx + gy*gy + gz*gz)*T*T;
//	q0 = q0_last*(1 - delta2/8 + delta2*delta2/384) + (-q1_last*gx - q2_last*gy - q3_last*gz)*T*(0.5 - delta2/48);
//	q1 = q1_last*(1 - delta2/8 + delta2*delta2/384) + ( q0_last*gx + q2_last*gz - q3_last*gy)*T*(0.5 - delta2/48);
//	q2 = q2_last*(1 - delta2/8 + delta2*delta2/384) + ( q0_last*gy - q1_last*gz + q3_last*gx)*T*(0.5 - delta2/48);
//	q3 = q3_last*(1 - delta2/8 + delta2*delta2/384) + ( q0_last*gz + q1_last*gy - q2_last*gx)*T*(0.5 - delta2/48);
			
	//��Ԫ���淶��
	norm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	q0 = q0 * norm;
	q1 = q1 * norm;
	q2 = q2 * norm;
	q3 = q3 * norm;
//	if(filter_gyro.z<100&&filter_gyro.z>-100)
//	{
//	  filter_gyro.z=0;
//	}
//	else
//	{
//		filter_gyro.z=filter_gyro.z/4;
//	}
  out_angle.yaw  +=  filter_gyro.z * RawData_to_Angle * 0.001f;
}


/******************************************************************************
����ԭ�ͣ�	void Get_Eulerian_Angle(struct _out_angle *angle)
��    �ܣ�	��Ԫ��תŷ����
*******************************************************************************/ 
void Get_Eulerian_Angle(struct _out_angle *angle)
{	
	angle->pitch = -atan2(2.0f*(q0*q1 + q2*q3),q0*q0 - q1*q1 - q2*q2 + q3*q3)* Radian_to_Angle;
	//temp[0]=angle->pitch;
	angle->roll  =  asin (2.0f*(q0*q2 - q1*q3))* Radian_to_Angle;
	//temp[1]=angle->roll;
  //temp[2]=(AUX-993)/6;
	//temp[3]=(AUY-989)/6;
	//temp[2]=out_angle.yaw;
	//angle->yaw = atan2(2.0f*(q0*q3+q3*q2),1.0f-2.0f*(q2*q2+q3*q3))* Radian_to_Angle;
}
