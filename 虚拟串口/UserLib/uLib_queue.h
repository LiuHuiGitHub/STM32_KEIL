#ifndef __USER_LIB_QUEUE_H__
#define __USER_LIB_QUEUE_H__

#include "string.h"

#ifndef __TYPEDEF_H__
typedef unsigned char         UINT8;
typedef unsigned short        UINT16;
#endif

#define USE_MEASURING_QUEUE_LEN

typedef struct
{
	UINT8 Read;
	UINT8 Write;
	UINT8 member_size;
	UINT8 member_num;
	UINT8 use_len;
#ifdef USE_MEASURING_QUEUE_LEN
	UINT8 max_use_len;
#endif
	void *pBuff;
} LIB_QUEUE;

void uLib_q_Create(LIB_QUEUE *q, void *buff, UINT8 member_size, UINT8 member_num);
void uLib_q_Insert(LIB_QUEUE *q, void *member);
void uLib_q_Delete(LIB_QUEUE *q);
void *uLib_q_First(LIB_QUEUE *q);

#endif
