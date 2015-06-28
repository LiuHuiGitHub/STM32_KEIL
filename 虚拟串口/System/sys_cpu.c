#include "stm32f10x.h"
#include "sys_watchDog.h"
#include "sys.h"


#define USE_IWDG										//����ʹ�ܶ������Ź�
#define IWDG_Prescale				0x04		//�������Ź�Ԥ��Ƶ����  ��Ƶ��=1<<(IWDG_Prescale+2)
#define IWDG_Reset_Time			100		//�������Ź���ʱʱ��ms

#ifdef SYS_TASK_WATCHDOG
void sys_taskWatchDogInit(void)
{
	RCC_LSICmd(ENABLE);
	while(!RCC_GetFlagStatus(RCC_FLAG_LSIRDY));
	IWDG->KR = 0x5555;
	IWDG->PR |= IWDG_Prescale;
	IWDG->RLR = IWDG_Reset_Time * 40 / (1 << (IWDG_Prescale + 2));
	IWDG->KR = 0xCCCC;
	IWDG->KR = 0xAAAA;
}
void sys_taskWatchDog(void)
{
	IWDG->KR = 0xAAAA;
}
#endif

#ifdef SYS_TASK_SLEEP
void sys_taskSleep(void)
{
	PWR_EnterSTANDBYMode();
}
#endif

void sys_timeInit(void)
{
}


//void sys_timeHandler(void) interrupt 5  //20msʱ���׼
//{
//	TMR2CN &= 0x3F;
//	RCAP2H = 0x70;
//	RCAP2L = 0x00;
//	sys_clockScheduler();
//}


//UINT16 sys_timeTick(void)
//{
//	return (RCAP2H << 8 + RCAP2L - 0x7000);
//}

