/*
We create a layer on top of the OS pin definitions to more easily keep track of things.  This makes it easier to set pins as inputs and outputs.
*/



#ifndef GPIO_DEFINITIONS_H_
#define GPIO_DEFINITIONS_H_



#include "os_type.h"
#include "gpio.h"

#define NUM_VALID_INPUT_PINS 8

extern const uint32_t pin_mux[NUM_VALID_INPUT_PINS];
extern const uint8_t pin_num[NUM_VALID_INPUT_PINS];
extern const uint8_t pin_func[NUM_VALID_INPUT_PINS];
extern const int pin_bit[NUM_VALID_INPUT_PINS];

#endif
