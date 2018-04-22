#ifndef __LEDDRIVER_H__
#define __LEDDRIVER_H__

#include <Arduino.h>

class IOExpander;
class Ticker;

class LEDDriver {
private:
	IOExpander *_ioexpander = nullptr;
	uint8_t currentValue = 0;
	bool wifiApiStatus = false;

	void setLEDs();

public:
	LEDDriver();
	~LEDDriver();

	void setWifiStatus(int status);
	void setRelayStatus(int status);
	void setSystemReadyStatus(int status);

};

#endif