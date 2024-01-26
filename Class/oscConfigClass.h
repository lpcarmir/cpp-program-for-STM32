#ifndef __OSC_CONFIG_H__
#define __OSC_CONFIG_H__

#include "stm32f10x.h"

class oscConfigClass
{
	public:
		oscConfigClass(uint32_t fin);
		oscConfigClass(uint32_t fin,uint32_t fosc);
		uint32_t getFreq();
	
	private:
		uint32_t fosc;
};

oscConfigClass::oscConfigClass(uint32_t fin,uint32_t fosc)
{
	uint8_t multiplier;
	RCC->CR|=1<<16;//HSE=1
	while(!(RCC->CR & (1<<17)));//wait HSE ready
	
	/*if(fosc>72000000)
		throw("Error Freq Too Much");*/
	
	multiplier=fosc/fin;
	this->fosc=fin*multiplier;
	
	if(multiplier>=2)
		multiplier-=2;
	else
	{
		this->fosc=fin;//plase change it after compelete this method
		return;
	}
	
	RCC->CFGR&=~((0xF<<18)|(0x7<<8));//clear PPRE1 and PLLMUL
	RCC->CFGR|=multiplier<<18 | (1<<16);
	//                     PLLSRC=HSE
	
	multiplier=3;
	while(fosc>36000000)
	{
		multiplier++;
		fosc/=2;
	}
	RCC->CFGR|=multiplier<<8;
	
	RCC->CR|=1<<24;//PLL=1
	while(!(RCC->CR & (1<<25)));//wait PLL ready
	
	RCC->CFGR|=1<<1;//PLL set as clock system
}

oscConfigClass::oscConfigClass(uint32_t fin)
{
	fosc=fin;
}

uint32_t oscConfigClass::getFreq()
{
	return fosc;
}

#endif //__OSC_CONFIG_H__
