#ifndef __LCD_CLASS_H__
#define __LCD_CLASS_H__

/*
example:
	lcd.init();
	lcd.writeData('I');
	lcd.writeData('O');
	lcd.writeData('T');
	"IOT Class"
	*/

#include "GPIOClass.h"
#include "delayClass.h"

extern delayClass delay;

class LCDClass
{
	public:
		LCDClass(){}
		LCDClass(GPIO_NAME_T rsPin,GPIO_NAME_T enPin,GPIO_NAME_T d4Pin,GPIO_NAME_T d5Pin,GPIO_NAME_T d6Pin,GPIO_NAME_T d7Pin);
		void init();
		void writeData(uint8_t data);
		void writeCmd(uint8_t data);
		void clear();
		void gotoXY(uint8_t x,uint8_t y);
	
	private:
		void put4bit(uint8_t x);
		GPIOClass rs;
		GPIOClass en;
		GPIOClass d4;
		GPIOClass d5;
		GPIOClass d6;
		GPIOClass d7;
};

LCDClass::LCDClass(GPIO_NAME_T rsPin,GPIO_NAME_T enPin,GPIO_NAME_T d4Pin,GPIO_NAME_T d5Pin,GPIO_NAME_T d6Pin,GPIO_NAME_T d7Pin)
{
	rs.init(rsPin,GPIO_OUTPUT_GP_PP);
	en.init(enPin,GPIO_OUTPUT_GP_PP);
	d4.init(d4Pin,GPIO_OUTPUT_GP_PP);
	d5.init(d5Pin,GPIO_OUTPUT_GP_PP);
	d6.init(d6Pin,GPIO_OUTPUT_GP_PP);
	d7.init(d7Pin,GPIO_OUTPUT_GP_PP);
}

void LCDClass::init()
{
	rs.off();
	put4bit(0x3);
	put4bit(0x3);
	put4bit(0x3);
	put4bit(0x2);//mode 4bit
	writeCmd(0x28);//mode 4bit 2line
	writeCmd(0x0E);//display on cursor on
	clear();
}

void LCDClass::clear()
{
	writeCmd(0x1);
	delay.ms(2);//2ms
}

void LCDClass::gotoXY(uint8_t x,uint8_t y)
{
	if(y==1)
	{
		writeCmd(0x80|0x40|x);
	}
	else
	{
		writeCmd(0x80|x);
	}
}

void LCDClass::writeData(uint8_t data)
{
	rs.on();
	put4bit(data>>4);//4bit MSB
	put4bit(data);//4bit LSB
	delay.us(50);//40us
}

void LCDClass::writeCmd(uint8_t data)
{
	rs.off();
	put4bit(data>>4);//4bit MSB
	put4bit(data);//4bit LSB
	delay.us(50);//40us
}

void LCDClass::put4bit(uint8_t x)
{
	d4.set(x%2);
	x=x>>1;
	d5.set(x%2);
	x=x>>1;
	d6.set(x%2);
	x=x>>1;
	d7.set(x%2);
	en.on();
	delay.us(1);//>450ns
	en.off();
}


#endif //__LCD_CLASS_H__
