#ifndef __SYS_WATCHDOG_H__
#define __SYS_WATCHDOG_H__


#define USE_IWDG										//����ʹ�ܶ������Ź�
#define IWDG_Prescale				0x04		//�������Ź�Ԥ��Ƶ����  ��Ƶ��=1<<(IWDG_Prescale+2)
#define IWDG_Reset_Time			100		//�������Ź���ʱʱ��ms


void sys_watchDogInit(void);
void sys_watchDogReset(void);

#endif
