#ifndef IOTPIPE_H_
#define IOTPIPE_H_

#include "os_type.h"


//Initializes iotpipe service
void iotpipe_init();

//Sets scanning input ports
//Scanning ports CAN have a name, but it isn't a requirement.
bool iotpipe_addInputPort(int portNum, char *portName);

//Sets interruptable input ports
bool iotpipe_addInterruptablePort(int portNum, char *portName);

//Sets output ports
bool iotpipe_addOutputPort(int portNum, char* portName);

#endif
