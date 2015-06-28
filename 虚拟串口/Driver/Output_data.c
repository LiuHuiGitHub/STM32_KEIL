/*
================================================================================
*	  ����ʾ��������ģ��
*	  �ļ���Output_data.c
*	  ������ͨ�����ڽ����ݷ���������ʾ����
*     ˵��������Ӵ���ͨ��ģ��USART.c
================================================================================
*/
#include "Output_data.h"

extern void sys_usartSendData(unsigned char *Data, unsigned int SendSize);
/*
================================================================================
* ����        : OutPut_Data()
* ����        : �������ֵ���͸�����ʾ������ʾ
* ����        : signed short OutData[4]
* ���        : ��
* ˵��        : ��
================================================================================
*/


void OutPut_Data(short int *OutData)
{
	unsigned char databuf[10] = {0};
	unsigned char i, j;
	unsigned short CRC_Temp = 0xffff;
	for(i = 0; i < 4; i++)
	{
		databuf[i * 2] = (unsigned char)(OutData[i] & 0xff);
		databuf[i * 2 + 1] = (unsigned char)(OutData[i] >> 8);
	}
	for(i = 0; i < 8; i++)
	{
		CRC_Temp ^= databuf[i];
		for(j = 0; j < 8; j++)
		{
			if(CRC_Temp & 0x01)
			{ CRC_Temp = (CRC_Temp >> 1) ^ 0xa001; }
			else
			{ CRC_Temp = CRC_Temp >> 1; }
		}
	}
	databuf[8] = CRC_Temp & 0xff;
	databuf[9] = CRC_Temp >> 8;
	sys_usartSendData(databuf, 10);
}
