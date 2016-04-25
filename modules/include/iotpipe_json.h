// ----------------------------------------------------------------------------
// File: jit_json_utils.h
// Copyright (c) 2015 JIT. All Right Reserved
// This file contains the definitions of methodsused to parse http requests
// ----------------------------------------------------------------------------


#include "user_interface.h"


#define max_key_length 32
#define max_value_length 32

typedef struct json_node {

	char key[max_key_length];
	char value[max_value_length];
	struct json_node *next;
} json_node_t;

json_node_t *json_head;

//Initializes the head of the json node
bool ICACHE_FLASH_ATTR init_json();

//Makes it easy to add nodes to a json object
static bool ICACHE_FLASH_ATTR add_json_node(char *key, int value);

//Converts json object into string.  
static bool ICACHE_FLASH_ATTR stringify(char * buffer, int len);

//frees memory of jsob object
static void ICACHE_FLASH_ATTR free_json();

//prints json
static void ICACHE_FLASH_ATTR print_json();

//Public facing function
bool ICACHE_FLASH_ATTR createJsonForScan(char *buf, int bufLength);

