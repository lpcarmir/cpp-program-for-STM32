/*
 * GPIOClass.h
 *
 *  Created on: 1402-10-10
 *  Modified on: 1402-11-02
 *      Author: lpcarm.ir
 */

#ifndef CLASS_GPIOCLASS_H_
#define CLASS_GPIOCLASS_H_

#include "stm32f10x.h"

#define  GPIO_NOPULL        0x00000000u   /*!< No Pull-up or Pull-down activation  */
#define  GPIO_PULLUP        0x00000001u   /*!< Pull-up activation                  */
#define  GPIO_PULLDOWN      0x00000002u   /*!< Pull-down activation                */

typedef struct
{
	GPIO_TypeDef *GPIOx;
	uint8_t GPIO_Pin;
}GPIO_MAP_T;

const GPIO_MAP_T GPIO_MAP[]=
{
	{GPIOA,0},
	{GPIOA,1},
	{GPIOA,2},
	{GPIOA,3},
	{GPIOA,4},
	{GPIOA,5},
	{GPIOA,6},
	{GPIOA,7},
	{GPIOA,8},
	{GPIOA,9},
	{GPIOA,10},
	{GPIOA,11},
	{GPIOA,12},
	{GPIOA,13},
	{GPIOA,14},
	{GPIOA,15},
	{GPIOB,0},
	{GPIOB,1},
	{GPIOB,2},
	{GPIOB,3},
	{GPIOB,4},
	{GPIOB,5},
	{GPIOB,6},
	{GPIOB,7},
	{GPIOB,8},
	{GPIOB,9},
	{GPIOB,10},
	{GPIOB,11},
	{GPIOB,12},
	{GPIOB,13},
	{GPIOB,14},
	{GPIOB,15},
	{GPIOC,13},
	{GPIOC,14},
	{GPIOC,15},
	{GPIOD,0},
	{GPIOD,1},
};

typedef enum
{
	PA_0=0,
	PA_1,
	PA_2,
	PA_3,
	PA_4,
	PA_5,
	PA_6,
	PA_7,
	PA_8,
	PA_9,
	PA_10,
	PA_11,
	PA_12,
	PA_13,
	PA_14,
	PA_15,
	PB_0,
	PB_1,
	PB_2,
	PB_3,
	PB_4,
	PB_5,
	PB_6,
	PB_7,
	PB_8,
	PB_9,
	PB_10,
	PB_11,
	PB_12,
	PB_13,
	PB_14,
	PB_15,
	PC_13,
	PC_14,
	PC_15,
	PD_0,
	PD_1
}GPIO_NAME_T;

typedef enum
{
	GPIO_ANALOG=0,
	GPIO_INPUT_FLOAT,
	GPIO_INPUT_PULL_UP,
	GPIO_INPUT_PULL_DOWN,
	GPIO_OUTPUT_GP_PP,
	GPIO_OUTPUT_GP_OD,
	GPIO_OUTPUT_AF_PP,
	GPIO_OUTPUT_AF_OD
}GPIO_MODE_T;

typedef enum
{
	GPIO_INPUT,
	GPIO_OUTPUT_10MHZ,
	GPIO_OUTPUT_2MHZ,
	GPIO_OUTPUT_50MHZ
}GPIO_TYPE_T;

class GPIOClass
{
public:
	GPIOClass(){}
	GPIOClass(GPIO_NAME_T pinID,GPIO_MODE_T gpioMode=GPIO_INPUT_FLOAT);
	GPIOClass(GPIO_NAME_T pinID,GPIO_MODE_T gpioMode,GPIO_TYPE_T type);
	void on(void);
	void off(void);
	void toggle(void);
	void set(uint8_t val);
	uint8_t get(void);
	void dir(GPIO_MODE_T gpioMode);
	void dir(GPIO_MODE_T gpioMode,GPIO_TYPE_T type);
	void init(GPIO_NAME_T pinID,GPIO_MODE_T gpioMode,GPIO_TYPE_T type);
	void init(GPIO_NAME_T pinID,GPIO_MODE_T gpioMode);
private:
	GPIO_TypeDef *GPIO;
	uint8_t Pin;
	uint8_t state;
};

void GPIOClass::on(void)
{
	GPIO->BSRR=1<<Pin;
	state=1;
}

void GPIOClass::off(void)
{
	GPIO->BRR=1<<Pin;
	state=0;
}

