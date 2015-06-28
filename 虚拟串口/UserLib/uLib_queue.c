#include "uLib_queue.h"
#include "string.h"

#define IS_QUEUE_EMPTY(__q)       (__q->use_len==0)
#define IS_QUEUE_FULL(__q)        (__q->use_len==__q->member_num)


UINT8 QueueMaxLen = 0;

void uLib_q_Create(LIB_QUEUE *q, void *buff, UINT8 member_size, UINT8 member_num)
{
	q->Read = 0;
	q->Write = 0;
	q->pBuff = buff;
	q->member_size = member_size;
	q->member_num = member_num;
	q->use_len = 0;
}

void uLib_q_Insert(LIB_QUEUE *q, void *member)
{
	UINT8 *pWrite;
	if(!IS_QUEUE_FULL(q))
	{
		pWrite = (UINT8 *)q->pBuff + q->Write * q->member_size;
		memcpy(pWrite, (UINT8 *)member, q->member_size);
		q->Write = (q->Write + 1) % q->member_num;
		q->use_len++;
		if(QueueMaxLen < q->use_len)
		{ QueueMaxLen = q->use_len; }
	}
}

void uLib_q_Delete(LIB_QUEUE *q)
{
	if(!IS_QUEUE_EMPTY(q))
	{
		q->Read = (q->Read + 1) % q->member_num;
		q->use_len--;
#ifdef USE_MEASURING_QUEUE_LEN
		if(q->max_use_len < q->use_len)
		{
			q->max_use_len = q->use_len;
		}
#endif
	}
}

void *uLib_q_First(LIB_QUEUE *q)
{
	UINT8 *pRead;
	if(!IS_QUEUE_EMPTY(q))
	{
		pRead = (UINT8 *)q->pBuff + q->Read * q->member_size;
	}
	else
	{
		pRead = NULL;
	}
	return (void *)pRead;
}
