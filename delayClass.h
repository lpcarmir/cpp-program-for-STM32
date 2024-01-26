#ifndef __DELAY_CLASS_H__
#define __DELAY_CLASS_H__

#include "stdint.h"

class delayClass
{
	public:
		void us(uint16_t x);
		void ms(uint32_t x);
	
	private:
};

void delayClass::us(uint16_t x)
{
	//__asm("nop");
	x=x>>4;
	while(x>0)
		x--;
}

void delayClass::ms(uint32_t x)
{
	while(x>0)
	{
		x--;
		for(uint8_t i=0;i<110;i++);
	}
}

#endif //__DELAY_CLASS_H__
