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


//Makes it easy to add nodes to a json object
bool add_json_node(json_node_t *head, char *key, char *value);

//Converts json object into string.  <-- not implemented
bool stringify(json_node_t * head, char * buffer, int len);

//frees memory of jsob object
void free_json(json_node_t *head);

//prints json
void print_json(json_node_t *head);
