/**************************************************************
** 	��ţ������
**	LCD��������
**  �汾��V1.0
**	��̳��www.openmcu.com
**	�Ա���http://shop36995246.taobao.com/
**  ����֧��Ⱥ��121939788
***************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "Typedef.h"
#include "drive_lcd.h"
#include "Font_Lib.h"	 //�ֿ��ļ���
#include "hz16.c"

/* LCD ������������*/
#define LCD_CS  PBout(0)
#define LCD_RS	PDout(9)
#define LCD_WR  PBout(1)
#define LCD_RD	PDout(10)

UINT16 POINT_COLOR = BLUE, BACK_COLOR = WHITE; /* �ֱ����õ����ɫ�͵�ɫ	*/
UINT16 DeviceCode;

extern void Delay_us(uint16_t xus);
extern void Delay_ms(uint16_t xms);
static void LCD_IO_Init(void);
static void Write_Cmd(UINT16 LCD_Reg);
static void Write_Dat(UINT16 LCD_Dat);
static UINT16 LCD_ReadDat(void);
static UINT16 LCD_ReadReg(UINT16 LCD_Reg);
static void drv_lcdWriteRAMPrepare(void);
static void LCD_WriteReg(UINT16 LCD_Reg, UINT16 LCD_Dat);
static void LCD_WriteReg(UINT16 LCD_Reg, UINT16 LCD_Dat);

