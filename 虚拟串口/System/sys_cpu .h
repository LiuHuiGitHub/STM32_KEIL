#include "C8051F040.h"

void sys_taskWatchDogInit(void)
{
	SFRPAGE = 0x00;
	RSTSRC |= 1 << 2;           //ʹ��ʱ�Ӷ�ʧ���
	WDTCN = 0xA5;               //ʹ�ܿ��Ź���ʱ��
}
void sys_taskWatchDog(void)
{
	char SFRPAGE_SAVE = SFRPAGE;
	SFRPAGE = 0x00;
	WDTCN = 0xA5;       //ι��  ��ʱʱ��47ms
	SFRPAGE = SFRPAGE_SAVE;
}
