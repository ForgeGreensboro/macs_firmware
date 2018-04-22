#ifndef __MACHINE_ID_H__
#define __MACHINE_ID_H__

#include "ioexpander.h"

class MachineID {
private:
	IOExpander *_ioexpander = nullptr;

public:
	MachineID();
	~MachineID();

	uint8_t getId();
};

#endif