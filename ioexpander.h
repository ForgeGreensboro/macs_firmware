#ifndef __IOEXPANDER_H__
#define __IOEXPANDER_H__

#include <Arduino.h>

class IOExpander {
public:

	IOExpander(int, int, uint8_t);
	IOExpander(uint8_t);

	~IOExpander();

	void setIODir();
	void setIODir(uint8_t);
	uint8_t getIODir();

	void setInputPolarity();
	void setInputPolarity(uint8_t);
	uint8_t getInputPolarity();

	void setIOCon();
	void setIOCon(uint8_t);
	uint8_t getIOCon();

	void setGPPU();
	void setGPPU(uint8_t);
	uint8_t getGPPU();

	void setGPIO();
	void setGPIO(uint8_t);
	uint8_t getGPIO();

private:
	const uint8_t baseAddress =  0x20;
	int _pinData;
	int _pinClock;

	uint8_t _address;
	uint8_t _givenAddress;

	void setRegister(uint8_t register, uint8_t value);
	uint8_t getRegister(uint8_t register);

};


#endif