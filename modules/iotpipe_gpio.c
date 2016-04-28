#include "iotpipe_gpio.h"
#include "iotpipe_utils.h"
#include "string.h"
#include "mem.h"
#include "jsmn.h"

//we pick a different name to not be confused with gpio_init() in gpio.h
bool ICACHE_FLASH_ATTR init_gpio()
{
	gpio_head = (gpio_node_t*)os_malloc(sizeof(gpio_node_t));
	if(gpio_head==NULL)
		return false;
	else
	{
		gpio_head->next=NULL;	
		gpio_head->portNumber=-1;
		gpio_head->portName=(char *)os_zalloc(sizeof(char));
		gpio_head->value=-1;
	}
	return true;

}

//Checks if port # is a valid GPIO for the ESP8266
static bool ICACHE_FLASH_ATTR isValidGPIO(int portNum)
{
	int index = getIndex(portNum);
	if(index==-1)
	{
		return false;
	}
	return true;
}

//GPIOs of the same type cannot have the same name
static bool ICACHE_FLASH_ATTR isPortNameValid(char *portName, int type)
{
	gpio_node_t *node = gpio_head;
	while(node->next!=NULL)
	{
		if ( strcmp(node->next->portName, portName) == 0 & node->next->gpio_type == type )
		{
			return false;
		}	
		node=node->next;
	}
}


bool ICACHE_FLASH_ATTR setPortAsInputWithName(int portNum, char *portName)
{
	
	if( !isValidGPIO(portNum) )
	{
		LOG_DEBUG_ARGS("GPIO%d is not valid.",portNum);
		return false;
	}
	
	if ( !isPortNameValid(portName,INPUT) )	
	{
		LOG_DEBUG_ARGS("Failed to set GPIO%d as input.  Portname of (%s) is already assigned to a port of type %d", portNum, portName, INPUT);
		return false;
	}	
	
	
	bool success = addNode(portNum, portName,INPUT);
	if(success==false)
		return false;

	int index = getIndex(portNum);
	PIN_FUNC_SELECT(pin_mux[index],pin_func[index]); 
	PIN_PULLUP_DIS(pin_mux[index]);
	gpio_output_set(0,0,0,pin_num[index]);
	
	return true;
}


bool ICACHE_FLASH_ATTR setPortAsOutputWithName(int portNum, char *portName)
{
	if( !isValidGPIO(portNum) )
	{
		LOG_DEBUG_ARGS("GPIO%d is not valid.",portNum);
		return false;
	}

	if ( !isPortNameValid(portName, OUTPUT) )	
	{
		LOG_DEBUG_ARGS("%s is already assigned to a port of type %d", portName, OUTPUT);
		return false;
	}

	bool success = addNode(portNum, portName, OUTPUT);
	if(success==false)
		return false;

	int index = getIndex(portNum);
	PIN_FUNC_SELECT(pin_mux[index],pin_func[index]); 
	gpio_output_set(0, 0, 1 << portNum, 0); // enable pin as output - See more at: http://www.esp8266.com/viewtopic.php?f=6&t=1033#sthash.kNUniUri.dpuf	

	set_gpio_output_high(portNum);


	return true;


}

bool ICACHE_FLASH_ATTR setPortAsInterruptableWithName(int portNum, char *portName)
{
	LOG_DEBUG("Interrupt not yet supported.");
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
		flatten_string(buf1,16);
		flatten_string(buf2,16);

		itoa(portNumber,buf1);		

		strcat(buf2,"GPIO");
		strcat(buf2,buf1);
		portName = (char *)os_zalloc(sizeof(char)*16);
		strcpy(portName,buf2);
		
	}


	gpio_node_t *conductor;
	conductor = gpio_head;
	bool portAlreadyExists = false;

	while(conductor->next!=NULL)
	{
		if(conductor->next->portNumber==portNumber)
		{
			portAlreadyExists = true;
			conductor=conductor->next;
			break;
		}
		conductor=conductor->next;
	}

	if(portAlreadyExists)
	{
		strcpy(conductor->portName,portName);
		conductor->gpio_type=type;		
	}
	else
	{
		gpio_node_t *newNode = (gpio_node_t*)os_malloc(sizeof(gpio_node_t));
		newNode->portName = (char *)os_zalloc(sizeof(char)*strlen(portName)+1);
		if(newNode->portName==NULL)
		{
			LOG_DEBUG("Failed to allocate memory for gpio node.");
			return false;
		}
		flatten_string(newNode->portName,strlen(portName));	
		newNode->gpio_type = type;
		strcpy(newNode->portName,portName);
		newNode->portNumber = portNumber;
		newNode->value=-1;
		newNode->next=NULL;
		conductor->next = newNode;
	}

	return true;
}


