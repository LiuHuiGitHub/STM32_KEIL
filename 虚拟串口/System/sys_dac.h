#ifndef __SYS_DAC_H__
#define __SYS_DAC_H__

/*DAC Channel1���
  ��СϵͳVREF+���������ӵ�3.3V
	for(i=0;i<3.14;i+=0.01)
	{
		sys_dacOutput((uint16_t)(1000*sin(i)));
		Delay_us(100);
	}
*/
void sys_dacInit(void);
void sys_dacOutput(uint16_t value);

#endif
