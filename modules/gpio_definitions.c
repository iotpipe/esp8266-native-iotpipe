#include "gpio_definitions.h"

const uint32_t pin_mux[NUM_VALID_INPUT_PINS] = { PERIPHS_IO_MUX_GPIO0_U, 
				   	   PERIPHS_IO_MUX_GPIO2_U,  
					   PERIPHS_IO_MUX_GPIO4_U, 
					   PERIPHS_IO_MUX_GPIO5_U, 
					   PERIPHS_IO_MUX_MTDI_U,     
					   PERIPHS_IO_MUX_MTCK_U, 
					   PERIPHS_IO_MUX_MTMS_U, 
					   PERIPHS_IO_MUX_MTDO_U}; 

const uint8_t pin_num[NUM_VALID_INPUT_PINS] = {0,2,4,5,12,13,14,15};

const uint8_t pin_func[NUM_VALID_INPUT_PINS] = { FUNC_GPIO0, 
					   FUNC_GPIO2, 
					   FUNC_GPIO4,
					   FUNC_GPIO5, 
					   FUNC_GPIO12,
					   FUNC_GPIO13,
					   FUNC_GPIO14,
					   FUNC_GPIO15};




