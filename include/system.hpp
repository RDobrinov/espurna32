#ifndef HOSTNAME
#define HOSTNAME                ""
#endif

//------------------------------------------------------------------------------
// SYSTEM CHECK
//------------------------------------------------------------------------------

#ifndef SYSTEM_CHECK_ENABLED
#define SYSTEM_CHECK_ENABLED    1               // Enable crash check by default
#endif

#ifndef SYSTEM_CHECK_TIME
#define SYSTEM_CHECK_TIME       60000           // The system is considered stable after these many millis
#endif

#ifndef SYSTEM_CHECK_MAX
#define SYSTEM_CHECK_MAX        5               // After this many crashes on boot
                                                // the system is flagged as unstable
#endif

//------------------------------------------------------------------------------
// HEARTBEAT
//------------------------------------------------------------------------------

#define HEARTBEAT_NONE              0           // Never send heartbeat
#define HEARTBEAT_ONCE              1           // Send it only once upon MQTT connection
#define HEARTBEAT_REPEAT            2           // Send it upon MQTT connection and every HEARTBEAT_INTERVAL
#define HEARTBEAT_REPEAT_STATUS     3           // Send it upon MQTT connection and every HEARTBEAT_INTERVAL only STATUS report

// Backwards compatibility check
#if defined(HEARTBEAT_ENABLED) && (HEARTBEAT_ENABLED == 0)
#define HEARTBEAT_MODE              HEARTBEAT_NONE
#endif

#ifndef HEARTBEAT_MODE
#define HEARTBEAT_MODE              HEARTBEAT_REPEAT
#endif

#ifndef HEARTBEAT_INTERVAL
#define HEARTBEAT_INTERVAL          300         // Interval between heartbeat messages (in sec)
#endif

#define UPTIME_OVERFLOW             4294967295  // Uptime overflow value

// Values that will be reported in heartbeat
#ifndef HEARTBEAT_REPORT_STATUS
#define HEARTBEAT_REPORT_STATUS     1
#endif

#ifndef HEARTBEAT_REPORT_SSID
#define HEARTBEAT_REPORT_SSID       1
#endif

#ifndef HEARTBEAT_REPORT_IP
#define HEARTBEAT_REPORT_IP         1
#endif

#ifndef HEARTBEAT_REPORT_MAC
#define HEARTBEAT_REPORT_MAC        1
#endif

#ifndef HEARTBEAT_REPORT_RSSI
#define HEARTBEAT_REPORT_RSSI       1
#endif

#ifndef HEARTBEAT_REPORT_UPTIME
#define HEARTBEAT_REPORT_UPTIME     1
#endif

#ifndef HEARTBEAT_REPORT_DATETIME
#define HEARTBEAT_REPORT_DATETIME   1
#endif

#ifndef HEARTBEAT_REPORT_FREEHEAP
#define HEARTBEAT_REPORT_FREEHEAP   1
#endif

#ifndef HEARTBEAT_REPORT_VCC
#define HEARTBEAT_REPORT_VCC        1
#endif

#ifndef HEARTBEAT_REPORT_RELAY
#define HEARTBEAT_REPORT_RELAY      1
#endif

#ifndef HEARTBEAT_REPORT_LIGHT
#define HEARTBEAT_REPORT_LIGHT      1
#endif

#ifndef HEARTBEAT_REPORT_HOSTNAME
#define HEARTBEAT_REPORT_HOSTNAME   1
#endif

#ifndef HEARTBEAT_REPORT_DESCRIPTION
#define HEARTBEAT_REPORT_DESCRIPTION 1
#endif

#ifndef HEARTBEAT_REPORT_APP
#define HEARTBEAT_REPORT_APP        1
#endif

#ifndef HEARTBEAT_REPORT_VERSION
#define HEARTBEAT_REPORT_VERSION    1
#endif

#ifndef HEARTBEAT_REPORT_BOARD
#define HEARTBEAT_REPORT_BOARD      1
#endif

#ifndef HEARTBEAT_REPORT_LOADAVG
#define HEARTBEAT_REPORT_LOADAVG    1
#endif

#ifndef HEARTBEAT_REPORT_INTERVAL
#define HEARTBEAT_REPORT_INTERVAL   0
#endif

#ifndef HEARTBEAT_REPORT_RANGE
#define HEARTBEAT_REPORT_RANGE         THERMOSTAT_SUPPORT
#endif

#ifndef HEARTBEAT_REPORT_REMOTE_TEMP
#define HEARTBEAT_REPORT_REMOTE_TEMP   THERMOSTAT_SUPPORT
#endif

#ifndef HEARTBEAT_REPORT_BSSID
#define HEARTBEAT_REPORT_BSSID       0
#endif

//------------------------------------------------------------------------------
// RESET
//------------------------------------------------------------------------------

#define CUSTOM_RESET_HARDWARE       1       // Reset from hardware button
#define CUSTOM_RESET_WEB            2       // Reset from web interface
#define CUSTOM_RESET_TERMINAL       3       // Reset from terminal
#define CUSTOM_RESET_MQTT           4       // Reset via MQTT
#define CUSTOM_RESET_RPC            5       // Reset via RPC (HTTP)
#define CUSTOM_RESET_OTA            6       // Reset after successful OTA update
#define CUSTOM_RESET_HTTP           7       // Reset via HTTP GET
#define CUSTOM_RESET_NOFUSS         8       // Reset after successful NOFUSS update
#define CUSTOM_RESET_UPGRADE        9       // Reset after update from web interface
#define CUSTOM_RESET_FACTORY        10      // Factory reset from terminal

#define CUSTOM_RESET_MAX            10

//------------------------------------------------------------------------------
// Load average
//------------------------------------------------------------------------------

#ifndef LOADAVG_INTERVAL
#define LOADAVG_INTERVAL        30000           // Interval between calculating load average (in ms)
#endif

void systemSetup();
bool systemCheck();
void customResetReason(unsigned char reason);
unsigned char customResetReason();
void deferredReset(unsigned long delay, unsigned char reason);
unsigned long systemLoadAverage();
void systemSendHeartbeat();