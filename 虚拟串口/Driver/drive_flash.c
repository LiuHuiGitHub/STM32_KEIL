/*********************************************************************************
 * �ļ���  ��drive_flash.c
 * ����    ��W25X16��д
 * Ӳ�����ӣ�--------------
 *          | PA5  -  CLK  |
 *          | PA6  -  MISO |
 *          | PA7  -  MOSI |
 *          | PA3  -  CS   |
 *           --------------
 * ��汾  ��ST3.5.0
 * ��дʱ�䣺2013��12��6��
 * ��д��  ��LiuHui
**********************************************************************************/
#include "stm32f10x.h"
#include "drive_flash.h"

#define FlashPageByte		256

#define SPI_W25X_CS_Low()    GPIO_ResetBits(GPIOA, GPIO_Pin_3)
#define SPI_W25X_CS_High()   GPIO_SetBits(GPIOA, GPIO_Pin_3)


void dr_flashInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	/*
	����ʹ��SPI1����ӳ������ʹ��ʱע�����������:
	1.ʹ��PB0ʱ�� RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	2.ʹ��AFIOʱ��RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	3.ʹ��SPI1ʱ��RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 , ENABLE);
	4.����JTAGʱ�Ӹ��� GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	5.SPI1������ӳ�� GPIO_PinRemapConfig(GPIO_Remap_SPI1,ENABLE);
	*/
	/* Enable SPI1 GPIOA clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 , ENABLE);
//  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
//  GPIO_PinRemapConfig(GPIO_Remap_SPI1,ENABLE);
	/* Configure SPI1 pins: SCKand MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Configure PC3 as Output push-pull, used as Flash Chip select */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);
	dr_flashReadDeviceID();
}

/********************************************************
 * ��������dr_flashSendByte
 * ����  ������1Byte����
 * ����  ��1Byte����
 * ���  ��1Byte����
 ********************************************************/
u8 dr_flashSendByte(u8 byte)
{
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, byte);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI1);
}

/********************************************************
 * ��������dr_flashCmd
 * ����  ��W25Xд����
 * ����  ��ENABLE ʹ��   DISABLE ����
 * ���  ����
 ********************************************************/
void dr_flashCmd(FunctionalState NowState)
{
	SPI_W25X_CS_Low();
	if(NowState)
	{ dr_flashSendByte(W25X_Write_ENABLE); }
	else
	{ dr_flashSendByte(W25X_Write_ENABLE); }
	SPI_W25X_CS_High();
}

/********************************************************
 * ��������dr_flashReadDeviceID
 * ����  ����W25X_ID �� �ͷŵ���
 * ����  ����
 * ���  ��W25X16Ϊ14H   W25X32Ϊ15H   W25X64Ϊ16H
 ********************************************************/
u8 dr_flashReadDeviceID(void)
{
	u8 ID;
	SPI_W25X_CS_Low();
	dr_flashSendByte(W25X_Device_ID);
	dr_flashSendByte(Dummy_Byte);
	dr_flashSendByte(Dummy_Byte);
	dr_flashSendByte(Dummy_Byte);
	ID = dr_flashSendByte(Dummy_Byte);
	SPI_W25X_CS_High();
	return ID;
}

/********************************************************
 * ��������dr_flashReadJedecID
 * ����  ��������/������
 * ����  ����
 * ���  ��W25X16Ϊ3015H   W25X32Ϊ3016H   W25X64Ϊ3017H
 ********************************************************/
u32 dr_flashReadJedecID(void)
{
	u32 ID;
	SPI_W25X_CS_Low();
	dr_flashSendByte(W25X_JEDEC_ID);
	ID = dr_flashSendByte(Dummy_Byte);
	ID <<= 8;
	ID |= dr_flashSendByte(Dummy_Byte);
	ID <<= 8;
	ID |= dr_flashSendByte(Dummy_Byte);
	SPI_W25X_CS_High();
	return ID;
}

/********************************************************
 * ��������dr_flashReadStatus
 * ����  ����W25X�Ĵ���״̬
 * ����  ����
 * ���  ��
 ********************************************************/
u8 dr_flashReadStatus(void)
{
	u8 tmp;
	SPI_W25X_CS_Low();
	dr_flashSendByte(W25X_Read_Status_Register);
	tmp = dr_flashSendByte(Dummy_Byte);
	SPI_W25X_CS_High();
	return tmp;
}

/********************************************************
 * ��������dr_flashWriteStatus
 * ����  ��дW25X�Ĵ���״̬
 * ����  ��
 * ���  ����
 ********************************************************/
void dr_flashWriteStatus(u8 NowStatus)
{
	dr_flashCmd(ENABLE);
	SPI_W25X_CS_Low();
	dr_flashSendByte(W25X_Write_Status_Register);
	dr_flashSendByte(NowStatus);
	SPI_W25X_CS_High();
}

/********************************************************
 * ��������dr_flashErase
 * ����  �����W25X����
 * ����  ��EraseAddrҪ������׵�ַ
 * ���  ��Rangge �����Χ (Sector  4K   Block  64K   Chip  ȫ��)
 ********************************************************/
