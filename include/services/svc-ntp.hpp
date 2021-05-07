
// -----------------------------------------------------------------------------
// NTP
// -----------------------------------------------------------------------------

#ifndef NTP_SUPPORT
#define NTP_SUPPORT                 1               // Build with NTP support by default (6.78Kb)
#endif

#ifndef NTP_SERVER
#define NTP_SERVER                  "pool.ntp.org"  // Default NTP server
#endif

#ifndef NTP_TIMEOUT
#define NTP_TIMEOUT                 1000            // Set NTP request timeout to 2 seconds (issue #452)
#endif

#ifndef NTP_TIME_OFFSET
#define NTP_TIME_OFFSET             60              // Default timezone offset (GMT+1)
#endif

#ifndef NTP_DAY_LIGHT
#define NTP_DAY_LIGHT               1               // Enable daylight time saving by default
#endif

#ifndef NTP_SYNC_INTERVAL
#define NTP_SYNC_INTERVAL           60              // NTP initial check every minute
#endif

#ifndef NTP_UPDATE_INTERVAL
#define NTP_UPDATE_INTERVAL         1800            // NTP check every 30 minutes
#endif

#ifndef NTP_START_DELAY
#define NTP_START_DELAY             1000            // Delay NTP start 1 second
#endif

#ifndef NTP_DST_REGION
#define NTP_DST_REGION              0               // 0 for Europe, 1 for USA (defined in NtpClientLib)
#endif

#ifndef NTP_WAIT_FOR_SYNC
#define NTP_WAIT_FOR_SYNC           1               // Do not report any datetime until NTP sync'ed
#endif

#include <Arduino.h>

void ntpSetup();
bool ntpSynced();
String ntpDateTime(time_t t);
String ntpDateTime();
// XXX: returns garbage during DST switch
time_t ntpLocal2UTC(time_t local);