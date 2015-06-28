/*********************************************************************************
 * �ļ���  ��sys_rtc.h
 * ����    ��ʵʱʱ��ģ��
 * ��汾  ��ST3.5.0
 * ��дʱ�䣺2013��12��17��
 * ��д��  ��LiuHui
 *
 *
 *ʹ��ʾ��
	if (TimeDisplay == 1){
		time=RTC_GetCounter();
		sprintf(ch,"Time Is:%2d:%2d:%2d",(time/3600)%24,(time%3600)/60,(time%3600)%60);
		drv_lcdShowString(20,20,(uint8_t*)ch,Color[i]);
		TimeDisplay = 0;
	}
**********************************************************************************/
#ifndef __SYS_RTC_H__
#define __SYS_RTC_H__

#define RTC_CLK_HSE128						0			//�ⲿ���پ���128��Ƶ
#define RTC_CLK_LSE							1			//�ⲿ32.768k����
#define RTC_CLK_LSI							0			//�ڲ�40k����

extern volatile u8 b_rtcTimeDisplayFlag;	//1sʱ�䵽���־
extern volatile u8 b_rtcAlarmRingFlag;//����ʱ�䵱��־

extern void sys_rtcInit(void);
extern void sys_rtcSetTime(uint32_t time);
extern void sys_rtcSetAlarm(uint32_t AlarmValue);		//��������ʱ��

#endif