void dr_flashErase(u32 EraseAddr, EraseRange Rangge)
{
	dr_flashCmd(ENABLE);
	SPI_W25X_CS_Low();
	dr_flashSendByte(Rangge);
	if(Rangge != Chip)
	{
		dr_flashSendByte((EraseAddr & 0xff0000) >> 16);
		dr_flashSendByte((EraseAddr & 0xff00) >> 8);
		dr_flashSendByte(EraseAddr & 0xff);
	}
	SPI_W25X_CS_High();
	while(dr_flashReadStatus() & 0x01);
}

/********************************************************
 * ��������dr_flashWriteData
 * ����  ����W25XоƬд����
 * ����  ��Data      Ҫд������
 *         WriteAddr Ҫд�ĵ�ַ
 *         WriteByte д����ֽ���
 * ���  ����
 * ˵��  ��ÿ��д������ǰ�����ԭ�����ݡ�
 *         W25Xÿҳ256�ֽ�,WriteByteС�ڵ���256
 ********************************************************/
void dr_flashWritePage(u8 *Data, u32 WriteAddr, u16 WriteByte)
{
	dr_flashCmd(ENABLE);
	SPI_W25X_CS_Low();
	dr_flashSendByte(W25X_Page_Program);
	dr_flashSendByte((WriteAddr & 0xff0000) >> 16);
	dr_flashSendByte((WriteAddr & 0xff00) >> 8);
	dr_flashSendByte(WriteAddr & 0xff);
	if(WriteByte >= FlashPageByte)
	{ WriteByte = 256; }
	while(WriteByte--)
	{
		dr_flashSendByte(*Data);
		Data++;
	}
	SPI_W25X_CS_High();
	while(dr_flashReadStatus() & 0x01);
}

/********************************************************
 * ��������dr_flashReadData
 * ����  ����W25XоƬ������
 * ����  ��Data      ���������ݴ��λ��
 *         ReadAddr  ��ȡ�ĵ�ַ
 *         ReadByte  ��ȡ���ֽ���
 * ���  ����
 * ˵��  ��W25X����һ�δ�ͷ����ĩβ
 ********************************************************/
void dr_flashReadData(u8 *Data, u32 ReadAddr, u32 ReadByte)
{
	SPI_W25X_CS_Low();
	dr_flashSendByte(W25X_Read_Data);
	dr_flashSendByte((ReadAddr & 0xff0000) >> 16);
	dr_flashSendByte((ReadAddr & 0xff00) >> 8);
	dr_flashSendByte(ReadAddr & 0xff);
	while(ReadByte--)
	{
		*Data = dr_flashSendByte(Dummy_Byte);
		Data++;
	}
	SPI_W25X_CS_High();
	while(dr_flashReadStatus()&Busy_Flag);
}
/********************************************************
 * ��������SPI_Write_Buffer
 * ����  ����W25XоƬд����
 * ����  ��Data      Ҫд������
 *         WriteAddr Ҫд�ĵ�ַ
 *         WriteByte д����ֽ���
 * ���  ����
 * ˵��  ��ÿ��д������ǰ�����ԭ�����ݡ�����д����
 ********************************************************/
void dr_flashWriteData(u8 *Data, u32 WriteAddr, u32 WriteByte)
{
	u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	Addr = WriteAddr % FlashPageByte;
	count = FlashPageByte - Addr;
	NumOfPage = WriteByte / FlashPageByte;
	NumOfSingle = WriteByte % FlashPageByte;
	if(Addr == 0)
	{
		if(NumOfPage == 0)
		{
			dr_flashWritePage(Data, WriteAddr, WriteByte);
		}
		else
		{
			while(NumOfPage--)
			{
				dr_flashWritePage(Data, WriteAddr, FlashPageByte);
				WriteAddr += FlashPageByte;
				Data += FlashPageByte;
			}
			dr_flashWritePage(Data, WriteAddr, NumOfSingle);
		}
	}
	else
	{
		if(NumOfPage == 0)
		{
			if(NumOfSingle > count)
			{
				temp = NumOfSingle - count;
				dr_flashWritePage(Data, WriteAddr, count);
				WriteAddr += count;
				Data += count;
				dr_flashWritePage(Data, WriteAddr, temp);
			}
			else
			{
				dr_flashWritePage(Data, WriteAddr, WriteByte);
			}
		}
		else
		{
			WriteByte -= count;
			NumOfPage = WriteByte / FlashPageByte;
			NumOfSingle = WriteByte % FlashPageByte;
			dr_flashWritePage(Data, WriteAddr, count);
			WriteAddr += count;
			Data += count;
			while(NumOfPage--)
			{
				dr_flashWritePage(Data, WriteAddr, FlashPageByte);
				WriteAddr += FlashPageByte;
				Data += FlashPageByte;
			}
			if(NumOfSingle != 0)
			{
				dr_flashWritePage(Data, WriteAddr, NumOfSingle);
			}
		}
	}
}

