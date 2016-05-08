// ----------------------------------------------------------------------------
// File: iotpipe_json.c
// Copyright (c) 2015 IoT Pipe. All Right Reserved
// This file contains the definitions of methods used to handle json strings
// ----------------------------------------------------------------------------


///////////////////////////////////////////////////////////////////////////////
//     This JSON library can only be used for 1 dimension json value. 

#include "iotpipe_utils.h"
#include "iotpipe_json.h"
#include "iotpipe_gpio.h"
#include "iotpipe_sntp.h"
#include "osapi.h"
#include "mem.h"
#include "string.h"

bool ICACHE_FLASH_ATTR init_json()
{
	json_head = (json_node_t*)os_malloc(sizeof(json_node_t));
	if(json_head==NULL)
		return false;
	else
	{
		json_head->next=NULL;	
	}
	return true;
}


bool ICACHE_FLASH_ATTR createJsonForScan(char *buf, int bufLength)
{

	if(bufLength<=2)
	{
		LOG_DEBUG("Buffer must have at least a length of 2.");
		return false;
	}

	gpio_node_t *gpio_node = gpio_head->next;
	bool success;
	bool atleastOneInput = false;
	while(gpio_node!=NULL)
	{
		if(gpio_node->gpio_type==0)
		{
			success = add_json_node(gpio_node->portName, gpio_node->value);
			if(success==false)
			{
				LOG_DEBUG("Failed to construct json node");
				return false;
			}
			atleastOneInput = true;
		}
		gpio_node=gpio_node->next;
	}

	if(atleastOneInput == false)
	{
		LOG_DEBUG("Was not at least one input");
		strcat(buf,"{}");
		return true;

	}
	
	//Add node for timestamp
	char timeBuf[16];
	success = getEpochTimeInMs(timeBuf,16); 
	if(success == false)
	{
		LOG_DEBUG("Failed to get SNTP time to put in JSON payload.");
		return false;
	}
	add_json_node("timestamp", 1234);

	success = stringify(buf, bufLength);
	free_json();
	return success;
}





//Prints out the key value pairs
static void ICACHE_FLASH_ATTR print_json()
{
	json_node_t *node = json_head->next;

	if(node!=NULL)
		LOG_DEBUG("-------------------------------------------");

	while(node!=NULL)
	{
		LOG_DEBUG_ARGS("(%s,%s)",node->key,node->value);
		node=node->next;
	}
}

static bool ICACHE_FLASH_ATTR add_json_node(char *key, int value)
{

	//convert integer to string
	char buf[16];
	flatten_string(buf,16);
	itoa(value,buf);


	//find end of list
	json_node_t *node = json_head;
	while(node->next!=NULL)
		node=node->next;	

	//create new node, return false if memory allocation fails
	json_node_t *new_node = (json_node_t *)os_zalloc(sizeof(json_node_t));
	if(new_node==NULL)
		return false;

	if(strlen(key)>max_key_length)
	{
		LOG_DEBUG_ARGS("Key length too long: %s", key);
		return false;
	}
	
	//add info to the new node
	strcpy(new_node->key,key);
	strcpy(new_node->value,buf);
	new_node->next = NULL; 
	node->next = new_node;
	return true;
}

static bool ICACHE_FLASH_ATTR stringify(char * buffer, int bufLen)
{
	if(json_head==NULL)
	{
		LOG_DEBUG("Canot turn json into string.  linked list is empty");
		return false;
	}
	
	if(json_head->next==NULL)
	{
		strcat(buffer,"{}");
		return true;
	}


	//Compute length of the about-to-be-made json string
	json_node_t *node = json_head;
	int length = 0;
	int numPairs = 0;
	while(node!=NULL)
	{
		length+=strlen(node->key);
		length+=strlen(node->value);
		numPairs++;
		node=node->next;
	}
	//now add length for commas, brackets, etc.
	length+=2; //add opening and closing brackets
	length+=numPairs; //add colons, 1 for each k,v pair;
	length+=numPairs; // add commas for each k,v pair
	
	if(length > bufLen)
	{
		LOG_DEBUG("Canot turn json into string.  buffer isn't long enough");
		return false;
	}
	//Start us off
	int i;
	for(i = 0; i < length; i++)
		buffer[i]='\0';

	int total_length = 0;
	
	buffer[total_length++]='{';
	node = json_head->next;
	while(node!=NULL)
	{
		buffer[total_length++]='\"';
		strcat(&buffer[total_length],node->key);
		total_length+=strlen(node->key);
		buffer[total_length++]='\"';
		buffer[total_length++]=':';
		buffer[total_length++]='\"';
		strcat(&buffer[total_length],node->value);
		total_length+=strlen(node->value);
		buffer[total_length++]='\"';
		buffer[total_length++]=',';
		node=node->next;
	}		
	buffer[--total_length]='}';


	return true;
}

//Frees the json linked list, except for the head, which is re-used
static void ICACHE_FLASH_ATTR free_json()
{
	json_node_t *conductor = json_head->next;
	json_node_t *temp;
	while(conductor!=NULL)
	{
		temp = conductor;
		conductor = conductor->next;
		os_free(temp);
	}
}

