#ifndef IOTPIPE_GPIO_H_
#define IOTPIPE_GPIO_H_

#include "os_type.h"
#include "gpio_definitions.h"
#include "jsmn.h"

#define PORTNAME_LENGTH 16
#define max_jsmn_tokens 128

//Node for the linked list that stores all of the GPIOs set as inputs
typedef struct input_node
{
	int portNumber;
	char *portName;
	int gpio_type;
	int value;
	struct input_node * next;
} gpio_node_t;
gpio_node_t *gpio_head;



typedef enum {INPUT, INTERRUPT, OUTPUT} gpio_mode;


//we pick a different name to not be confused with gpio_init() in gpio.h
bool ICACHE_FLASH_ATTR init_gpio();
bool ICACHE_FLASH_ATTR setPortAsInputWithName(int portNum, char *portName);
bool ICACHE_FLASH_ATTR setPortAsOutputWithName(int portNum, char *portName);
bool ICACHE_FLASH_ATTR setPortAsInterruptableWithName(int portNum, char *portName);

static int ICACHE_FLASH_ATTR getIndex(int pin);
static bool ICACHE_FLASH_ATTR addNode(int pin, char *portName, int type);
//GPIOs of the same type cannot have the same name
static bool ICACHE_FLASH_ATTR isPortNameValid(char *portName, int type);
//Checks if port # is a valid GPIO for the ESP8266
static bool ICACHE_FLASH_ATTR isValidGPIO(int portNum);

//updates value field in each node of gpio_head
bool gpio_input_scan();
bool gpio_update_outputs(char *jsonString);

//sets output pins high or low
static void set_gpio_output_high(int portNum);
static void set_gpio_output_low(int portNum);
static int jsoneq(const char *json, jsmntok_t *tok, const char *s);

#endif
