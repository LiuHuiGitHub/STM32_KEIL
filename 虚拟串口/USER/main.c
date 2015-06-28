#include "include.h"
#include "sys.h"
#include "hwa_key.h"
#include "hw_config.h"
#include "sys_exti.h"
#include "sys_tim.h"

void sys_Init(void)
{
	SYSTEM_Init();
//    sys_adcInit();
//	drv_usart1Init(115200);
//	sys_usartInit(USART2, 115200);
//	sys_usartInit(USART3, 115200);
    USB_Config();
}


void app_Init(void)
{
}

void taskLED1(void)
{
    LED1 = !LED1;
}

void taskUart(void)
{
    uint32_t len = 0;
    static uint8_t buf[200] = {0};
    len = USB_RxRead(buf, sizeof(buf));
    if (len > 0)
    {
        USB_TxWrite(buf, len);
    }
}

int main(void)
{
	sys_Init();
	app_Init();
	sys_taskInit();
    sys_extiInit(EXTI_PORTB, EXTI_PIN_9, EXTI_Trigger_Rising, taskLED1);
    sys_extiInit(EXTI_PORTB, EXTI_PIN_10, EXTI_Trigger_Rising, taskLED1);
    sys_timInit(TIM_4, 72, 1000, taskLED1);
    sys_timOpen(TIM_4);
//    sys_taskAdd(taskLED1, 500, 1);
    sys_taskAdd(taskUart, 10, 2);
	sys_taskStart();
	return 0;
}

void SysTick_Handler1ms(void)						//1ms interrupt ,and turn time max 1ms
{
}

#ifdef  USE_FULL_ASSERT
volatile UINT8 *pfile;
volatile UINT32 pline;
void assert_failed(uint8_t *file, uint32_t line)
{
	pfile = file;
	pline = line;
	while(1);
}
#endif

