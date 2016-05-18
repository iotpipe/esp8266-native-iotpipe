#include "iotpipe.h"
#include "ets_sys.h"
#include "driver/uart.h"
#include "osapi.h"
#include "user_interface.h"
#include "mem.h"
#include "iotpipe_utils.h"

#define baud_rate 115200

void user_init(void)
{
	uart_div_modify(0, UART_CLK_FREQ / baud_rate); 
	os_delay_us(10);

	os_delay_us(1000000);

	LOG_DEBUG("Starting");
	
	iotpipe_init();
	
	iotpipe_addDigitalInputPort(5,"five");
	iotpipe_addDigitalInputPort(4,"four");
	iotpipe_addDigitalInputPort(15,"fifteen");
	
	print_gpio_nodes();

	char buf[256];
	flatten_string(buf,256);

	iotpipe_scan(buf,256);	
	LOG_DEBUG_ARGS("Generated JSON: %s", buf);
}




