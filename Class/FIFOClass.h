#ifndef __FIFO_CLASS_H__
#define __FIFO_CLASS_H__

#include <stdint.h>

class FIFOClass
{
	public:
		FIFOClass(){}
		FIFOClass(uint8_t depth);
		~FIFOClass();
		void init(uint8_t depth);
		bool isEmpty(void)const;
		bool isFull(void)const;
		uint8_t isDataAvailable(void)const;
		uint8_t getCount(void)const;
		uint8_t getDepth(void)const;
		void write(uint8_t data);
		uint8_t read(void);

		FIFOClass operator<<(const uint8_t data);
		FIFOClass operator>>(uint8_t &data);

	private:
		uint8_t *memory;
		uint8_t depth;
		uint8_t writeIndex;
		uint8_t readIndex;
		uint8_t count;
};

FIFOClass::FIFOClass(uint8_t depth)
:depth(depth)
{
	init(depth);
}

void FIFOClass::init(uint8_t depth)
{
	memory=new uint8_t[depth];
	this->depth=depth;
	writeIndex=0;
	readIndex=0;
	count=0;
}

FIFOClass::~FIFOClass()
{
	delete[] memory;
}

FIFOClass FIFOClass::operator>>(uint8_t &data)
{
	data=read();
	return *this;
}

FIFOClass FIFOClass::operator<<(const uint8_t data)
{
	write(data);
	return *this;
}

bool FIFOClass::isEmpty(void)const
{
	if(count==0)
		return true;
	return false;
}

bool FIFOClass::isFull(void)const
{
	if(count==depth)
		return true;
	return false;
}

uint8_t FIFOClass::isDataAvailable(void)const
{
	return (depth-count);
}

uint8_t FIFOClass::getCount(void)const
{
	return count;
}

uint8_t FIFOClass::getDepth(void)const
{
	return depth;
}

void FIFOClass::write(uint8_t data)
{
	if(!isFull())
	{
		memory[writeIndex]=data;
		count++;
		writeIndex++;
		if(writeIndex>(depth-1))
		{
			writeIndex=0;
		}
	}
}

uint8_t FIFOClass::read(void)
{
	uint8_t tmp=0;
	if(!isEmpty())
	{
		tmp=memory[readIndex];
		count--;
		readIndex++;
		if(readIndex>(depth-1))
		{
			readIndex=0;
		}
	}
	return tmp;
}


#endif // __FIFO_CLASS_H__