void GPIOClass::toggle(void)
{
	if(state==1)
		off();
	else
		on();
}

void GPIOClass::set(uint8_t val)
{
	if(val==1)
		on();
	else
		off();
}

uint8_t GPIOClass::get(void)
{
	return (GPIO->IDR >> Pin) & 0x1;
}

void GPIOClass::dir(GPIO_MODE_T gpioMode)
{
	uint8_t data;
	
	if(Pin<8)
		GPIO->CRL&=~(0xF<<(Pin*4));
	else
		GPIO->CRH&=~(0xF<<((Pin-8)*4));
	
	switch(gpioMode)
	{
		case GPIO_ANALOG:
			data=0;
			break;
		
		case GPIO_INPUT_FLOAT:
			data=1<<2;
			break;
		
		case GPIO_INPUT_PULL_UP:
			data=1<<3;
			on();
			break;
		
		case GPIO_INPUT_PULL_DOWN:
			data=1<<3;
			off();
			break;
		
		case GPIO_OUTPUT_GP_PP:
			data=0;
			data|=3;//50MHz
			break;
		
		case GPIO_OUTPUT_GP_OD:
			data=1<<2;
			data|=3;//50MHz
			break;
		
		case GPIO_OUTPUT_AF_PP:
			data=1<<3;
			data|=3;//50MHz
			break;
		
		case GPIO_OUTPUT_AF_OD:
			data=3<<2;
			data|=3;//50MHz
			break;
	}
	
	if(Pin<8)
		GPIO->CRL|=(data<<(Pin*4));
	else
		GPIO->CRH|=(data<<((Pin-8)*4));
}

void GPIOClass::dir(GPIO_MODE_T gpioMode,GPIO_TYPE_T type)
{
	uint8_t data;
	
	if(Pin<8)
		GPIO->CRL&=~(0xF<<(Pin*4));
	else
		GPIO->CRH&=~(0xF<<((Pin-8)*4));
	
	switch(gpioMode)
	{
		case GPIO_ANALOG:
			data=0;
			break;
		
		case GPIO_INPUT_FLOAT:
			data=1<<2;
			break;
		
		case GPIO_INPUT_PULL_UP:
			data=1<<3;
			on();
			break;
		
		case GPIO_INPUT_PULL_DOWN:
			data=1<<3;
			off();
			break;
		
		case GPIO_OUTPUT_GP_PP:
			data=0;
			break;
		
		case GPIO_OUTPUT_GP_OD:
			data=1<<2;
			break;
		
		case GPIO_OUTPUT_AF_PP:
			data=1<<3;
			break;
		
		case GPIO_OUTPUT_AF_OD:
			data=3<<2;
			break;
	}
	
	switch(type)
	{
		case GPIO_OUTPUT_10MHZ:
			data|=1;
			break;
		case GPIO_OUTPUT_2MHZ:
			data|=2;
			break;
		case GPIO_OUTPUT_50MHZ:
			data|=3;
			break;
		default:
			break;
	}
	
	if(Pin<8)
		GPIO->CRL|=(data<<(Pin*4));
	else
		GPIO->CRH|=(data<<((Pin-8)*4));
}

GPIOClass::GPIOClass(GPIO_NAME_T pinID,GPIO_MODE_T gpioMode)
{
	init(pinID,gpioMode);
}

GPIOClass::GPIOClass(GPIO_NAME_T pinID,GPIO_MODE_T gpioMode,GPIO_TYPE_T type)
{
	init(pinID,gpioMode,type);
}

void GPIOClass::init(GPIO_NAME_T pinID,GPIO_MODE_T gpioMode,GPIO_TYPE_T type)
{
	GPIO_MAP_T portPin;
	RCC->APB2ENR|=(1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5);//GPIO clk en
	portPin=GPIO_MAP[pinID];
	GPIO=portPin.GPIOx;
	Pin=portPin.GPIO_Pin;
	dir(gpioMode,type);
}

void GPIOClass::init(GPIO_NAME_T pinID,GPIO_MODE_T gpioMode)
{
	GPIO_MAP_T portPin;
	RCC->APB2ENR|=(1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5);//GPIO clk en
	portPin=GPIO_MAP[pinID];
	GPIO=portPin.GPIOx;
	Pin=portPin.GPIO_Pin;
	dir(gpioMode);
}



#endif /* CLASS_GPIOCLASS_H_ */
