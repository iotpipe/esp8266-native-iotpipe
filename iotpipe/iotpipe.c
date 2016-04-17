#include "iotpipe.h"
#include "iotpipe_gpio.h"
#include "iotpipe_utils.h"

void iotpipe_init()
{
	LOG_DEBUG("Init!!!");
}

bool iotpipe_addInputPort(int portNum, char* portName)
{
	bool success = setPortAsInputWithName(portNum, portName);
	return success;
}

bool iotpipe_addInterruptablePort(int portNum, char *portName)
{

}

bool iotpipe_addOutputPort(int portNum, char *portName)
{

}

