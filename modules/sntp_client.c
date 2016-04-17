#include "sntp_client.h"
#include "mem.h"
#include "sntp_client.h"
#include "iotpipe_utils.h"

//Global variables that keep track of things;
static int clock_counter = 0;
int current_stamp = 0;
static bool sntp_connection_acquired = false;
ntp_connected_cb ntp_cb;
void ICACHE_FLASH_ATTR setup_ntp_client(ntp_connected_cb cb)
{
	os_timer_setfn(&sntp_timer, (os_timer_func_t *)sntp_timerfunc, NULL);
	ntp_cb = cb;
	ip_addr_t *addr = (ip_addr_t *)os_zalloc(sizeof(ip_addr_t));
	sntp_setservername(0, "us.pool.ntp.org"); // set server 0 by domain name
	sntp_set_timezone(0); //Set timezone as UTC
	sntp_init();
	os_free(addr);

	os_timer_arm(&sntp_timer,1000,1);
}


void ICACHE_FLASH_ATTR sntp_timerfunc(void *arg)
{
	//It's time to sync our time with the NTP server. We sync every 60 seconds.
	if(clock_counter%time_to_sync_in_seconds==0)
	{
		int temp = current_stamp;
		current_stamp = sntp_get_current_timestamp();
		//Ensure that a response was actually returned
		if(current_stamp==0)
		{
			//If we have never had a signal then there is nothing we can do but wait.
			if(sntp_connection_acquired == false)
			{
				LOG_ERROR("Failed to contact NTP server AND we have yet to make a first connection.");
				return;		
			}
			//If we've had a signal before, then just increment current_stamp by 1, and we'll try again next time.
			else
			{
				LOG_ERROR("Failed to contact NTP server BUT we have made a previous connection.");
				//LOG_DEBUG_ARGS("sntp: %d, %s",current_stamp, sntp_get_real_time(current_stamp));
				current_stamp=temp;
				current_stamp++;
			}
		}
		//If we received a valid response then current_stamp has been updated.  we show the accurate time, and the correction.  we also reset clock_counter.
		//Also if this is our first time receiving a response we alert the OS because we don't start sending requests until a response is received.
		else
		{
			if(sntp_connection_acquired == false)
			{
				//LOG_DEBUG("First NTP connection established.");
				sntp_connection_acquired = true;
				//ALERT OS THAT TIME HAS BEEN ESTABLISHED
				ntp_cb();
			}
			//We subtract 1 because the current_stamp is given to be 1 second later than the previous time_stamp so we remove that offset.
			clock_counter = 1;
			//LOG_DEBUG_ARGS("Correction: %d seconds", correction);
			//LOG_DEBUG_ARGS("sntp: %d, %s",current_stamp, sntp_get_real_time(current_stamp));			
		}
	}
	else
	{
		current_stamp++;
		clock_counter++;
		//LOG_DEBUG_ARGS("sntp: %d, %s",current_stamp, sntp_get_real_time(current_stamp));
	}

}



int ICACHE_FLASH_ATTR  get_counter()
{
	//initialized first time function is called.  This line is ignored every time thereafter.
	static int counter = 0;
	static int prev_stamp = 0;


	if(prev_stamp!=current_stamp)
	{
		prev_stamp=current_stamp;
		counter = 0;
	}
	else
	{
		counter++;	
	}
	return counter;
}


void ICACHE_FLASH_ATTR stop_ntp_client()
{
	os_timer_disarm(&sntp_timer);
}
