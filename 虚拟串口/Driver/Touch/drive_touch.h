/******************** (C) COPYRIGHT 2013 www.armjishu.com  ********************
 * �ļ���  ��drive_touch.h
 * ����    ��ʵ��STM32F107VC����IV�ſ�����Ĵ��������ܺ���
 * ʵ��ƽ̨��STM32���ۿ�����
 * ��׼��  ��STM32F10x_StdPeriph_Driver V3.5.0
 * ����    ��www.armjishu.com
*******************************************************************************/

#ifndef __DRIVE_TOUCH_H__
#define __DRIVE_TOUCH_H__

/* Includes ------------------------------------------------------------------*/

/* ����״̬	*/
#define Pen_Down 0x01
#define Pen_Up   0x00
/* �ʸ˽ṹ�� */
typedef struct
{
	u16 X0;//ԭʼ����
	u16 Y0;
	u16 X; //����/�ݴ�����
	u16 Y;
	u8  Key_Sta;//�ʵ�״̬
	//������У׼����
	float xfac;
	float yfac;
	short xoff;
	short yoff;
} Pen_Holder;

extern Pen_Holder Pen_Point;


void drv_touchInit(void);
void drv_touchAdjust(void);
void drv_touchReadCoord(void);
void drv_touchHandler(void);

#endif

/*=====================================================================*/
