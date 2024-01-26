#ifndef __TIMER_CLASS_H__
#define __TIMER_CLASS_H__

#include "stm32f10x.h"
#include <stdint.h>

class timerClass
{
	public:
		timerClass(uint8_t timerNumber,uint32_t fosc,uint16_t time_ms);
		void clearInt();
		uint8_t getHandle(){return timerNumber;}
	private:
		TIM_TypeDef *tim;
		uint8_t timerNumber;
};

void timerClass::clearInt()
{
	tim->SR=0;//clear UIF bit
}

timerClass::timerClass(uint8_t timerNumber,uint32_t fosc,uint16_t time_ms)
:timerNumber(timerNumber)
{
	float tmp;
	uint16_t psc=1;
	
	switch(timerNumber)
	{
		case 1:
			tim=TIM1;
			break;
		case 2:
			tim=TIM2;
			break;
		case 3:
			tim=TIM3;
			break;
	}
	
	tmp=(fosc/1000)*time_ms;
	
	while(tmp>65535)
	{
		tmp/=2;
		psc*=2;
	}
	
	if(tim==TIM1)
	{
		RCC->APB2ENR|=(1<<11);//timer1 clock enable
		NVIC_EnableIRQ(TIM1_UP_IRQn);
	}
	if(tim==TIM2)
	{
		RCC->APB1ENR|=(1<<0);//timer2 clock enable
		NVIC_EnableIRQ(TIM2_IRQn);
	}
	if(tim==TIM3)
	{
		RCC->APB1ENR|=(1<<1);//timer3 clock enable
		NVIC_EnableIRQ(TIM3_IRQn);
	}
	
	tim->ARR=tmp-1;
	tim->PSC=psc-1;
	
	tim->CR1=1<<0;//enable count
	tim->DIER=1<<0;//enable update int
}

extern void timer_interrupt_fn(uint8_t timerNumber);

extern "C"
{
	void TIM1_UP_IRQHandler(void)
	{
		timer_interrupt_fn(1);
	}
	void TIM2_IRQHandler(void)
	{
		timer_interrupt_fn(2);
	}
	void TIM3_IRQHandler(void)
	{
		timer_interrupt_fn(3);
	}
}

#endif //__TIMER_CLASS_H__
