/*********************************************************************************
 * �ļ���  ��sys_rtc.c
 * ����    ��ʵʱʱ��ģ��
 * ��汾  ��ST3.5.0
 * ��дʱ�䣺2013��12��17��
 * ��д��  ��LiuHui
 *
 *
 * �޸�ʱ�䣺2014��4��5
 * �޸����ݣ�
 *          @1����TFT��ʾʱ�亯�� uint32_t Time_Regulate(uint8_t x, uint16_t y);
 *          @2����TFT������Уʱ����
 *          @3���������ж�
 *
 * �޸�ʱ�䣺2014��4��7
 * �޸����ݣ�
 *          @ʱ����ʾ��ʱ�������Ƴ�RTC.c��NumClock.c
 *
**********************************************************************************/
#include "stm32f10x.h"
#include "sys_rtc.h"
#include "typedef.h"

volatile uint8_t b_rtcTimeDisplayFlag = 0;
volatile uint8_t b_rtcAlarmRingFlag = 0;

static void RTC_Configuration(void);
static void RTC_NVIC_Configuration(void);

void sys_rtcInit(void)
{
	RTC_Configuration();
	RTC_NVIC_Configuration();
//	/*��������*/
//	RTC_SetAlarm(0x09);
	/* Clear reset flags */
	RCC_ClearFlag();
}

/**-------------------------------------------------------
  * @������ RTC_Configuration
  * @����   RTCʵʱʱ�ӵĲ������ú���
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
static void RTC_Configuration(void)
{
	/* PWRʱ�ӣ���Դ���ƣ���BKPʱ�ӣ�RTC�󱸼Ĵ�����ʹ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	/*ʹ��RTC�ͺ󱸼Ĵ������� */
	PWR_BackupAccessCmd(ENABLE);
//	/* ������BKP��ȫ���Ĵ�������Ϊȱʡֵ */
//	BKP_DeInit();
	if(BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	{
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
#if	RTC_CLK_LSE
		/* ʹ��LSE���ⲿ32.768KHz���پ���*/
		RCC_LSEConfig(RCC_LSE_ON);
		/* �ȴ��ⲿ�������ȶ���� */
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
#elif RTC_CLK_LSI
		RCC_LSICmd(ENABLE);
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
#elif RTC_CLK_HSE128
		RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div128);
#else
#error not select RTC clock!
#endif
		/* ʹ�� RTC ��ʱ�ӹ��� */
		RCC_RTCCLKCmd(ENABLE);
		/*�ȴ�RTC�Ĵ���ͬ�� */
		RTC_WaitForSynchro();
#if	RTC_CLK_LSE
		/* 32.768KHz����Ԥ��Ƶֵ��32767,����Ծ���Ҫ��ܸ߿����޸Ĵ˷�Ƶֵ��У׼���� */
		RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
#elif RTC_CLK_LSI
		RTC_SetPrescaler(50000);
#elif RTC_CLK_HSE128
		RTC_SetPrescaler(62499);
#else
#error not select RTC clock!
#endif
		/* �ȴ���һ�ζ�RTC�Ĵ�����д������� */
		RTC_WaitForLastTask();
	}
	/*�ȴ�RTC�Ĵ���ͬ�� */
	RTC_WaitForSynchro();
	/* �ȴ���һ�ζ�RTC�Ĵ�����д������� */
	RTC_WaitForLastTask();
	/* ʹ��RTC�����ж� */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	/* �ȴ���һ�ζ�RTC�Ĵ�����д������� */
	RTC_WaitForLastTask();
	/* ʹ��RTC�������ж� */
	RTC_ITConfig(RTC_IT_ALR, ENABLE);
	/* �ȴ���һ�ζ�RTC�Ĵ�����д������� */
	RTC_WaitForLastTask();
}

/**-------------------------------------------------------
  * @������ RTC_NVIC_Configuration
  * @����   RTCʵʱʱ�ӵ��жϲ������ú���
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
static void RTC_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
/**-------------------------------------------------------
  * @������ sys_rtcSetTime
  * @����   ��ʱ��ת��ΪRTC����ֵд��RTC�Ĵ���
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void sys_rtcSetTime(uint32_t time)
{
	/* �ȴ���һ�ζ�RTC�Ĵ�����д������� */
	RTC_WaitForLastTask();
	/* ��ʱ��ת��ΪRTC����ֵд��RTC�Ĵ��� */
	RTC_SetCounter(time);
	/* �ȴ���һ�ζ�RTC�Ĵ�����д������� */
	RTC_WaitForLastTask();
}

//#define USE_UCOS_II
#ifdef USE_UCOS_II
extern void OSIntEnter(void);
extern void OSIntExit(void);
#else
#define OSIntEnter()
#define OSIntExit()
#endif

/**-------------------------------------------------------
  * @������ RTC_IRQHandler
  * @����   RTCʵʱʱ�ӵ��жϴ�����
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void RTC_IRQHandler(void)
{
	OSIntEnter();
	/*  �ж�RTC�Ƿ��������жϣ�Ҳ�п�����������������ж�) */
	if(RTC_GetITStatus(RTC_IT_SEC) != RESET)
	{
		RTC_ClearITPendingBit(RTC_IT_SEC);
		/* ��ȫ�ֱ���Ϊ1��֪ͨ�������ӡʱ�� */
		b_rtcTimeDisplayFlag = 1;
		/*RTC�Ѽ������ڣ������������RTC�Ĵ���*/
//    /* �ȴ���һ�ζ�RTC�Ĵ�����д������� */
//    RTC_WaitForLastTask();
//    /* ���ʱ��ﵽ23:59:59����һ��ʱ��Ϊ00:00:00 */
//    if (RTC_GetCounter() == 0x00015180)
//    {
//      RTC_SetCounter(0x0);
//      /* �ȴ���һ�ζ�RTC�Ĵ�����д������� */
//      RTC_WaitForLastTask();
//    }
	}
	/*�ж��Ƿ��������ж�*/
	else if(RTC_GetITStatus(RTC_IT_ALR) == SET)
	{
		RTC_ClearITPendingBit(RTC_IT_ALR);
		b_rtcAlarmRingFlag = 1;
	}
	OSIntExit();
}
