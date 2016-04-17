#include "iotpipe_gpio.h"
#include "iotpipe_utils.h"
#include "string.h"
#include "mem.h"

bool ICACHE_FLASH_ATTR setPortAsInputWithName(int portNum, char *portName)
{
	bool success = addNode(portNum, portName, 0);

	int index = getIndex(portNum);
	if(index==-1)
	{
		LOG_DEBUG_ARGS("GPIO %d is not a valid GPIO", portNum);
		return false;
	}

	PIN_FUNC_SELECT(pin_mux[index],pin_func[index]); 
	PIN_PULLUP_DIS(pin_mux[index]);
	gpio_output_set(0,0,0,pin_num[index]);
	return true;

}


bool ICACHE_FLASH_ATTR setPortAsOutputWithName(int portNum, char *portName)
{

}

bool ICACHE_FLASH_ATTR setPortAsInterruptableWithName(int portNum, char *portName)
{

}

static int ICACHE_FLASH_ATTR getIndex(int portNum)
{
	int i = 0;
	for(i = 0; i < NUM_VALID_INPUT_PINS; i++)
	{
		if(portNum==pin_num[i])
		{
			return i;
		}
	}
	return -1;
}

static bool ICACHE_FLASH_ATTR addNode(int portNumber, char *portName, int type)
{

	//If user doesn't specify port name, then create one of the form  "GPION" where N is the port number
	if(strcmp(portName,"")==0)
	{
		char buf1[16];
		char buf2[16];

		itoa(portNumber,buf1);		

		strcat(buf2,"GPIO");
		strcat(buf2,buf1);
	
		strcpy(portName,buf2);
		
	}


	iotpipe_node_t *node;
	node = node_head;
	bool portAlreadyExists = false;
	while(node!=NULL)
	{
		//We either find the port already exists in our list, and we update it, or we go to end of list and create a new node
		if(node->portNumber == portNumber)
		{
			portAlreadyExists = true;
			break;
		}
		node=node->next;	
	}

	if(portAlreadyExists == false)
	{
		node = (iotpipe_node_t *)os_malloc( sizeof(iotpipe_node_t) );
		if(node==NULL)
		{
			LOG_ERROR("Ran out of memory allocating GPIOs");
			return false;
		}
		node->next = NULL;
	}

	node->portNumber = portNumber;	
	if(portName==NULL)
		node->portName=NULL;
	else
		strcpy(node->portName, portName);

	node->gpio_type = 0;


	return true;
}
