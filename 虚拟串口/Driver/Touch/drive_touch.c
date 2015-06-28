/******************** (C) COPYRIGHT 2013 www.armjishu.com  ********************
 * �ļ���  ��SZ_TouchScreen.c
 * ����    ��ʵ��STM32F107VC����IV�ſ�����Ĵ��������ܺ���
 * ʵ��ƽ̨��STM32���ۿ�����
 * ��׼��  ��STM32F10x_StdPeriph_Driver V3.5.0
 * ����    ��www.armjishu.com
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "typedef.h"
#include "drive_lcd.h"
#include "drive_touch.h"
#include "stdlib.h"
#include "math.h"

Pen_Holder Pen_Point;	/* �����ʵ�� */

extern void Delay_ms(uint16_t xms);
extern void Delay_us(uint16_t xus);

/* ADS7843/7846/UH7843/7846/XPT2046/TSC2046 ָ� */
#define CMD_RDY 0X90  //0B10010000���ò�ַ�ʽ��X����
#define CMD_RDX	0XD0  //0B11010000���ò�ַ�ʽ��Y����  


/* SPI3 Remap pins definition for test */
#define TS_SPI_PORT                 GPIOB
#define TS_SPI_SCK                  GPIO_Pin_13
#define TS_SPI_MISO                 GPIO_Pin_14
#define TS_SPI_MOSI                 GPIO_Pin_15
#define RCC_TS_SPI                  RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO
#define TS_SPI                      SPI2

/* TouchScreen_CS */
#define TS_CS_PIN_NUM               12 /* bitband������ʹ�ú궨��  */
#define TS_CS_PIN                   GPIO_Pin_12
#define TS_CS_GPIO_PORT             GPIOB
#define TS_CS_GPIO_CLK              RCC_APB2Periph_GPIOB
//#define TS_CS_OBB                   Periph_BB((uint32_t)&TS_CS_GPIO_PORT->ODR, TS_CS_PIN_NUM)
/* Select TouchScreen SPI:   ChipSelect pin low  */
#define TS_CS_LOW()                 PBout(12) = 0
/* Deselect TouchScreen SPI: ChipSelect pin high */
#define TS_CS_HIGH()                PBout(12) = 1


/* TouchScreen_INT */
#define TS_INT_PIN_NUM              12
#define TS_INT_PIN                  GPIO_Pin_12
#define TS_INT_GPIO_PORT            GPIOD
#define TS_INT_GPIO_CLK             RCC_APB2Periph_GPIOD
#define TS_INT_EXTI_LINE            EXTI_Line12
#define TS_INT_EXTI_PORT_SOURCE     GPIO_PortSourceGPIOD
#define TS_INT_EXTI_PIN_SOURCE      GPIO_PinSource12
#define TS_INT_EXTI_IRQn            EXTI15_10_IRQn
//#define TS_INT_IBB                  Periph_BB((uint32_t)&TS_INT_GPIO_PORT->IDR, TS_INT_PIN_NUM) //�ȼ���Periph_BB((uint32_t)&GPIOC->IDR, 4)
//#define TS_INT_VALID                (!PDout(12))
#define PEN  PDin(12)   //PC4 INT

void drv_touchGPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Enable TouchScreen_CS clock */
	RCC_APB2PeriphClockCmd(TS_CS_GPIO_CLK , ENABLE);
	/* TouchScreen_CS pins configuration */
	GPIO_InitStructure.GPIO_Pin = TS_CS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(TS_CS_GPIO_PORT, &GPIO_InitStructure);
	/* Enable TouchScreen_INT clock */
	RCC_APB2PeriphClockCmd(TS_INT_GPIO_CLK , ENABLE);
	/* TouchScreen_INT pins configuration */
	GPIO_InitStructure.GPIO_Pin = TS_INT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(TS_INT_GPIO_PORT, &GPIO_InitStructure);
}

