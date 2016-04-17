// ----------------------------------------------------------------------------
// File: jit_json_utils.c
// Copyright (c) 2015 JIT. All Right Reserved
// This file contains the definitions of methods used to handle json strings
// ----------------------------------------------------------------------------


///////////////////////////////////////////////////////////////////////////////
//     This JSON library can only be used for 1 dimension json value. 

#include "iotpipe_utils.h"
#include "iotpipe_json.h"
#include "osapi.h"
#include "mem.h"

//Prints out the key value pairs
void print_json(json_node_t *head)
{
	json_node_t *node = head->next;
	while(node!=NULL)
	{
		LOG_DEBUG_ARGS("(%s,%s)",node->key,node->value);
		node=node->next;
	}
}

bool add_json_node(json_node_t *head, char *key, char *value)
{
	//find end of list
	json_node_t *node = head;
	while(node->next!=NULL)
		node=node->next;	

	//create new node, return false if memory allocation fails
	json_node_t *new_node = (json_node_t *)os_zalloc(sizeof(json_node_t));
	if(new_node==NULL)
		return false;

	if(strlen(key)>max_key_length)
	{
		LOG_ERROR_ARGS("Key length too long: %s", key);
		return false;
	}
	if(strlen(value)>max_value_length)
	{
		LOG_ERROR_ARGS("Value length too long: %s", value);
		return false;
	}
	//add info to the new node
	strcpy(new_node->key,key);
	strcpy(new_node->value,value);
	new_node->next = NULL; 
	node->next = new_node;
	return true;
}

bool stringify(json_node_t * head, char *json_string, int max_len)
{
	if(head==NULL)
	{
		LOG_ERROR("Canot turn json into string.  linked list is empty");
		strcat(json_string,"{\"error\":\"Couldn't create json string because json structure was empty.\"}");
		return false;
	}
	
	if(head->next==NULL)
	{
		strcat(json_string,"{}");
		return true;
	}


	//Compute length of the about-to-be-made json string
	json_node_t *node = head;
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
	
	if(length > max_len)
	{
		LOG_ERROR("Canot turn json into string.  buffer isn't long enough");
		strcat(json_string,"{\"error\":\"Couldn't create json string because it was longer than available buffer\"}");
		return false;
	}
	//Start us off
	int i;
	for(i = 0; i < length; i++)
		json_string[i]='\0';

	int total_length = 0;
	
	json_string[total_length++]='{';
	node = head->next;
	while(node!=NULL)
	{
		json_string[total_length++]='\"';
		strcat(&json_string[total_length],node->key);
		total_length+=strlen(node->key);
		json_string[total_length++]='\"';
		json_string[total_length++]=':';
		json_string[total_length++]='\"';
		strcat(&json_string[total_length],node->value);
		total_length+=strlen(node->value);
		json_string[total_length++]='\"';
		json_string[total_length++]=',';
		node=node->next;
	}		
	json_string[--total_length]='}';
	return true;
}

void free_json(json_node_t *head)
{
	json_node_t *conductor = head;
	json_node_t *temp;
	while(conductor!=NULL)
	{
		temp = conductor;
		conductor = conductor->next;
		os_free(temp);
	}
	head=NULL;
	
}


