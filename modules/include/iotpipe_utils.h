// ----------------------------------------------------------------------------
// File: iotpipe_utils.h
// Copyright (c) 2015 IoT Pipe. All Right Reserved
// This file contains utilities macros
// ----------------------------------------------------------------------------

#ifndef IOTPIPE_UTILS_H
#define IOTPIPE_UTILS_H

#include "osapi.h"
#include "user_interface.h"



// TODO: Add conditions to remove logging in non-debug builds
#define LOG_DEBUG(message)		do { os_printf("[JIT-DEBUG] %s", message); os_printf("\r\n"); } while (0)
#define LOG_DEBUG_ARGS(message, args...)		do { os_printf("[JIT-DEBUG] "); os_printf(message, args); os_printf("\r\n"); } while (0)

#define LOG_INFO(message)		do { os_printf("[JIT-INFO] %s", message); os_printf("\r\n"); } while (0)
#define LOG_INFO_ARGS(message, args...)		do { os_printf("[JIT-INFO] "); os_printf(message, args); os_printf("\r\n"); } while (0)

#define LOG_WARNING(message)		do { os_printf("[JIT-WARNING] %s", message); os_printf("\r\n"); } while (0)
#define LOG_WARNING_ARGS(message, args...)	do { os_printf("[JIT-WARNING] "); os_printf(message, args); os_printf("\r\n"); } while (0)

#define LOG_ERROR(message)		do { os_printf("[JIT-ERROR] %s", message); os_printf("\r\n"); } while (0)
#define LOG_ERROR_ARGS(message, args...)		do { os_printf("[JIT-ERROR] "); os_printf(message, args); os_printf("\r\n"); } while (0)


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
