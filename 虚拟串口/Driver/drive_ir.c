#include "stm32f10x.h"
#include "typedef.h"
#include "drive_ir.h"
#include "sys_count.h"

static void GPIOx_Init(void)				//��������
{
	GPIO_InitTypeDef GPIOx;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIOx.GPIO_Pin = GPIO_Pin_5;
	GPIOx.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOx.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIOx);
}
static void INTx_Init(void)
{
	NVIC_InitTypeDef NVICx;
	EXTI_InitTypeDef EXTIx;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5);
	NVICx.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVICx.NVIC_IRQChannelPreemptionPriority = 0;
	NVICx.NVIC_IRQChannelSubPriority = 1;
	NVICx.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVICx);
	EXTIx.EXTI_Line = EXTI_Line5;
	EXTIx.EXTI_Mode = EXTI_Mode_Interrupt;//����EXTI��·Ϊ�ж�����
	EXTIx.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//���ش���
	EXTIx.EXTI_LineCmd	= ENABLE;
	EXTI_Init(&EXTIx);
}

#define IR_CHANNEL				0u
#define HIGH							1u
#define LOW								0u

static BOOL getIOlevel(void)
{
	if(PBin(5) == 0)
	{ return (BOOL)LOW; }
	else
	{ return (BOOL)HIGH; }
}

void drv_irInit(void)
{
	GPIOx_Init();
	INTx_Init();
	sys_countInit();
}

static UINT8 recData[4];      //Addr    AddrN   Data    DataN
static BOOL b_irStartFlag = FALSE;
static UINT8 u8_irDownCnt = 0;

BOOL b_irRevFlag = FALSE;
BOOL b_irDownFlag = FALSE;
UINT8 u8_irRecAdd1;
UINT8 u8_irRecAdd2;
UINT8 u8_irRecData1;
UINT8 u8_irRecData2;
/*
____                  ________   _   _   _   _   _   _   _   _   ___   ___   ___   ___   ___   ___   ___   ___
    |                |        | | | | | | | | | | | | | | | | | |   | |   | |   | |   | |   | |   | |   | |
    |                |        | | | | | | | | | | | | | | | | | |   | |   | |   | |   | |   | |   | |   | |
    |________________|        |_| |_| |_| |_| |_| |_| |_| |_| |_|   |_|   |_|   |_|   |_|   |_|   |_|   |_|

    |         ��ʼ��          |           ��ַ��  0x00        |               ��ַ�뷴��  0xff                |

                                 _   ___   _   _   _   ___   _   ___   ___   _   ___   ___   ___   _   ___   _   ______
                              | | | |   | | | | | | | |   | | | |   | |   | | | |   | |   | |   | | | |   | | | |
                              | | | |   | | | | | | | |   | | | |   | |   | | | |   | |   | |   | | | |   | | | |
                              |_| |_|   |_| |_| |_| |_|   |_| |_|   |_|   |_| |_|   |_|   |_|   |_| |_|   |_| |_|

                              |             ������   0x45           |           �����뷴��    0xba            | ֹͣ |

*/

void drv_irHandler(void)
{
	static UINT8 ucIndex = 0;
	static UINT32 uiLowLevelTime, uiHighLevelTime;
	UINT8 index, bits;
	if(getIOlevel() == LOW)			//�����жϣ��ж�ʱ���ŵ͵�ƽ��ȡ��ӦΪǰ�εĸ�����
	{
		uiHighLevelTime = sys_countGetTime(IR_CHANNEL);
		if(b_irStartFlag == TRUE)
		{
			b_irStartFlag = FALSE;
			ucIndex = 0;
			recData[0] = 0;
			recData[1] = 0;
			recData[2] = 0;
			recData[3] = 0;
		}
		if(uiLowLevelTime > START_HIGH_TIME - START_ERROR_TIME && uiLowLevelTime < START_HIGH_TIME + START_ERROR_TIME)
		{
			if(uiHighLevelTime > START_LOW_TIME - START_ERROR_TIME && uiHighLevelTime < START_LOW_TIME + START_ERROR_TIME)		//START
			{
				b_irStartFlag = TRUE;
				u8_irDownCnt = 0;
			}
			else if(uiHighLevelTime > START_REPEAT_TIME - START_ERROR_TIME && uiHighLevelTime < START_REPEAT_TIME + START_ERROR_TIME)
			{
				if(++u8_irDownCnt >= DOWN_OVER_CNT)
				{
					u8_irDownCnt = 0;
					b_irDownFlag = TRUE;
				}
			}
		}
		if(ucIndex >= 32)		//reveiced Over or Error
		{
			ucIndex = 0;
			if((recData[2] ^ recData[3]) == 0xFF)   //������ɣ�У����յ�ַ������
			{
				u8_irRecAdd1 = recData[0];
				u8_irRecAdd2 = recData[1];
				u8_irRecData1 = recData[2];
				u8_irRecData2 = recData[3];
				b_irRevFlag = TRUE;
			}
		}
	}
	else
	{
		uiLowLevelTime = sys_countGetTime(IR_CHANNEL);
		if(uiLowLevelTime > LOW_TIME - ERROR_TIME && uiLowLevelTime < LOW_TIME + ERROR_TIME)
		{
			index = (ucIndex) / 8;
			bits = 1 << ((ucIndex) & 0x07);
			if(uiHighLevelTime > ZERO_TIME - ERROR_TIME && uiHighLevelTime < ZERO_TIME + ERROR_TIME)
			{ recData[index] &= ~bits; }
			else if(uiHighLevelTime > ONE_TIME - ERROR_TIME && uiHighLevelTime < ONE_TIME + ERROR_TIME)
			{ recData[index] |= bits; }
			ucIndex++;
		}
	}
}
