#ifndef __HWA_MODBUS_H__
#define __HWA_MODBUS_H__

#include "typedef.h"

#define MAX_DARA_SIZE		(8)

typedef enum
{
  broadcast 		= 0u,
  getVoltage,
  getElectricity,


  returnBroadcast = broadcast | 0x80,
  returnVoltage,
  returnElectricity,


  NUM_OF_modbusFun,
} modbusFun;

typedef struct _MSG_STRUCT
{
	UINT8 addr;
	modbusFun fun;
	UINT8 len;
	UINT8 data[MAX_DARA_SIZE];
	UINT8 crc[2];
} MSG_STRUCT;

typedef struct
{
	BOOL rxflag;
	MSG_STRUCT msg;
} MODBUS_STRUCT;

extern MODBUS_STRUCT s_modbus;

#define RXSIZE			(MAX_DARA_SIZE+10)
#define TXSIZE			(MAX_DARA_SIZE+10)
extern volatile UINT8 RxBuff[RXSIZE];
extern volatile UINT8 TxBuff[TXSIZE];

void ModBusInit(void);
void Rx_interrupt(UINT8 ch);
void Send(UINT8 addr, modbusFun fun, UINT8 *data, UINT8 size);

#endif
