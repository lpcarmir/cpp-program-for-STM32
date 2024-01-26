#include "stm32f10x.h"
#include "delayClass.h"
#include "uartServiceClass.h"
#include "stdio.h"
#include "string.h"
#include "timerClass.h"
#include "LCDServiceClass.h"
#include "oscConfigClass.h"

oscConfigClass osc(8000000,42000000);

GPIOClass led(PC_13,GPIO_OUTPUT_GP_PP);
GPIOClass key(PC_14,GPIO_INPUT_PULL_UP);

delayClass delay;
uartServiceClass uart1Service(1,PA_9,PA_10,osc.getFreq(),115200);

timerClass timer_1ms(3,osc.getFreq(),1);
timerClass timer_10ms(2,osc.getFreq(),10);
timerClass timer_100ms(1,osc.getFreq(),100);

LCDServiceClass lcdService(PA_0,PA_1,PA_2,PA_3,PA_4,PA_5);

uint8_t data[80]="IOT Class",size,size2;

int main(void)
{
	while(lcdService.isBusy());
	
	size=sprintf((char *)data,"%c       IOT%c      Class",0x1,0x2);
	lcdService.push(data,size);
	
	size=sprintf((char *)data,"IOT\n\rClass");
	size2=uart1Service.getBufferSize();
	if(size<size2)
		uart1Service.push(data);
	while(1)
	{
		/*led.toggle();
		delay.us(10);
		led.toggle();*/
	}
}

void timer_interrupt_fn(uint8_t timerNumber)
{
	if(timerNumber==timer_1ms.getHandle())
	{
		timer_1ms.clearInt();
		lcdService.run();
		uart1Service.run();
	}
	if(timerNumber==timer_10ms.getHandle())
	{
		timer_10ms.clearInt();
	}
	if(timerNumber==timer_100ms.getHandle())
	{
		timer_100ms.clearInt();
	}
}

void rxDataProcess(uint8_t *x,uint8_t size)
{
	if(!strcmp((char *)x,"LED_ON"))
	{
		led.on();
	}
	if(!strcmp((char *)x,"LED_OFF"))
	{
		led.off();
	}
}

extern "C"
{

	void USART1_IRQHandler(void)
	{
		uart1Service.process();
	}

	void USART2_IRQHandler(void)
	{
		
	}
	
	void HardFault_Handler(void)
	{
		NVIC_SystemReset();
	}

}
