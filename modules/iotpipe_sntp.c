#include "iotpipe_sntp.h"
#include "iotpipe_utils.h"
#include "sntp.h"

void ICACHE_FLASH_ATTR setup_sntp()
{
	sntp_setservername(0,sntp_server_1);
	sntp_setservername(1,sntp_server_2);
	sntp_setservername(2,sntp_server_3);

	sntp_init();
}

bool ICACHE_FLASH_ATTR getEpochTimeInMs(char *buf, int bufLen)
{
	
	//The epoch time in Seconds is approximately 1.5 Billion, which is 10 digits.  It has been approximately 45 years since 1970.
	//In order to reach 11 digits, which would be 10 Billion, it would take an additional 200 years or so.  So we can safely assume
	//The time will be a 10 digit number in seconds.  Since we need the time in milliseconds we will add an additional 3 digits to bring
	//the total to 13 digits.  To be safe, we will require the buffer to have a length of 16, minimum.
	if( bufLen < 16)
	{
		LOG_DEBUG("Error.  Cannot retrieve timestamp with a buffer of less than 16.");
		return false;
	}

	uint32 current_stamp;
	current_stamp = sntp_get_current_timestamp();
	
	//ESP8266 doesn't support long integers.  Since we must convert to milliseconds we will overflow a 32 bit integer
	//Our solution is to convert the integer to a string, then append 3 zeros.
	flatten_string(buf,bufLen);
	itoa(current_stamp, buf);
	strcat(buf,"000");
	
	return true;
}

