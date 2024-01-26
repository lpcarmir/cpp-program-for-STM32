/*
 * uartClass.h
 *
 *  Created on: 1402-11-03 19:04
 *      Author: lpcarm
 */

#ifndef __UART_SERVICE_CLASS_H__
#define __UART_SERVICE_CLASS_H__

#include "stm32f10x.h"
#include "stdint.h"

#include "GPIOClass.h"
#include "uartClass.h"
#include "FIFOClass.h"

extern void rxDataProcess(uint8_t *x,uint8_t size);

class uartServiceClass : private uartClass
{
	public:
		uartServiceClass(uint8_t uartNumber,GPIO_NAME_T txPin,GPIO_NAME_T rxPin,uint32_t fosc,uint32_t baud=9600);
		void run();
		uint8_t getBufferSize();
		void push(uint8_t *data);
		void pushf(const uint8_t *data);
		uartServiceClass operator<<(const uint8_t data);
		void processRxData(void);
		void process(void);
	
	private:
		#define TX_BUFF_SIZE 100
		#define RX_BUFF_SIZE 100
		uint8_t flagTx;
		FIFOClass fifoTx;
		uint8_t rxBuff[RX_BUFF_SIZE],rxCnt;
};

uartServiceClass::uartServiceClass(uint8_t uartNumber,GPIO_NAME_T txPin,GPIO_NAME_T rxPin,uint32_t fosc,uint32_t baud)
:uartClass(uartNumber,txPin,rxPin,fosc,baud)
{
	fifoTx.init(TX_BUFF_SIZE);
}

uint8_t uartServiceClass::getBufferSize()
{
	return fifoTx.isDataAvailable();
}

void uartServiceClass::push(uint8_t *data)
{
	while(*data!=0)
	{
		fifoTx.write(*data++);
	}
}

void uartServiceClass::pushf(const uint8_t *data)
{
	while(*data!=0)
	{
		fifoTx.write(*data++);
	}
}


void uartServiceClass::processRxData(void)
{
	uint8_t tmp;
	tmp=getData();
	
	if(tmp>' ')//showable
	{
		if(rxCnt<RX_BUFF_SIZE)
			rxBuff[rxCnt++]=tmp;
	}
	else
	{
		rxBuff[rxCnt]=0;//NULL
		//process rxBuff
		rxDataProcess(rxBuff,rxCnt);
		rxCnt=0;
	}
}

uartServiceClass uartServiceClass::operator<<(const uint8_t data)
{
	fifoTx << data;
	return *this;
}

void uartServiceClass::process(void)
{
	uint8_t status;
	status=getStatus();
	
	if(status & (1<<6))//Transmit Compelete
	{
		if(fifoTx.isEmpty())
		{
			setStatus(0);
			flagTx=0;
		}
		else
		{
			sendIT(fifoTx.read());
		}
	}
	else //rx int
	{
		processRxData();
	}
}

void uartServiceClass::run()
{
	if(fifoTx.isEmpty())
	{
		flagTx=0;
	}
	else
	{
		if(flagTx==0)
		{
			flagTx=1;
			sendIT(fifoTx.read());
		}
	}
}

#endif /* __UART_SERVICE_CLASS_H__ */
