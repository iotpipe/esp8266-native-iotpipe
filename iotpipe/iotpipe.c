#include "iotpipe.h"
#include "iotpipe_gpio.h"
#include "iotpipe_utils.h"
#include "iotpipe_json.h"
#include "os_type.h"


bool iotpipe_init()
{
	bool s = init_gpio();
	if(s==false)
		return false;

	s = init_json();
	if(s==false)
		return false;

	return true;
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
	bool success = setPortAsOutputWithName(portNum, portName);
	return success;
}



bool iotpipe_scan(char *buf, int bufLength)
{
	createJsonForScan(buf,bufLength);
}

