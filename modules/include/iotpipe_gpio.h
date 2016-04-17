#ifndef IOTPIPE_GPIO_H_
#define IOTPIPE_GPIO_H_

#include "os_type.h"
#include "gpio_definitions.h"

//Node for the linked list that stores all of the GPIOs set as inputs
typedef struct input_node 
{
	int portNumber;
	char *portName;
	int gpio_type;
	struct input_node * next;
} iotpipe_node_t;

iotpipe_node_t *node_head;

typedef enum {INPUT, INTERRUPT, OUTPUT} input_mode;

bool ICACHE_FLASH_ATTR setPortAsInputWithName(int portNum, char *portName);
bool ICACHE_FLASH_ATTR setPortAsOutputWithName(int portNum, char *portName);
bool ICACHE_FLASH_ATTR setPortAsInterruptableWithName(int portNum, char *portName);

static int ICACHE_FLASH_ATTR getIndex(int pin);

static bool ICACHE_FLASH_ATTR addNode(int pin, char *portName, int type);

#endif
