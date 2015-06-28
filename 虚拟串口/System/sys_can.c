#include "stm32f10x.h"
#include "sys_can.h"

CanTxMsg CAN_TxMessage;
CanRxMsg CAN_RxMessage;

static void CAN_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
	/* Configure CAN pin: RX */									               // PB8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* Configure CAN pin: TX */									               // PB9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

static void CAN_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

static void CAN_Mode_Config(void)
{
	CAN_InitTypeDef CAN_InitStructure;
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	CAN_InitStructure.CAN_TTCM = DISABLE;			 //MCR-TTCM  �ر�ʱ�䴥��ͨ��ģʽʹ��
	CAN_InitStructure.CAN_ABOM = ENABLE;			 //MCR-ABOM  �Զ����߹���
	CAN_InitStructure.CAN_AWUM = ENABLE;			 //MCR-AWUM  ʹ���Զ�����ģʽ
	CAN_InitStructure.CAN_NART = DISABLE;			 //MCR-NART  ��ֹ�����Զ��ش�	  DISABLE-�Զ��ش�
	CAN_InitStructure.CAN_RFLM = DISABLE;			 //MCR-RFLM  ����FIFO ����ģʽ  DISABLE-���ʱ�±��ĻḲ��ԭ�б���
	CAN_InitStructure.CAN_TXFP = DISABLE;			 //MCR-TXFP  ����FIFO���ȼ� DISABLE-���ȼ�ȡ���ڱ��ı�ʾ��
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_3tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_8tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
	CAN_InitStructure.CAN_Prescaler = 6;
	CAN_Init(CAN1, &CAN_InitStructure);
}

static void CAN_Filter_Config(void)
{
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;                     //�����ڱ�ʶ������ģʽ
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;                    //������λ��Ϊ����32λ��
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x400 << 5;                              //Ҫ���˵�ID��λ
	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_RTR_Data | CAN_Id_Standard;           //Ҫ���˵�ID��λ
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xffff;								//����0x400-0x4ff���������ID
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0xffff;								//���ηǱ�׼����֡
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0 ;				//��������������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;                              //ʹ�ܹ�����
	CAN_FilterInit(&CAN_FilterInitStructure);
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

void sys_canInit(void)
{
	CAN_GPIO_Config();
	CAN_NVIC_Config();
	CAN_Mode_Config();
	CAN_Filter_Config();
}

extern CanRxMsg CAN_RxMessage;

void USB_LP_CAN1_RX0_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
	{
		CAN_Receive(CAN1, CAN_FIFO0, &CAN_RxMessage);
	}
	do
	{
		CAN_FIFORelease(CAN1, CAN_FIFO0);
	}
	while(CAN_MessagePending(CAN1, CAN_FIFO0));
}

void CAN1_RX1_IRQHandler(void)
{
	CAN_FIFORelease(CAN1, CAN_FIFO1);
}

void CAN1_SCE_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN1, CAN_IT_ERR) != RESET)
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_ERR);
	}
}

