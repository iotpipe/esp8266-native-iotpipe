//This library maintains a fairly accurate system time.
//Every time_to_sync_in_seconds ESP8266 reaches out to NTP server using SNTP protocol to sync it's clock.
//In the 120 seconds until the next sync it updates it time (with a 1 second resolution) via a timer that is armed to repeat indefinitely once per second.
//Library also handles cases where the NTP server is unavailable and offerss a callback so the OS knows when an accurate time has been established.

#ifndef IOTPIPE_SNTP_H_
#define IOTPIPE_SNTP_H_

#include "os_type.h"
#include "iotpipe_utils.h"

#define sntp_server_1 "us.pool.ntp.org"
#define sntp_server_2 "europe.pool.ntp.org"
#define sntp_server_3 "asia.pool.ntp.org"


//setup function
void ICACHE_FLASH_ATTR setup_sntp();

bool ICACHE_FLASH_ATTR getEpochTimeInMs(char *buf, int bufLen);

#endif
