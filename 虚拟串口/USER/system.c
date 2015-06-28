#include "system.h"

void sys_GPIO_Init( GPIO_TypeDef* GPIOx,\
                    UINT16 GPIO_Pin,\
                    GPIOMode_TypeDef GPIO_Mode,\
                    GPIOSpeed_TypeDef GPIO_Speed)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    if(GPIOx == GPIOA)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    }
    else if(GPIOx == GPIOB)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    }
    else if(GPIOx == GPIOC)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    }
    else if(GPIOx == GPIOD)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    }
    else if(GPIOx == GPIOE)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    }
    else if(GPIOx == GPIOF)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
    }
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed;
	GPIO_Init(GPIOx, &GPIO_InitStructure);
}
/*LED��ʼ��*/
//void LED_Init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
//	LED1 = 0;
//	LED2 = 0;
//}
void LED_Init(void)
{
    sys_GPIO_Init(GPIOB, GPIO_Pin_8, GPIO_Mode_Out_PP, GPIO_Speed_10MHz);
	LED1 = 0;
}

/*��ʱnus����*/
void Delay_us(uint16_t xus)
{
	volatile uint8_t i;
	while(xus--)for(i = 0; i < 9; i++);
}
/*��ʱnms����*/
void Delay_ms(uint16_t xms)
{
	while(xms--) { Delay_us(1000); }
}

/*��ȡоƬID*/
uint32_t STM32ID[3];
void GetDeviceSerialID(void)
{
	STM32ID[0] = *(__IO uint32_t *)(0x1FFFF7E8);
	STM32ID[1] = *(__IO uint32_t *)(0x1FFFF7EC);
	STM32ID[2] = *(__IO uint32_t *)(0x1FFFF7F0);
}

/*��ʼ��*/
void SYSTEM_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//�ж�������
//	GetDeviceSerialID();
	LED_Init();
//	BEEP_Init();
	SysTick_Config(SystemCoreClock / 1000);
}
