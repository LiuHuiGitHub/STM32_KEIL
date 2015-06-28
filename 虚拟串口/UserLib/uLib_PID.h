#ifndef __USER_LIB_PID_H__
#define __USER_LIB_PID_H__

#ifndef __TYPEDEF_H__
typedef unsigned char       UINT8;
typedef unsigned short      UINT16;
typedef unsigned int        UINT32;
#endif


typedef struct
{
	float Kp, Ki, Kd;
	float Err, ErrLast;		//ƫ��
	float NowValue;			//��ǰ����ֵ
	float value;            //Ŀ��ֵ
	float Integral;			//����
}PID_STRUCT;

extern PID_STRUCT PID;

void uLib_PID_Init(void);
float uLib_PID_Cal(float HopeValue);

#endif
