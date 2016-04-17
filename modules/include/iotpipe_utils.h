// ----------------------------------------------------------------------------
// File: iotpipe_utils.h
// Copyright (c) 2015 IoT Pipe. All Right Reserved
// This file contains utilities macros
// ----------------------------------------------------------------------------

#ifndef IOTPIPE_UTILS_H
#define IOTPIPE_UTILS_H

#include "osapi.h"
#include "user_interface.h"

//enumeration of available debug levels
//Low - Errors and MQTT debug stuff only
//High - Everything
typedef enum {DEBUG_OFF=0, DEBUG_LOW, DEBUG_HIGH} debug_level;
//Global value of debug verbosity
uint8_t debug_verbosity;

#define LOG_DEBUG(message)		do { if(debug_verbosity==DEBUG_HIGH) {os_printf("[JIT-DEBUG] %d", message); os_printf("\r\n");}  } while (0)
#define LOG_DEBUG_ARGS(message, args...)		do { if(debug_verbosity==DEBUG_HIGH) {os_printf("[JIT-DEBUG] "); os_printf(message, args); os_printf("\r\n");} } while (0)

#define LOG_ERROR(message)		do { if(debug_verbosity>=DEBUG_LOW) {os_printf("[JIT-ERROR] %s", message); os_printf("\r\n");} } while (0)
#define LOG_ERROR_ARGS(message, args...)		do { if(debug_verbosity>=DEBUG_LOW) {os_printf("[JIT-ERROR] "); os_printf(message, args); os_printf("\r\n");} } while (0)


#define IF_NULL_RETURN_FALSE(value, message)	\
	if (value == NULL) {				\
		LOG_ERROR(message);				\
		return FALSE;					\
	}

#define IF_NULL_RETURN(value, message)	\
	if (value == NULL) {				\
		LOG_ERROR(message);				\
		return;					\
	}


//implement itoa
void itoa(int n, char s[]);

//implement atoi
//int atoi(char s[]);


//reverse is a helper function used inside of itoa.  It is static so can only be used in this compilation unit.
static void reverse(char s[]);

//flatten strings with null characters
void flatten_string(char *str,int len);

#endif
