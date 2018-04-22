#include "ioexpander.h"

#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>

IOExpander::IOExpander(int sda, int scl, uint8_t address)
{
	Wire.begin(sda, scl);

	_address = baseAddress + address;
	_givenAddress = address;
}

IOExpander::IOExpander(uint8_t address):IOExpander(4, 5, address){}

uint8_t IOExpander::getRegister(uint8_t reg)
{

	Wire.beginTransmission(_address);

	Wire.write(reg);

	uint8_t err = Wire.endTransmission();

	delay(10);

	Wire.requestFrom((int)_address, 1);

	uint8_t c = 0;

	while(Wire.available()) {
		c = Wire.read();
	}

	delay(10);
	return c;
}

void IOExpander::setRegister(uint8_t reg, uint8_t value)
{
	Wire.beginTransmission(_address);

	Wire.write(reg);
	delay(10);
	Wire.write(value);
	delay(10);
	Wire.endTransmission();
	
}

void IOExpander::setIODir()
{
	uint8_t direction = 0xFF;

	setIODir(direction);
}

void IOExpander::setIODir(uint8_t direction)
{
	setRegister(0x00, direction);
}

uint8_t IOExpander::getIODir()
{
	return getRegister(0x00);
}

void IOExpander::setInputPolarity()
{
	uint8_t polarity = 0x00;

	setInputPolarity(polarity);
}

void IOExpander::setInputPolarity(uint8_t polarity)
{
	setRegister(0x01, polarity);
}

uint8_t IOExpander::getInputPolarity()
{
	return getRegister(0x01);
}

void IOExpander::setIOCon()
{
	uint8_t con = 0x00;

	setIOCon(con);
}

void IOExpander::setIOCon(uint8_t con)
{
	setRegister(0x02, con);
}

uint8_t IOExpander::getIOCon()
{
	return getRegister(0x02);
}

void IOExpander::setGPPU()
{
	uint8_t gppu = 0xFF;
}

void IOExpander::setGPPU(uint8_t gppu)
{
	setRegister(0x03, gppu);
}

uint8_t IOExpander::getGPPU()
{
	return getRegister(0x03);
}

void IOExpander::setGPIO()
{
	uint8_t gpio = 0x00;

	setGPIO(gpio);
}

void IOExpander::setGPIO(uint8_t gpio)
{
	setRegister(0x09, gpio);
}

uint8_t IOExpander::getGPIO()
{
	return getRegister(0x09);
}