#include "LEDDriver.h"
#include "ioexpander.h"

#include <Arduino.h>
#include <Ticker.h>

#define WIFI_STATUS_MASK 0x01
#define RELAY_STATUS_MASK 0x02

LEDDriver::LEDDriver()
{
	_ioexpander = new IOExpander(0x01);

	_ioexpander->setIOCon();
    _ioexpander->setGPPU(0x00);
	_ioexpander->setIODir(0x00);
	_ioexpander->setInputPolarity(0x00);
}

LEDDriver::~LEDDriver()
{
	_ioexpander = nullptr;
}

void LEDDriver::setLEDs()
{
	_ioexpander->setGPIO(currentValue);
}

void LEDDriver::setWifiStatus(int status)
{

	if(status)
	{
		currentValue += 0x1;
	} else {
		if(currentValue & 0x1)
		{
			currentValue -= 0x1;
		}
	}

	setLEDs();
}
void LEDDriver::setRelayStatus(int status)
{

	bool isSet = (currentValue & 0x2) == 2;

	if(status)
	{
		if(!isSet)
		{
			currentValue += 0x2;
			Serial.println(currentValue);
		}
	} else {
		if(isSet) {
			currentValue -= 0x2;
			Serial.println(currentValue);
		}
	}


	setLEDs();
}

void LEDDriver::setSystemReadyStatus(int status)
{
	bool isSet = (currentValue & 0x4) == 4;

	if(status)
	{
		if(!isSet)
		{
			currentValue += 0x4;
			Serial.println(currentValue);
		}
	} else {
		if(isSet) {
			currentValue -= 0x4;
			Serial.println(currentValue);
		}
	}


	setLEDs();

}
