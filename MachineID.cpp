#include "MachineID.h";

MachineID::MachineID()
{
	_ioexpander = new IOExpander(0x00);

	_ioexpander->setIOCon();
    _ioexpander->setIODir();
    _ioexpander->setInputPolarity(0xff);
    _ioexpander->setGPPU(0xff);
}

MachineID::~MachineID()
{
	_ioexpander = nullptr;
}

uint8_t MachineID::getId()
{
	return _ioexpander->getGPIO();
}