bool gpio_input_scan()
{

	gpio_node_t *node = gpio_head->next;
	int index;
	while(node!=NULL)
	{
		index = getIndex(node->portNumber);
		if(index==-1)
		{
			LOG_DEBUG_ARGS("GPIO %d is not a valid GPIO", node->portNumber);
			return false;
		}
		
		if(node->gpio_type==0)
		{
			node->value = GPIO_INPUT_GET(pin_num[index]);
		}
		node=node->next;
	}
	return true;
}


//Scans MQTT payload and checks if any OUTPUT port is mentioned
bool gpio_update_outputs(char *jsonString)
{

	jsmn_parser parser;
	jsmntok_t tokens[max_jsmn_tokens];

	jsmn_init(&parser);

	//r is the # of tokens returned.
	int r = jsmn_parse(&parser, jsonString, strlen(jsonString), tokens, max_jsmn_tokens);

	//if r < 1 or the first token isn't a JSMN_OBJECT
	if (r < 1 || tokens[0].type!=JSMN_OBJECT)
	{
		LOG_DEBUG("Nothing to be updated.");
		return true;
	}

	int i = 0;
	gpio_node_t *node = gpio_head->next;
	while(node!=NULL)
	{
		if(node->gpio_type==OUTPUT)
		{
			//start at i = 1, since i = 0 is the JSMN_OBJECT token (aka the root)
			for(i = 1; i < r; i++)
			{
				if ( jsoneq(jsonString, &tokens[i],node->portName) == 0)
				{	
					/* We may use strndup() to fetch string value */
					int bufLen = tokens[i+1].end - tokens[i+1].start + 1;
					char *buf = (char *)os_zalloc( sizeof(char)*bufLen );
					if(buf==NULL)
					{
						LOG_DEBUG("Couldn't allocate memory.");
						return false;
					}
			
					int k,z=0;
					for(k=tokens[i+1].start; k < tokens[i+1].end; k++)
					{
						buf[z]=jsonString[k];
						z++;
					}
					buf[z]='\0';
					LOG_DEBUG("Changing output:");
					LOG_DEBUG_ARGS("\t%s: %s-->%s",node->portName, buf, node->value ? "true" : "false");
					i++;
					break;
				}
			}
		}
		node=node->next;
	}

	return true;
}


void print_gpio_nodes()
{
	gpio_node_t *node = gpio_head->next;
	while(node!=NULL)
	{

		if(node->gpio_type==INPUT)
			LOG_DEBUG_ARGS("INPUT: (%d,%s)",node->portNumber,node->portName);
		else if(node->gpio_type==INTERRUPT)
			LOG_DEBUG_ARGS("INTERRUPT: (%d,%s)",node->portNumber,node->portName);
		else if(node->gpio_type==OUTPUT)
			LOG_DEBUG_ARGS("OUTPUT: (%d,%s)",node->portNumber,node->portName);
		node=node->next;
	}
}


static void set_gpio_output_high(int portNum)
{
	int index = getIndex(portNum);
	gpio_output_set(pin_bit[index], 0, pin_bit[index], 0);
}

static void set_gpio_output_low(int portNum)
{
	int index = getIndex(portNum);
	gpio_output_set(0,pin_bit[index], pin_bit[index], 0);
}

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}
