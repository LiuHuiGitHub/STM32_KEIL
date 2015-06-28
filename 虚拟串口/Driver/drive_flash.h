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
 *
*/
/*ʹ��ʾ����
	dr_flashInit();
	dr_flashErase(0, Block);
	dr_flashWriteData(chi,0,500);
	sprintf((char*)ch,"%x",dr_flashReadJedecID());
	drv_lcdShowString(20,20,(uint8_t*)ch,Color[0]);
	dr_flashReadData((uint8_t*)ch,0,500);
	drv_lcdShowString(0,0,(uint8_t*)ch,Color[0]);
*/
/**********************************************************************************/
#ifndef __DRIVE_FLASH_H__
#define __DRIVE_FLASH_H__

typedef enum {Sector = 0x20, Block = 0xd8, Chip = 0xc7} EraseRange;

#define Dummy_Byte                  0xA5
#define Busy_Flag                   0x01

#define W25X_Write_ENABLE						0x06		//д����
#define W25X_Write_DISABLE					0x04		//д����
#define W25X_Read_Status_Register		0x05    //��״̬�Ĵ���
#define W25X_Write_Status_Register	0x01    //д״̬�Ĵ���
#define W25X_Read_Data							0x03    //������
#define W25X_Fast_Read              0x0b    //���
#define W25X_Fasr_Read_Dual_Output	0x3b    //���˫���
#define W25X_Page_Program						0x02		//ҳ���
#define W25X_Block_Erase						0xd8		//�����  64K
#define W25X_Sector_Rease						0x20		//�������� 4K
#define W25X_Chip_Erase							0xc7		//оƬ����
#define W25X_Power_Down							0xb9		//����
#define W25X_Device_ID							0xAB		//�ͷŵ���/����ID
#define W25X_JEDEC_ID								0x9f		//����JEDEC_ID

void dr_flashInit(void);
/*����1Byte����*/
u8 dr_flashSendByte(u8 byte);
/*W25Xд����   ENABLE ʹ��   DISABLE ����*/
void dr_flashCmd(FunctionalState NowState);
/*��W25X_Device_ID*/
u8 dr_flashReadDeviceID(void);
/*��W25x_JEDEC_ID*/
u32 dr_flashReadJedecID(void);
/*��W25X�Ĵ���״̬*/
u8 dr_flashReadStatus(void);
/*дW25X�Ĵ���״̬*/
void dr_flashWriteStatus(u8 NowStatus);
/*���W25X����
EraseAddr�����ַ
Rangge �����Χ(Sector  4K   Block  64K   Chip  ȫ��)*/
void dr_flashErase(u32 EraseAddr, EraseRange Rangge);
/*��W25XоƬд����*/
void dr_flashWritePage(u8 *Data, u32 WriteAddr, u16 WriteByte);
/*��W25XоƬ������*/
void dr_flashReadData(u8 *Data, u32 ReadAddr, u32 ReadByte);
/*��W25XоƬд����*/
void dr_flashWriteData(u8 *pBuffer, u32 WriteAddr, u32 NumByteToWrite);
#endif
