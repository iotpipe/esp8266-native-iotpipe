#include "iotpipe.h"
#include "iotpipe_gpio.h"
#include "iotpipe_utils.h"
#include "iotpipe_json.h"
#include "os_type.h"


//Initializes IoT Pipe client service
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

//Adds a GPIO to a list of input ports
bool iotpipe_addInputPort(int portNum, char* portName)
{
	bool success = setPortAsInputWithName(portNum, portName);
	return success;
}

//Adds a GPIO to a list of interruptable ports
bool iotpipe_addInterruptablePort(int portNum, char *portName)
{

}

//Adds a GPIO to a list of output ports
bool iotpipe_addOutputPort(int portNum, char *portName)
{
	bool success = setPortAsOutputWithName(portNum, portName);
	return success;
}

//Scans all input ports and creates a JSON payload of thier values which can be read by IoT Pipe web service
bool iotpipe_scan(char *buf, int bufLength)
{
	if( gpio_head==NULL)
	{
		LOG_DEBUG("GPIO HEAD EMPTY");
		strcat(buf,"{}");
		return true;
	}	

	gpio_node_t *gpio_node = gpio_head->next;
	
	bool scan_success = gpio_input_scan();
	if(scan_success==false)
	{
		LOG_DEBUG("Failed to scan GPIOs.");
		return false;
	}

	createJsonForScan(buf,bufLength);
}

//Reads a JSON payload from IoT Pipe web service that contains the desired values of output ports.
bool iotpipe_update_outputs(char *jsonString)
{
	return gpio_update_outputs(jsonString);
}
