#include "hwa_modbus.h"
#include "typedef.h"
#include "stdio.h"
#include "string.h"

/*
Modbus Format

host cmd format
********************************************************************
fun		  | addr | fun | regh | regl | datah | datal | crc8h | crc8l |
********************************************************************
size	  |   1  |  1  |   1  |   1  |   1   |   1   |   1   |   1   |
********************************************************************

slave data format
************************************************************
fun		  | addr | fun | dataLen | data0...n | crc8h | crc8l |
************************************************************
size	  |   1  |  1  |    1    |     n     |   1   |   1   |
************************************************************
*/

typedef struct _CMD_STRUCT
{
	UINT8 addr;
	modbusFun fun;
	UINT8 reg[2];
	UINT8 data[2];
	UINT8 crc[2];
} CMD_STRUCT;

typedef struct _DATA_STRUCT
{
	UINT8 addr;
	modbusFun fun;
	UINT8 len;
	UINT8 data[MAX_DARA_SIZE];
	UINT8 crc[2];
} DATA_STRUCT;

volatile CMD_STRUCT st_HostCmd;
volatile DATA_STRUCT st_SlaveData;



















/*
#define SURPLUSLEN			(5)
#define RXSIZE			(MAX_DARA_SIZE+10)
#define TXSIZE			(MAX_DARA_SIZE+10)

volatile UINT8 RxBuff[RXSIZE];
volatile UINT8 TxBuff[TXSIZE];

BOOL Rx_Flag = FALSE;
MODBUS_STRUCT s_modbus;

void Uart_send(UINT8* data, UINT8 len);

void Uart_send(UINT8* data, UINT8 len)
{

}
void ModBusInit(void)
{
	s_modbus.rxflag = FALSE;
}
unsigned short CRC16RTU(unsigned char *pszBuf, unsigned short unLength);
void Send(UINT8 addr, modbusFun fun, UINT8* data, UINT8 size)
{
	UINT16 crc;
	UINT8 i, index = 0;

	if(size > MAX_DARA_SIZE)
	{
		return;
	}
	TxBuff[index++] = 0xA5;
	TxBuff[index++] = 0xA5;

	TxBuff[index++] = addr;
	TxBuff[index++]= fun;
	TxBuff[index++] = size;

	crc = CRC16RTU(data, size);

	for(i=0; i<size; i++)
	{
		TxBuff[index++]= *data++;
	}

	TxBuff[index++] = (UINT8)(crc>>8);
	TxBuff[index++] = (UINT8)crc;

	TxBuff[index++] = 0xA5;
	TxBuff[index++] = 0xA5;

	Uart_send((UINT8*)TxBuff, index);
}
static void rx_handle(void)
{
	UINT8 i, j = 3;
	s_modbus.msg.addr = RxBuff[0];
	s_modbus.msg.fun = RxBuff[1];
	s_modbus.msg.len = RxBuff[2];
	for(i=0; i< RxBuff[2]; i++)
	{
		s_modbus.msg.data[i] = RxBuff[j++];
	}
	for(i=RxBuff[2]; i<MAX_DARA_SIZE; i++)
	{
		s_modbus.msg.data[i] = 0x00;
	}
	s_modbus.msg.crc[0] = RxBuff[j++];
	s_modbus.msg.crc[1] = RxBuff[j];
	s_modbus.rxflag = TRUE;
}
#define get_ch		getchar

void Rx_interrupt(UINT8 ch)
{
	static UINT8 index = 0;
	static UINT8 count = 0;
	static BOOL startfalg = FALSE;
	static BOOL stopfalg = FALSE;
	UINT16 crc;
//	UINT8 ch;
//	ch = get_ch();
	if(startfalg == FALSE)
	{
		if(ch == 0xA5)
		{
			count++;
			if(count == 2)
			{
				startfalg = TRUE;				//stat to reveice
				stopfalg = FALSE;
				index = 0;
				count = 0;
			}
		}
	}
	else
	{
		RxBuff[index++] = ch;
		if(stopfalg == FALSE)
		{
			if(ch == 0xA5)
			{
				count++;
				if(count == 2)
				{
					startfalg = FALSE;
					stopfalg = TRUE;
					index = 0;
					count = 0;
					crc = CRC16RTU((UINT8*)RxBuff, RxBuff[2]+SURPLUSLEN);
					if(crc == 0x0000)
					{
						if(RxBuff[2] > MAX_DARA_SIZE)
							return;
						rx_handle();
						Rx_Flag = TRUE;				//reveice succeed
					}
				}
			}
		}
	}
	if(Rx_Flag == TRUE)
	{
		Rx_Flag = FALSE;
	}
}
*/