static void drv_touchINTConfig(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Connect Button EXTI Line to Button GPIO Pin */
	GPIO_EXTILineConfig(TS_INT_EXTI_PORT_SOURCE, TS_INT_EXTI_PIN_SOURCE);
	/* Configure Button EXTI line */
	EXTI_InitStructure.EXTI_Line = TS_INT_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	/* Enable the EXTI5 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TS_INT_EXTI_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void drv_touchSPIConfig(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef   SPI_InitStructure;
	/* Enable TouchScreen SPI clock */
	RCC_APB2PeriphClockCmd(RCC_TS_SPI, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	/* Configure TouchScreen SPI pins: SCK and MOSI  */
	GPIO_InitStructure.GPIO_Pin = TS_SPI_SCK | TS_SPI_MOSI;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //�����������
	GPIO_Init(TS_SPI_PORT, &GPIO_InitStructure);
	/* Configure TouchScreen SPI pins: MISO */
	GPIO_InitStructure.GPIO_Pin = TS_SPI_MISO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;     //������������������
	GPIO_Init(TS_SPI_PORT, &GPIO_InitStructure);
	/* Configure TouchScreen SPI */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS  = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(TS_SPI, &SPI_InitStructure);
	/* enable TouchScreen SPI */
	SPI_Cmd(TS_SPI, ENABLE);
}

/*=====================================================================*/
unsigned char drv_touchReadWriteByte(unsigned char data)
{
	unsigned char Data = 0;
	//Wait until the transmit buffer is empty
	while(SPI_I2S_GetFlagStatus(TS_SPI, SPI_I2S_FLAG_TXE) == RESET);
	// Send the byte
	SPI_I2S_SendData(TS_SPI, data);
	//Wait until a data is received
	while(SPI_I2S_GetFlagStatus(TS_SPI, SPI_I2S_FLAG_RXNE) == RESET);
	// Get the received data
	Data = SPI_I2S_ReceiveData(TS_SPI);
	// Return the shifted data
	return Data;
}

/*=====================================================================*/
uint16_t ADS_Read_AD(uint8_t cmd)//TS_ReadX(void)
{
	uint16_t adValue = 0;
	TS_CS_LOW();
	drv_touchReadWriteByte(cmd);
	adValue = drv_touchReadWriteByte(0x00);
	adValue <<= 8;
	adValue += drv_touchReadWriteByte(0x00);
	TS_CS_HIGH();
	adValue = adValue >> 3;  //�Ҷ���
	return (adValue);
}

//��ȡһ������ֵ
//������ȡREAD_TIMES������,����Щ������������,
//Ȼ��ȥ����ͺ����LOST_VAL����,ȡƽ��ֵ
#define READ_TIMES 20 //��ȡ����
#define LOST_VAL 6	  //����ֵ
uint16_t ADS_Read_XY(uint8_t xy)
{
	uint16_t i, j;
	uint16_t buf[READ_TIMES];
	uint16_t sum = 0;
	uint16_t temp;
	for(i = 0; i < READ_TIMES; i++)
	{
		buf[i] = ADS_Read_AD(xy);
	}
	for(i = 0; i < READ_TIMES - 1; i++) //����
	{
		for(j = i + 1; j < READ_TIMES; j++)
		{
			if(buf[i] > buf[j]) //��������
			{
				temp = buf[i];
				buf[i] = buf[j];
				buf[j] = temp;
			}
		}
	}
	sum = 0;
	for(i = LOST_VAL; i < READ_TIMES - LOST_VAL; i++) { sum += buf[i]; }
	temp = sum / (READ_TIMES - 2 * LOST_VAL);
	return temp;
}
//���˲��������ȡ
//��Сֵ��������100.
uint8_t Read_ADS(uint16_t *x, uint16_t *y)
{
	uint16_t xtemp, ytemp;
	xtemp = ADS_Read_XY(CMD_RDX);
	ytemp = ADS_Read_XY(CMD_RDY);
	if(xtemp < 100 || ytemp < 100) { return 0; } //����ʧ��
	*x = xtemp;
	*y = ytemp;
	return 1;//�����ɹ�
}
//2�ζ�ȡADS7846,������ȡ2����Ч��ADֵ,�������ε�ƫ��ܳ���
//50,��������,����Ϊ������ȷ,�����������.
//�ú����ܴ�����׼ȷ��
#define ERR_RANGE 50 //��Χ 
uint8_t Read_ADS2(uint16_t *x, uint16_t *y)
{
	uint16_t x1, y1;
	uint16_t x2, y2;
	uint8_t flag;
	flag = Read_ADS(&x1, &y1);
	if(flag == 0) { return(0); }
	flag = Read_ADS(&x2, &y2);
	if(flag == 0) { return(0); }
	if(((x2 <= x1 && x1 < x2 + ERR_RANGE) || (x1 <= x2 && x2 < x1 + ERR_RANGE)) //ǰ�����β�����+-50��
	    && ((y2 <= y1 && y1 < y2 + ERR_RANGE) || (y1 <= y2 && y2 < y1 + ERR_RANGE)))
	{
		*x = (x1 + x2) / 2;
		*y = (y1 + y2) / 2;
		return 1;
	}
	else { return 0; }
}


//PEN�ж�����
void Pen_Int_Set(uint8_t en)
{
	if(en) { EXTI->IMR |= 1 << 12; } //����line4�ϵ��ж�
	else { EXTI->IMR &= ~(1 << 12); } //�ر�line4�ϵ��ж�
}

//��ȡһ������ֵ
//������ȡһ��,֪��PEN�ɿ��ŷ���!
uint8_t Read_TP_Once(void)
{
	uint8_t t = 0;
	Pen_Int_Set(0);//�ر��ж�
	Pen_Point.Key_Sta = Pen_Up;
	Read_ADS2(&Pen_Point.X, &Pen_Point.Y);
	while(PEN == 0 && t <= 250)
	{
		t++;
		Delay_ms(10);
	};
	Pen_Int_Set(1);//�����ж�
	if(t >= 250) { return 0; } //����2.5s ��Ϊ��Ч
	else { return 1; }
}

//////////////////////////////////////////////////
//��LCD�����йصĺ���
//��һ��������
//����У׼�õ�
void Drow_Touch_Point(uint8_t x, uint16_t y)
{
	drv_lcdDrawLine(x - 12, y, x + 13, y); //����
	drv_lcdDrawLine(x, y - 12, x, y + 13); //����
	drv_lcdDrawPoint(x + 1, y + 1);
	drv_lcdDrawPoint(x - 1, y + 1);
	drv_lcdDrawPoint(x + 1, y - 1);
	drv_lcdDrawPoint(x - 1, y - 1);
	drv_lcdDrawCircle(x, y, 6); //������Ȧ
}
//////////////////////////////////////////////////

//ת�����
//���ݴ�������У׼����������ת����Ľ��,������X0,Y0��
void drv_touchReadCoord(void)
{
	if(Read_ADS2(&Pen_Point.X, &Pen_Point.Y))
	{
		Pen_Point.X0 = Pen_Point.xfac * Pen_Point.X + Pen_Point.xoff;
		Pen_Point.Y0 = Pen_Point.yfac * Pen_Point.Y + Pen_Point.yoff;
	}
}
//������У׼����
//�õ��ĸ�У׼����
void drv_touchAdjust(void)
{
	uint16_t pos_temp[4][2];//���껺��ֵ
	uint8_t  cnt = 0;
	uint16_t d1, d2;
	uint32_t tem1, tem2;
	float fac;
	cnt = 0;
	POINT_COLOR = BLUE;
	BACK_COLOR = WHITE;
	drv_lcdClear(WHITE);//����
	POINT_COLOR = RED; //��ɫ
	drv_lcdClear(WHITE);//����
	Drow_Touch_Point(20, 20); //����1
	Pen_Point.Key_Sta = Pen_Up; //���������ź�
	Pen_Point.xfac = 0; //xfac��������Ƿ�У׼��,����У׼֮ǰ�������!�������
	while(1)
	{
		if(Pen_Point.Key_Sta == Pen_Down) //����������
		{
			Pen_Point.Key_Sta = Pen_Up;
			if(Read_TP_Once())//�õ����ΰ���ֵ
			{
				pos_temp[cnt][0] = Pen_Point.X;
				pos_temp[cnt][1] = Pen_Point.Y;
				cnt++;
			}
			switch(cnt)
			{
				case 1:
					drv_lcdClear(WHITE);//����
					Drow_Touch_Point(220, 20); //����2
					break;
				case 2:
					drv_lcdClear(WHITE);//����
					Drow_Touch_Point(20, 300); //����3
					break;
				case 3:
					drv_lcdClear(WHITE);//����
					Drow_Touch_Point(220, 300); //����4
					break;
				case 4:	 //ȫ���ĸ����Ѿ��õ�
					//�Ա����
					tem1 = abs(pos_temp[0][0] - pos_temp[1][0]); //x1-x2
					tem2 = abs(pos_temp[0][1] - pos_temp[1][1]); //y1-y2
					tem1 *= tem1;
					tem2 *= tem2;
					d1 = sqrt(tem1 + tem2); //�õ�1,2�ľ���
					tem1 = abs(pos_temp[2][0] - pos_temp[3][0]); //x3-x4
					tem2 = abs(pos_temp[2][1] - pos_temp[3][1]); //y3-y4
					tem1 *= tem1;
					tem2 *= tem2;
					d2 = sqrt(tem1 + tem2); //�õ�3,4�ľ���
					fac = (float)d1 / d2;
					if(fac < 0.95 || fac > 1.05 || d1 == 0 || d2 == 0) //���ϸ�
					{
						cnt = 0;
						drv_lcdClear(WHITE);//����
						Drow_Touch_Point(20, 20);
						continue;
					}
					tem1 = abs(pos_temp[0][0] - pos_temp[2][0]); //x1-x3
					tem2 = abs(pos_temp[0][1] - pos_temp[2][1]); //y1-y3
					tem1 *= tem1;
					tem2 *= tem2;
					d1 = sqrt(tem1 + tem2); //�õ�1,3�ľ���
					tem1 = abs(pos_temp[1][0] - pos_temp[3][0]); //x2-x4
					tem2 = abs(pos_temp[1][1] - pos_temp[3][1]); //y2-y4
					tem1 *= tem1;
					tem2 *= tem2;
					d2 = sqrt(tem1 + tem2); //�õ�2,4�ľ���
					fac = (float)d1 / d2;
					if(fac < 0.95 || fac > 1.05) //���ϸ�
					{
						cnt = 0;
						drv_lcdClear(WHITE);//����
						Drow_Touch_Point(20, 20);
						continue;
					}//��ȷ��
					//�Խ������
					tem1 = abs(pos_temp[1][0] - pos_temp[2][0]); //x1-x3
					tem2 = abs(pos_temp[1][1] - pos_temp[2][1]); //y1-y3
					tem1 *= tem1;
					tem2 *= tem2;
					d1 = sqrt(tem1 + tem2); //�õ�1,4�ľ���
					tem1 = abs(pos_temp[0][0] - pos_temp[3][0]); //x2-x4
					tem2 = abs(pos_temp[0][1] - pos_temp[3][1]); //y2-y4
					tem1 *= tem1;
					tem2 *= tem2;
					d2 = sqrt(tem1 + tem2); //�õ�2,3�ľ���
					fac = (float)d1 / d2;
					if(fac < 0.95 || fac > 1.05) //���ϸ�
					{
						cnt = 0;
						drv_lcdClear(WHITE);//����
						Drow_Touch_Point(20, 20);
						continue;
					}//��ȷ��
					//������
					Pen_Point.xfac = (float)200 / (pos_temp[1][0] - pos_temp[0][0]); //�õ�xfac
					Pen_Point.xoff = (240 - Pen_Point.xfac * (pos_temp[1][0] + pos_temp[0][0])) / 2; //�õ�xoff
					Pen_Point.yfac = (float)280 / (pos_temp[2][1] - pos_temp[0][1]); //�õ�yfac
					Pen_Point.yoff = (320 - Pen_Point.yfac * (pos_temp[2][1] + pos_temp[0][1])) / 2; //�õ�yoff
					POINT_COLOR = BLUE;
					drv_lcdClear(WHITE);//����
					drv_lcdShowString(35, 110, "Touch Screen Adjust OK!"); //У�����
					Delay_ms(1000);
					drv_lcdClear(WHITE);//����
					return;//У�����
			}
		}
	}
}
/*=====================================================================*/

void drv_touchInit(void)
{
	drv_touchGPIOInit();
	TS_CS_HIGH();
	drv_touchINTConfig();
	drv_touchSPIConfig();
	Pen_Point.xfac = 0.06956521;
	Pen_Point.yfac = -0.0863132;
	Pen_Point.xoff = 0xFFE9;
	Pen_Point.yoff = 0x014D;
	Pen_Point.Key_Sta = Pen_Up;
}

void drv_touchHandler(void)
{
	Pen_Point.Key_Sta = Pen_Down;
}
