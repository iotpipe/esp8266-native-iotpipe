#ifndef IOTPIPE_H_
#define IOTPIPE_H_

#include "os_type.h"


//Initializes IoT Pipe client service
//Returns True/False to designate a success or failure
bool iotpipe_init();

//Adds a GPIO to a list of input ports
//Arguments:  GPIO # and desired name of the port
//Return: True/False to designate a success or failure
bool iotpipe_addInputPort(int portNum, char *portName);

//Adds a GPIO to a list of interruptable ports
//Arguments:  GPIO # and desired name of the port
//Return: True/False to designate a success or failure
bool iotpipe_addInterruptablePort(int portNum, char *portName);

//Adds a GPIO to a list of output ports
//Arguments:  GPIO # and desired name of the port
//Return: True/False to designate a success or failure
bool iotpipe_addOutputPort(int portNum, char* portName);

//Scans all input ports and creates a JSON payload of thier values which can be read by IoT Pipe web service
//Arguments:  The buffer to which the JSON payload will be written as well as the maximum length of the buffer
//Return: True/False to designate a success or failure
bool iotpipe_scan(char *buf, int bufLen);

//Reads a JSON payload from IoT Pipe web service that contains the desired values of output ports.
//Arguments: The JSON payload sent from IoT Pipe web service.  It should be null terminated.
//Return: True/False to designate a success or failure
bool iotpipe_update_outputs(char *json);

#endif
