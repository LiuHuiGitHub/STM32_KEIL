#ifndef __SYS_USART_H__
#define __SYS_USART_H__

#include "stm32f10x.h"

#define USART1_REMAP								0     //0��ӳ��
#define USART2_REMAP								1
#define USART3_PARTIALREMAP					0			//������ӳ��
#define USART3_FULLREMAP						0			//��ȫ��ӳ��

void sys_usartInit(USART_TypeDef *USART_x, u32 Boud);
void sys_usartSendData(uint8_t *Data, uint32_t SendSize);
void sys_usartTransmit(USART_TypeDef *Usartx, uint8_t *ptr, uint16_t size);

#endif
