#ifndef __LCD_CONTROLLER_CLASS_H__
#define __LCD_CONTROLLER_CLASS_H__

#include "LCDClass.h"

class LCDServiceClass: private LCDClass
{
	public:
		LCDServiceClass(GPIO_NAME_T rsPin,GPIO_NAME_T enPin,GPIO_NAME_T d4Pin,GPIO_NAME_T d5Pin,GPIO_NAME_T d6Pin,GPIO_NAME_T d7Pin);
		void run(void);
		bool isBusy(){return busy;}
		bool push(uint8_t *data,uint8_t size);
		
	private:
		uint8_t lcdBuffer[80];
		uint16_t cnt;
		//void init();//function override
		void init2();
		bool busy;
		uint8_t size,index,sizeTmp;
};

bool LCDServiceClass::push(uint8_t *data,uint8_t size)
{
	if(isBusy())
	{
		return false;
	}
	else
	{
		for(uint8_t i=0;i<size;i++)
		{
			lcdBuffer[i]=data[i];
		}
		this->size=size;
		sizeTmp=size;
		busy=true;
		index=0;
		return true;
	}
}

LCDServiceClass::LCDServiceClass(GPIO_NAME_T rsPin,GPIO_NAME_T enPin,GPIO_NAME_T d4Pin,GPIO_NAME_T d5Pin,GPIO_NAME_T d6Pin,GPIO_NAME_T d7Pin)
:LCDClass(rsPin,enPin,d4Pin,d5Pin,d6Pin,d7Pin)//member initializer
{
	cnt=500;
	sizeTmp=0;
	busy=true;
}

void LCDServiceClass::run(void)//call every 1ms
{
	cnt--;
	if(cnt==0)
	{
		cnt=60000;
		init2();
		if(sizeTmp>0)
		{
			this->size=sizeTmp;
			index=0;
			busy=true;
		}
		return;
	}
	
	if(size>0)//showable data
	{
		size--;
		if(lcdBuffer[index]>=' ')
		{
			writeData(lcdBuffer[index]);
		}
		else
		{
			if(lcdBuffer[index]==0x01)//line0
			{
				gotoXY(0,0);
			}
			if(lcdBuffer[index]==0x02)//line1
			{
				gotoXY(0,1);
			}
		}
		index++;
	}
}

void LCDServiceClass::init2()
{
	init();
	busy=false;
}

#endif //__LCD_CONTROLLER_CLASS_H__
