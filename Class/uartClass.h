/*
 * uartClass.h
 *
 *  Created on: 1402-11-02 20:47
 *      Author: lpcarm
 */

#ifndef CLASS_UARTCLASS_H_
#define CLASS_UARTCLASS_H_

#include "stm32f10x.h"
#include "stdint.h"

#include "GPIOClass.h"

class uartClass
{
	
public:
	uartClass(){}
	uartClass(uint8_t uartNumber,GPIO_NAME_T txPin,GPIO_NAME_T rxPin,uint32_t fosc=8000000,uint32_t baud=9600);
	void send(uint8_t data);
	void sendIT(uint8_t data);
	uint8_t getData(void);
	uint8_t getStatus(void);
	void setStatus(uint8_t x);
		
private:
	USART_TypeDef *USART;
	uint16_t calculateBaud(uint32_t fosc,uint32_t baud);
	//uint8_t uartNumber;

};

uint16_t uartClass::calculateBaud(uint32_t fosc,uint32_t baud)
{
	float tmp;
	
	tmp=fosc;
	tmp/=16;
	tmp/=baud;
	
	tmp*=16;//get 4bit of fraction
	
	return (uint16_t)tmp;
}

uartClass::uartClass(uint8_t uartNumber,GPIO_NAME_T txPin,GPIO_NAME_T rxPin,uint32_t fosc,uint32_t baud)
//:uartNumber(uartNumber)
{
	GPIOClass rx;
	GPIOClass tx;
	
	//TX Pin
	tx.init(txPin,GPIO_OUTPUT_AF_PP,GPIO_OUTPUT_2MHZ);
	
	//Rx Pin
	tx.init(rxPin,GPIO_INPUT_PULL_UP);
	
	switch(uartNumber)
	{
		case 1://TX,RX pin = PA9,PA10 or PB6,PB7
			RCC->APB2ENR|=1<<14;//UART1 Clock En
			USART=USART1;
			NVIC_EnableIRQ(USART1_IRQn);
			break;
		case 2:
			RCC->APB1ENR|=1<<17;//UART2 Clock En
			NVIC_EnableIRQ(USART2_IRQn);
			USART=USART2;
			break;
	}

	USART->BRR=calculateBaud(fosc,baud);
	
	USART->CR1=(1<<13)|(1<<6)|(1<<5)|(1<<2)|(1<<3);
	//           UE     TCIE  RXNEIE   RE     TE
}

void uartClass::send(uint8_t data)
{
	getStatus();
	USART->DR=data;
	while(!(getStatus() & (1<<7)));
}

void uartClass::sendIT(uint8_t data)
{
	USART->DR=data;
}

uint8_t uartClass::getData(void)
{
	return USART->DR;
}

uint8_t uartClass::getStatus(void)
{
	return USART->SR;
}

void uartClass::setStatus(uint8_t x)
{
	USART->SR=x;
}

#endif /* CLASS_UARTCLASS_H_ */