/*****************************************************************************
** ��������: LCD_Write_Reg
** ��������: дָ�����
** ��  ����: Dream
** �ա�  ��: 2010��12��06��
*****************************************************************************/
void LCD_WriteReg(UINT16 LCD_Reg, UINT16 LCD_Dat)
{
	Write_Cmd(LCD_Reg);
	Write_Dat(LCD_Dat);
}
/*****************************************************************************
** ��������: Write_Cmd
** ��������: дָ��
** ��  ����: Dream
** �ա�  ��: 2010��12��06��
*****************************************************************************/
void Write_Cmd(UINT16 LCD_Reg)
{
	LCD_CS = 0;
	LCD_RS = 0;
	GPIOE->ODR = LCD_Reg;
	LCD_WR = 0;
	LCD_WR = 1;
	LCD_CS = 1;
}
/*****************************************************************************
** ��������: Write_Dat
** ��������: д����
** ��  ����: Dream
** �ա�  ��: 2010��12��06��
*****************************************************************************/
void Write_Dat(UINT16 LCD_Dat)
{
	LCD_CS = 0;
	LCD_RS = 1;
	GPIOE->ODR = LCD_Dat;
	LCD_WR = 0;
	LCD_WR = 1;
	LCD_CS = 1;
}
/*****************************************************************************
** ��������: LCD_ReadReg
** ��������: ��ָ��
** ��  ����: Dream
** �ա�  ��: 2010��12��06��
*****************************************************************************/
UINT16 LCD_ReadReg(UINT16 LCD_Reg)
{
	Write_Cmd(LCD_Reg);  //д��Ҫ���ļĴ�����
	return LCD_ReadDat();
}
/*****************************************************************************
** ��������: LCD_ReadDat
** ��������: ������
** ��  ����: Dream
** �ա�  ��: 2010��12��06��
*****************************************************************************/
UINT16 LCD_ReadDat(void)
{
	UINT16 temp;
	GPIOE->CRH = (GPIOE->CRH & 0x00000000) | 0x44444444;  //���˿ڸ�8λ���ó�����
	GPIOE->CRL = (GPIOE->CRL & 0x00000000) | 0x44444444;  //���˿ڵ�8λ���ó�����
	LCD_CS = 0;
	LCD_RS = 1;
	LCD_RD = 0;
	temp = GPIOE->IDR; 	//��ȡ����(���Ĵ���ʱ,������Ҫ��2��)
	//temp = GPIOE->IDR;
	LCD_RD = 1;
	LCD_CS = 1;
	GPIOE->CRH = (GPIOE->CRH & 0x00000000) | 0x33333333;  //�ͷŶ˿ڸ�8λΪ���
	GPIOE->CRL = (GPIOE->CRL & 0x00000000) | 0x33333333;  //�ͷŶ˿ڵ�8λΪ���
	return temp;
}
/*****************************************************************************
** ��������: LCD_IO_Init
** ��������: LCD_IO������
** ��  ����: Dream
** �ա�  ��: 2010��12��06��
*****************************************************************************/
void LCD_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);
	/* ���ÿ���IO ���ӵ�PD12.PD13.PD14.PD15 *********************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   // ���������ʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // ���IO���������Ϊ50MHZ
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   // ���������ʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // ���IO���������Ϊ50MHZ
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	/* ��������IO ���ӵ�GPIOE *********************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   // ���������ʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // ���IO���������Ϊ50MHZ
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}
/*****************************************************************************
** ��������: drv_lcdInit
** ��������: LCD��ʼ��
** ��  ����: Dream
** �ա�  ��: 2010��12��06��
*****************************************************************************/
void drv_lcdInit(void)
{
	LCD_IO_Init();
	LCD_WriteReg(0x0000, 0x0001);
	Delay_ms(50); // LCD_Delay 50 ms
	DeviceCode = LCD_ReadReg(0x0000);
	//printf("ID:%d\n",DeviceCode);
	if(DeviceCode == 0x9325 || DeviceCode == 0x9328) //ILI9325
	{
		LCD_WriteReg(0x00e7, 0x0010);
		LCD_WriteReg(0x0000, 0x0001); //�����ڲ�ʱ��
		LCD_WriteReg(0x0001, 0x0100);
		LCD_WriteReg(0x0002, 0x0700); //��Դ����
		//LCD_WriteReg(0x0003,(1<<3)|(1<<4) ); 	//65K  RGB
		//DRIVE TABLE(�Ĵ��� 03H)
		//BIT3=AM BIT4:5=ID0:1
		//AM ID0 ID1   FUNCATION
		// 0  0   0	   R->L D->U
		// 1  0   0	   D->U	R->L
		// 0  1   0	   L->R D->U
		// 1  1   0    D->U	L->R
		// 0  0   1	   R->L U->D
		// 1  0   1    U->D	R->L
		// 0  1   1    L->R U->D �����������.ˮƽ��������
		// 1  1   1	   U->D	L->R  ��ֱ��������
		LCD_WriteReg(0x0003, (1 << 12) | (3 << 4) | (0 << 3)); //65K
		LCD_WriteReg(0x0004, 0x0000);
		LCD_WriteReg(0x0008, 0x0207);
		LCD_WriteReg(0x0009, 0x0000);
		LCD_WriteReg(0x000a, 0x0000); //display setting
		LCD_WriteReg(0x000c, 0x0001); //display setting
		LCD_WriteReg(0x000d, 0x0000); //0f3c
		LCD_WriteReg(0x000f, 0x0000);
		//��Դ����
		LCD_WriteReg(0x0010, 0x0000);
		LCD_WriteReg(0x0011, 0x0007);
		LCD_WriteReg(0x0012, 0x0000);
		LCD_WriteReg(0x0013, 0x0000);
		Delay_ms(1);
		LCD_WriteReg(0x0010, 0x1590);
		LCD_WriteReg(0x0011, 0x0227);
		Delay_ms(1);
		LCD_WriteReg(0x0012, 0x009c);
		Delay_ms(1);
		LCD_WriteReg(0x0013, 0x1900);
		LCD_WriteReg(0x0029, 0x0023);
		LCD_WriteReg(0x002b, 0x000e);
		Delay_ms(1);
		LCD_WriteReg(0x0020, 0x0000);
		LCD_WriteReg(0x0021, 0x013f);
		Delay_ms(1);
		//٤��У��
		LCD_WriteReg(0x0030, 0x0007);
		LCD_WriteReg(0x0031, 0x0707);
		LCD_WriteReg(0x0032, 0x0006);
		LCD_WriteReg(0x0035, 0x0704);
		LCD_WriteReg(0x0036, 0x1f04);
		LCD_WriteReg(0x0037, 0x0004);
		LCD_WriteReg(0x0038, 0x0000);
		LCD_WriteReg(0x0039, 0x0706);
		LCD_WriteReg(0x003c, 0x0701);
		LCD_WriteReg(0x003d, 0x000f);
		Delay_ms(1);
		LCD_WriteReg(0x0050, 0x0000); //ˮƽGRAM��ʼλ��
		LCD_WriteReg(0x0051, 0x00ef); //ˮƽGRAM��ֹλ��
		LCD_WriteReg(0x0052, 0x0000); //��ֱGRAM��ʼλ��
		LCD_WriteReg(0x0053, 0x013f); //��ֱGRAM��ֹλ��
		LCD_WriteReg(0x0060, 0xa700);
		LCD_WriteReg(0x0061, 0x0001);
		LCD_WriteReg(0x006a, 0x0000);
		LCD_WriteReg(0x0080, 0x0000);
		LCD_WriteReg(0x0081, 0x0000);
		LCD_WriteReg(0x0082, 0x0000);
		LCD_WriteReg(0x0083, 0x0000);
		LCD_WriteReg(0x0084, 0x0000);
		LCD_WriteReg(0x0085, 0x0000);
		LCD_WriteReg(0x0090, 0x0010);
		LCD_WriteReg(0x0092, 0x0000);
		LCD_WriteReg(0x0093, 0x0003);
		LCD_WriteReg(0x0095, 0x0110);
		LCD_WriteReg(0x0097, 0x0000);
		LCD_WriteReg(0x0098, 0x0000);
		//������ʾ����
		LCD_WriteReg(0x0007, 0x0133);
		LCD_WriteReg(0x0020, 0x0000);
		LCD_WriteReg(0x0021, 0x013f);
	}
	else if(DeviceCode == 0x9320 || DeviceCode == 0x9300)
	{
		LCD_WriteReg(0x00, 0x0000);
		LCD_WriteReg(0x01, 0x0100);	//Driver Output Contral.
		LCD_WriteReg(0x02, 0x0700);	//LCD Driver Waveform Contral.
		LCD_WriteReg(0x03, 0x1030); //Entry Mode Set.
		//LCD_WriteReg(0x03,0x1018);	//Entry Mode Set.
		LCD_WriteReg(0x04, 0x0000);	//Scalling Contral.
		LCD_WriteReg(0x08, 0x0202);	//Display Contral 2.(0x0207)
		LCD_WriteReg(0x09, 0x0000);	//Display Contral 3.(0x0000)
		LCD_WriteReg(0x0a, 0x0000);	//Frame Cycle Contal.(0x0000)
		LCD_WriteReg(0x0c, (1 << 0));	//Extern Display Interface Contral 1.(0x0000)
		LCD_WriteReg(0x0d, 0x0000);	//Frame Maker Position.
		LCD_WriteReg(0x0f, 0x0000);	//Extern Display Interface Contral 2.
		Delay_ms(1);
		LCD_WriteReg(0x07, 0x0101);	//Display Contral.
		Delay_ms(1);
		LCD_WriteReg(0x10, (1 << 12) | (0 << 8) | (1 << 7) | (1 << 6) | (0 << 4));	//Power Control 1.(0x16b0)
		LCD_WriteReg(0x11, 0x0007);								//Power Control 2.(0x0001)
		LCD_WriteReg(0x12, (1 << 8) | (1 << 4) | (0 << 0));				//Power Control 3.(0x0138)
		LCD_WriteReg(0x13, 0x0b00);								//Power Control 4.
		LCD_WriteReg(0x29, 0x0000);								//Power Control 7.
		LCD_WriteReg(0x2b, (1 << 14) | (1 << 4));
		LCD_WriteReg(0x50, 0);	//Set X Star
		//ˮƽGRAM��ֹλ��Set X End.
		LCD_WriteReg(0x51, 239);	//Set Y Star
		LCD_WriteReg(0x52, 0);	//Set Y End.t.
		LCD_WriteReg(0x53, 319);	//
		LCD_WriteReg(0x60, 0x2700);	//Driver Output Control.
		LCD_WriteReg(0x61, 0x0001);	//Driver Output Control.
		LCD_WriteReg(0x6a, 0x0000);	//Vertical Srcoll Control.
		LCD_WriteReg(0x80, 0x0000);	//Display Position? Partial Display 1.
		LCD_WriteReg(0x81, 0x0000);	//RAM Address Start? Partial Display 1.
		LCD_WriteReg(0x82, 0x0000);	//RAM Address End-Partial Display 1.
		LCD_WriteReg(0x83, 0x0000);	//Displsy Position? Partial Display 2.
		LCD_WriteReg(0x84, 0x0000);	//RAM Address Start? Partial Display 2.
		LCD_WriteReg(0x85, 0x0000);	//RAM Address End? Partial Display 2.
		LCD_WriteReg(0x90, (0 << 7) | (16 << 0));	//Frame Cycle Contral.(0x0013)
		LCD_WriteReg(0x92, 0x0000);	//Panel Interface Contral 2.(0x0000)
		LCD_WriteReg(0x93, 0x0001);	//Panel Interface Contral 3.
		LCD_WriteReg(0x95, 0x0110);	//Frame Cycle Contral.(0x0110)
		LCD_WriteReg(0x97, (0 << 8));	//
		LCD_WriteReg(0x98, 0x0000);	//Frame Cycle Contral.
		LCD_WriteReg(0x07, 0x0173);	//(0x0173)
		Delay_ms(1);
	}
	else if(DeviceCode == 0x8999)	 //��Ӧ������ICΪSSD1289
	{
		//************* Start Initial Sequence **********//
		LCD_WriteReg(0x00, 0x0001); // Start internal OSC.
		LCD_WriteReg(0x01, 0x3B3F); // Driver output control, RL=0;REV=1;GD=1;BGR=0;SM=0;TB=1
		LCD_WriteReg(0x02, 0x0600); // set 1 line inversion
		//************* Power control setup ************/
		LCD_WriteReg(0x0C, 0x0007); // Adjust VCIX2 output voltage
		LCD_WriteReg(0x0D, 0x0006); // Set amplitude magnification of VLCD63
		LCD_WriteReg(0x0E, 0x3200); // Set alternating amplitude of VCOM
		LCD_WriteReg(0x1E, 0x00BB); // Set VcomH voltage
		LCD_WriteReg(0x03, 0x6A64); // Step-up factor/cycle setting
		//************ RAM position control **********/
		LCD_WriteReg(0x0F, 0x0000); // Gate scan position start at G0.
		LCD_WriteReg(0x44, 0xEF00); // Horizontal RAM address position
		LCD_WriteReg(0x45, 0x0000); // Vertical RAM address start position
		LCD_WriteReg(0x46, 0x013F); // Vertical RAM address end position
		// ----------- Adjust the Gamma Curve ----------//
		LCD_WriteReg(0x30, 0x0000);
		LCD_WriteReg(0x31, 0x0706);
		LCD_WriteReg(0x32, 0x0206);
		LCD_WriteReg(0x33, 0x0300);
		LCD_WriteReg(0x34, 0x0002);
		LCD_WriteReg(0x35, 0x0000);
		LCD_WriteReg(0x36, 0x0707);
		LCD_WriteReg(0x37, 0x0200);
		LCD_WriteReg(0x3A, 0x0908);
		LCD_WriteReg(0x3B, 0x0F0D);
		//************* Special command **************/
		LCD_WriteReg(0x28, 0x0006); // Enable test command
		LCD_WriteReg(0x2F, 0x12EB); // RAM speed tuning
		LCD_WriteReg(0x26, 0x7000); // Internal Bandgap strength
		LCD_WriteReg(0x20, 0xB0E3); // Internal Vcom strength
		LCD_WriteReg(0x27, 0x0044); // Internal Vcomh/VcomL timing
		LCD_WriteReg(0x2E, 0x7E45); // VCOM charge sharing time
		//************* Turn On display ******************/
		LCD_WriteReg(0x10, 0x0000); // Sleep mode off.
		Delay_ms(30); // Wait 30mS
		LCD_WriteReg(0x11, 0x6870); // Entry mode setup. 262K type B, take care on the data bus with 16it only
		LCD_WriteReg(0x07, 0x0033); // Display ON	*/
	}
	drv_lcdClear(BACK_COLOR);
}
/*****************************************************************************
** ��������: drv_lcdDrawPoint
** ��������: дһ����
** ��  ����: Dream
** �ա�  ��: 2010��12��06��
*****************************************************************************/
void drv_lcdDrawPoint(UINT16 x, UINT16 y)
{
	drv_lcdSetCursor(x, y); //���ù��λ��
	Write_Cmd(R34);//��ʼд��GRAM
	Write_Dat(POINT_COLOR);
}
/*****************************************************************************
** ��������: drv_lcdWriteRAMPrepare
** ��������: Щ׼��
** ��  ����: Dream
** �ա�  ��: 2010��12��06��
*****************************************************************************/
void drv_lcdWriteRAMPrepare(void)
{
	Write_Cmd(R34);
}
/*****************************************************************************
** ��������: drv_lcdSetCursor
** ��������: ���ù�꺯��
** ��  ����: Dream
** �ա�  ��: 2010��12��06��
*****************************************************************************/
void drv_lcdSetCursor(UINT8 Xpos, UINT16 Ypos)
{
	if(DeviceCode == 0x8999 || DeviceCode == 0x9919)
	{
		LCD_WriteReg(0x004E, Xpos);
		LCD_WriteReg(0X004F, Ypos);
	}
	else
	{
		LCD_WriteReg(0x0020, Xpos);
		LCD_WriteReg(0X0021, Ypos);
	}
}
/*****************************************************************************
** ��������: drv_lcdSetWindow
** ��������: ���ô��ں��� (��Width��Height����һ�£�ǰ����)
** ��  ����: Dream
** �ա�  ��: 2010��12��06��
*****************************************************************************/
void drv_lcdSetWindow(UINT8 x, UINT16 y, UINT8 x_end, UINT16 y_end)
{
	if(DeviceCode == 0x8999)
	{
		LCD_WriteReg(0x44, x | ((x_end + x) << 8));
		LCD_WriteReg(0x45, y);
		LCD_WriteReg(0x46, y + y_end);
	}
	else
	{
		LCD_WriteReg(0x50, x);                      /* Horizontal GRAM Start Address      */
		LCD_WriteReg(0x51, x + x_end);             /* Horizontal GRAM End   Address (-1) */
		LCD_WriteReg(0x52, y);                      /* Vertical   GRAM Start Address      */
		LCD_WriteReg(0x53, y + y_end);              /* Vertical   GRAM End   Address (-1) */
	}
	drv_lcdSetCursor(x, y);			//���ù��λ��
}
/*****************************************************************************
** ��������: drv_lcdShowString
** ��������: ��ʾ�ַ�������
** ��  ����: Dream
** �ա�  ��: 2010��12��06��
*****************************************************************************/
#define MAX_CHAR_POSX 232
#define MAX_CHAR_POSY 304
void drv_lcdShowString(UINT8 x, UINT16 y, UINT8 *p)
{
	while(*p != '\0')
	{
		if(x > MAX_CHAR_POSX) {x = 0; y += 16;}
		if(y > MAX_CHAR_POSY) {y = x = 0; drv_lcdClear(WHITE);}
		LCD_ShowChar(x, y, *p, 16, 0);
		x += 8;
		p++;
	}
}
/*****************************************************************************
** ��������: LCD_ShowChar
** ��������: ��ʾһ���ַ�����
** ��  ����: Dream
** �ա�  ��: 2010��12��06��
*****************************************************************************/
void LCD_ShowChar(UINT8 x, UINT16 y, UINT8 chars, UINT8 size, UINT8 mode)
{
	UINT8 temp;
	UINT8 pos, t;
	if(x > MAX_CHAR_POSX || y > MAX_CHAR_POSY) { return; }
	drv_lcdSetWindow(x, y, (size / 2 - 1), size - 1); //���ô���
	drv_lcdWriteRAMPrepare();        //��ʼд��GRAM
	if(!mode) 						//�ǵ��ӷ�ʽ
	{
		for(pos = 0; pos < size; pos++)
		{
			if(size == 12) { temp = ASCII_1206[chars - 0x20][pos]; } //����1206����
			else { temp = ASCII_1608[chars - 0x20][pos]; }		 //����1608����
			for(t = 0; t < size / 2; t++)
			{
				//if(temp&0x01)            	 			//�ȴ���λ��ȡģ�й�ϵ
				if((temp << t) & 0x80)						//�ȴ���λ
				{
					Write_Dat(RED);
				}
				else
				{
					Write_Dat(WHITE);
				}
				//temp>>=1; 	   							//����ȴ���λ��ȥ��������
			}
		}
	}
	else//���ӷ�ʽ
	{
		for(pos = 0; pos < size; pos++)
		{
			if(size == 12) { temp = ASCII_1206[chars - 0x20][pos]; }	//����1206����
			else { temp = ASCII_1608[chars - 0x20][pos]; }		 	//����1608����
			for(t = 0; t < size / 2; t++)
			{
				if((temp << t) & 0x80) { drv_lcdDrawPoint(x + t, y + pos); }	//��һ����
				//temp>>=1; 								//����ȴ���λ��ȥ��������
			}
		}
	}
	/* �ָ������С	*/
//	LCD_WriteReg(0x43, 0x0000); //ˮƽ����GRAM��ʼ��ַ
	LCD_WriteReg(0x44, 0xEF00); //ˮƽ����GRAM������ַ
	LCD_WriteReg(0x45, 0x0000); //��ֱ����GRAM��ʼ��ַ
	LCD_WriteReg(0x46, 0x013F); //��ֱ����GRAM������ַ
}
/*****************************************************************************
** ��������: drv_lcdClear
** ��������: ����Ļ����
** ��  ����: Dream
** �ա�  ��: 2010��12��06��
*****************************************************************************/
void drv_lcdClear(UINT16 Color)
{
	UINT32 index = 0;
	drv_lcdSetCursor(0x00, 0x0000); //���ù��λ��
	drv_lcdWriteRAMPrepare();     //��ʼд��GRAM
	for(index = 0; index < 76800; index++)
	{
		Write_Dat(Color);
	}
}
/*****************************************************************************
** ��������: drv_lcdWriteString
** ��������: ��ָ��λ�ÿ�ʼ��ʾһ���ַ�����һ������
				֧���Զ�����
** ��  ����: Dream
** �ա�  ��: 2010��12��06��
*****************************************************************************/
void drv_lcdWriteString(UINT16 x0, UINT16 y0, UINT8 *pcStr, UINT16 color)
{
	UINT16 usIndex;
	UINT16 usWidth = 0;
	FNT_GB16 *ptGb16 = 0;
	ptGb16 = (FNT_GB16 *)GBHZ_16;
	while(1)
	{
		if(*pcStr == 0)
		{
			break;                                     /* �ַ�������            */
		}
		x0 = x0 + (usWidth);                           /* �����ַ�����ʾ�ɽ���         */
		if(*pcStr > 0x80)                              /* �ж�Ϊ����                   */
		{
			if((x0 + 16) > LCD_W)                      /* ���ʣ��ռ��Ƿ��㹻         */
			{
				x0 = 0;
				y0 = y0 + 16;                          /* �ı���ʾ����                 */
				if(y0 > LCD_H)                         /* �����곬��                   */
				{
					y0 = 0;
				}
			}
			usIndex = findHzIndex(pcStr);
			usWidth = drv_lcdWriteOneHzChar((UINT8 *) & (ptGb16[usIndex].Msk[0]), x0, y0, color);
			/* ��ʾ�ַ�                     */
			pcStr += 2;
		}
		else
		{
			/* �ж�Ϊ�Ǻ���                 */
			if(*pcStr == '\r')                          /* ����                         */
			{
				y0 = y0 + 16;                           /* �ı���ʾ����                 */
				if(y0 > LCD_H)                          /* �����곬��                   */
				{
					y0 = 0;
				}
				pcStr++;
				usWidth = 0;
				continue;
			}
			else if(*pcStr == '\n')                     /* ���뵽���                   */
			{
				x0 = 0;
				pcStr++;
				usWidth = 0;
				continue;
			}
			else
			{
				if((x0 + 8) > LCD_W)                     /* ���ʣ��ռ��Ƿ��㹻         */
				{
					x0 = 0;
					y0 = y0 + 16;                        /* �ı���ʾ����                 */
					if(y0 > LCD_H)                       /* �����곬��                   */
					{
						y0 = 0;
					}
				}
				usWidth = drv_lcdWriteOneASCII((UINT8 *)&ASCII_1608[(*pcStr - 0x20)][0], x0, y0, color);
				/* ASCII���21H��ֵ��Ӧ��λ��3��*/
				pcStr += 1;
			}
		}
	}
}
/*****************************************************************************
** ��������: drv_lcdWriteOneHzChar
** ��������: ��ʾһ��ָ����С�ĺ���
** ��  ����: Dream
** �ա�  ��: 2010��12��06��
*****************************************************************************/
UINT16 drv_lcdWriteOneHzChar(UINT8 *pucMsk,
                             UINT16 x0,
                             UINT16 y0,
                             UINT16 color)
{
	UINT16 i, j;
	UINT16 mod[16];                                      /* ��ǰ��ģ                     */
	UINT16 *pusMsk;                                      /* ��ǰ�ֿ��ַ                 */
	UINT16 y;
	pusMsk = (UINT16 *)pucMsk;
	for(i = 0; i < 16; i++)                                /* ���浱ǰ���ֵ���ʽ��ģ       */
	{
		mod[i] = *pusMsk++;                                /* ȡ�õ�ǰ��ģ�����ֶ������   */
		mod[i] = ((mod[i] & 0xff00) >> 8) | ((mod[i] & 0x00ff) << 8);/* ��ģ�����ߵ��ֽڣ�Ϊ����ʾ   */
		/* ��Ҫ��                       */
	}
	y = y0;
	for(i = 0; i < 16; i++)                                /* 16��                         */
	{
#ifdef __DISPLAY_BUFFER                            /* ʹ���Դ���ʾ                 */
		for(j = 0; j < 16; j++)                            /* 16��                         */
		{
			if((mod[i] << j) & 0x8000)                     /* ��ʾ��ģ                     */
			{
				DispBuf[240 * (y0 + i) + x0 + j] = color;
			}
		}
#else                                              /* ֱ����ʾ                     */
		drv_lcdSetCursor(x0, y);                              /* ����д���ݵ�ַָ��           */
		drv_lcdWriteRAMPrepare();        					   /*��ʼд��GRAM	*/
		for(j = 0; j < 16; j++)                            /* 16��                         */
		{
			if((mod[i] << j) & 0x8000)                     /* ��ʾ��ģ                     */
			{
				Write_Dat(color);
			}
			else
			{
				Write_Dat(BACK_COLOR);                     /* �ö���ʽ����д�հ׵������   */
				//LCD_ReadDat();
			}
		}
		y++;
#endif
	}
	return (16);                                          /* ����16λ�п�                 */
}
/*****************************************************************************
** ��������: drv_lcdWriteOneASCII
** ��������: ��ʾһ��ָ����С���ַ�
** ��  ����: Dream
** �ա�  ��: 2010��12��06��
*****************************************************************************/
UINT16 drv_lcdWriteOneASCII(UINT8 *pucMsk,
                            UINT16 x0,
                            UINT16 y0,
                            UINT16 color)
{
	UINT16 i, j;
	UINT16 y;
	UINT8 ucChar;
	y = y0;
	for(i = 0; i < 16; i++)                               /* 16��                         */
	{
		ucChar = *pucMsk++;
#ifdef __DISPLAY_BUFFER                           /* ʹ���Դ���ʾ                 */
		for(j = 0; j < 8; j++)                            /* 8��                          */
		{
			if((ucChar << j) & 0x80)                      /* ��ʾ��ģ                     */
			{
				DispBuf[240 * (y0 + i) + x0 + j] = color;
			}
		}
#else                                             /* ֱ����ʾ                     */
		drv_lcdSetCursor(x0, y);                             /* ����д���ݵ�ַָ��           */
		drv_lcdWriteRAMPrepare();        					  /* ��ʼд��GRAM	    		  */
		for(j = 0; j < 8; j++)                            /* 8��                          */
		{
			if((ucChar << j) & 0x80)                      /* ��ʾ��ģ                     */
			{
				Write_Dat(color);
			}
			else
			{
				Write_Dat(BACK_COLOR);
				//LCD_ReadDat();
			}
		}
		y++;
#endif
	}
	return (8);                                           /* ����16λ�п�                 */
}
/*****************************************************************************
** ��������: Num_power
** ��������: M��N�η�
** ��  ����: Dream
** �ա�  ��: 2010��12��06��
*****************************************************************************/
UINT32 Num_power(UINT8 m, UINT8 n)
{
	UINT32 result = 1;
	while(n--) { result *= m; }
	return result;
}
/*****************************************************************************
** ��������: LCD_ShowNum
** ��������: ��ָ��λ����ʾһ������
				num:��ֵ(0~4294967295);
** ��  ����: Dream
** �ա�  ��: 2010��12��06��
*****************************************************************************/
void LCD_ShowNum(UINT8 x, UINT16 y, UINT32 num, UINT8 len, UINT8 size)
{
	UINT8 t, temp;
	UINT8 enshow = 0;
	for(t = 0; t < len; t++)
	{
		temp = (num / Num_power(10, len - t - 1)) % 10;
		if(enshow == 0 && t < (len - 1))
		{
			if(temp == 0)
			{
				LCD_ShowChar(x + (size / 2)*t, y, ' ', size, 0);
				continue;
			}
			else { enshow = 1; }
		}
		LCD_ShowChar(x + (size / 2)*t, y, temp + '0', size, 0);
	}
}
/*****************************************************************************
** ��������: drv_lcdShowPicture
** ��������: ��ָ����λ����ʾһ��ͼƬ
				Xpos��YposΪͼƬ��ʾ��ַ��Height��Width ΪͼƬ�Ŀ�Ⱥ͸߶�
** ��  ����: Dream
** �ա�  ��: 2010��12��06��
*****************************************************************************/
void drv_lcdShowPicture(UINT8 Xpos, UINT16 Ypos, UINT8 Height, UINT16 Width, UINT8 *bitmap)
{
	UINT32 index;
	UINT32 size = Height * Width;
	UINT16 *bitmap_ptr = (UINT16 *)bitmap;
	drv_lcdSetWindow(Xpos, Ypos, Width - 1, Height - 1);
	//LCD_WriteReg(0x03, 0x1038);	//�����Ҫ������ʾͼƬ������ȥ������ ��ͬʱ��Width��Hight����һ�¾Ϳ���
	drv_lcdWriteRAMPrepare();
	for(index = 0; index < size; index++)
	{
		Write_Dat(*bitmap_ptr++);
	}
	//�ָ������С
	LCD_WriteReg(R80, 0x0000); //ˮƽ����GRAM��ʼ��ַ
	LCD_WriteReg(R81, 0x00EF); //ˮƽ����GRAM������ַ
	LCD_WriteReg(R82, 0x0000); //��ֱ����GRAM��ʼ��ַ
	LCD_WriteReg(R83, 0x013F); //��ֱ����GRAM������ַ
}
/*****************************************************************************
** ��������: drv_lcdDrawLine
** ��������: ��ָ��������λ�û�һ����
				x1,y1:��������  x2,y2:�յ�����
** ��  ����: Dream
** �ա�  ��: 2010��12��06��
*****************************************************************************/
void drv_lcdDrawLine(UINT16 x1, UINT16 y1, UINT16 x2, UINT16 y2)
{
	UINT16 t;
	SINT16 xerr = 0, yerr = 0, delta_x, delta_y, distance;
	SINT16 incx, incy, uRow, uCol;
	delta_x = x2 - x1; 				//������������
	delta_y = y2 - y1;
	uRow = x1;
	uCol = y1;
	if(delta_x > 0) { incx = 1; } 		//���õ�������
	else if(delta_x == 0) { incx = 0; }	//��ֱ��
	else {incx = -1; delta_x = -delta_x;}
	if(delta_y > 0) { incy = 1; }
	else if(delta_y == 0) { incy = 0; }	//ˮƽ��
	else {incy = -1; delta_y = -delta_y;}
	if(delta_x > delta_y) { distance = delta_x; } //ѡȡ��������������
	else { distance = delta_y; }
	for(t = 0; t <= distance + 1; t++)	//�������
	{
		drv_lcdDrawPoint(uRow, uCol); //����
		xerr += delta_x ;
		yerr += delta_y ;
		if(xerr > distance)
		{
			xerr -= distance;
			uRow += incx;
		}
		if(yerr > distance)
		{
			yerr -= distance;
			uCol += incy;
		}
	}
}
/*****************************************************************************
** ��������: drv_lcdDrawLine
** ��������: ��ָ��λ�û�һ��ָ����С��Բ
				(x,y):���ĵ� 	 r    :�뾶
** ��  ����: Dream
** �ա�  ��: 2010��12��06��
*****************************************************************************/
void drv_lcdDrawCircle(UINT8 x0, UINT16 y0, UINT8 r)
{
	SINT16 a, b;
	SINT16 di;
	a = 0;
	b = r;
	di = 3 - (r << 1);       //�ж��¸���λ�õı�־
	while(a <= b)
	{
		drv_lcdDrawPoint(x0 - b, y0 - a);        //3
		drv_lcdDrawPoint(x0 + b, y0 - a);        //0
		drv_lcdDrawPoint(x0 - a, y0 + b);        //1
		drv_lcdDrawPoint(x0 - b, y0 - a);        //7
		drv_lcdDrawPoint(x0 - a, y0 - b);        //2
		drv_lcdDrawPoint(x0 + b, y0 + a);        //4
		drv_lcdDrawPoint(x0 + a, y0 - b);        //5
		drv_lcdDrawPoint(x0 + a, y0 + b);        //6
		drv_lcdDrawPoint(x0 - b, y0 + a);
		a++;
		//ʹ��Bresenham�㷨��Բ
		if(di < 0) { di += 4 * a + 6; }
		else
		{
			di += 10 + 4 * (a - b);
			b--;
		}
		drv_lcdDrawPoint(x0 + a, y0 + b);
	}
}

//��һ�����
//2*2�ĵ�
void drv_lcdDrawBigPoint(UINT16 x, UINT16 y)
{
	drv_lcdDrawPoint(x, y); //���ĵ�
	drv_lcdDrawPoint(x + 1, y);
	drv_lcdDrawPoint(x, y + 1);
	drv_lcdDrawPoint(x + 1, y + 1);
}

/*********************************************************************************************************
** End of File
*********************************************************************************************************/
