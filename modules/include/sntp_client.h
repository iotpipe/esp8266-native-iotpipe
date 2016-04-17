//This library maintains a fairly accurate system time.
//Every time_to_sync_in_seconds ESP8266 reaches out to NTP server using SNTP protocol to sync it's clock.
//In the 120 seconds until the next sync it updates it time (with a 1 second resolution) via a timer that is armed to repeat indefinitely once per second.
//Library also handles cases where the NTP server is unavailable and offerss a callback so the OS knows when an accurate time has been established.

#ifndef SNTP_CLIENT_H_
#define SNTP_CLIENT_H_

#include "os_type.h"

#define time_to_sync_in_seconds 120  //How many seconds between calls to NTP server to correct our clock
#define time_resolution_in_seconds 1  //resolution of our clock


//Callback used when connected connection to NTP server has been established
typedef void (*ntp_connected_cb)();


//Timer function 
static volatile os_timer_t sntp_timer;
static void ICACHE_FLASH_ATTR sntp_timerfunc();

//setup function
void ICACHE_FLASH_ATTR setup_ntp_client(ntp_connected_cb cb);

//returns a counter of how many times this function has been called in a given second.
//At the end of every second the counter is reset.  This is currently used to generate a unique guid for each mqtt request.
//as long as this function isn't called 2^32 times in a given second we'll be OK. :)
int ICACHE_FLASH_ATTR  get_counter();

#endif
