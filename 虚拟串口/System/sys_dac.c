#include "stm32f10x.h"
#include "sys_dac.h"

void sys_dacInit(void)
{
	DAC_InitTypeDef DAC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);
	DAC_Cmd(DAC_Channel_1, ENABLE);
	sys_dacOutput(0);
}

/*******************************************************************************
* ��������: sys_dacOutput      // �������ơ�
* ˵��:          // �������ܡ����ܵȵ�������
* �������:      // �������˵��������ÿ������������
*                // �á�ȡֵ˵�����������ϵ��
* �������:      // �����������˵����
* ����ֵ:        // ��������ֵ��˵����
* ����:          // ����˵����ѡ���
*******************************************************************************/
/*******************************************************************************
* function:	sys_dacOutput
* ˵��:
* input:	none
*
* output:	none
* return:	none
* other:	none
*******************************************************************************/
void sys_dacOutput(uint16_t value)
{
	DAC_SetChannel1Data(DAC_Align_12b_R, value);
}

//void DAC_DMA_Out_Config(void)
//{
//	DAC_InitTypeDef DAC_InitStructure;
//	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
//	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
//	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
//	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
//	DAC_Init(DAC_Channel_1, &DAC_InitStructure);
//	DAC_Cmd(DAC_Channel_1, ENABLE);
//	DAC_SetChannel1Data(DAC_Align_12b_R, 0);
//}
//void DAC_Triangle_Config(void)
//{
//	DAC_InitTypeDef DAC_InitStructure;
//	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
//	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
//	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
//	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
//	DAC_Init(DAC_Channel_1, &DAC_InitStructure);
//	DAC_Cmd(DAC_Channel_1, ENABLE);
//	DAC_SetChannel1Data(DAC_Align_12b_R, 0);
//}
