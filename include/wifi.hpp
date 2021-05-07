#ifndef INC_WIFI_HPP
#define INC_WIFI_HPP
// WIFI
// -----------------------------------------------------------------------------

#ifndef WIFI_CONNECT_TIMEOUT
#define WIFI_CONNECT_TIMEOUT        60000               // Connecting timeout for WIFI in ms
#endif

#ifndef WIFI_RECONNECT_INTERVAL
#define WIFI_RECONNECT_INTERVAL     180000              // If could not connect to WIFI, retry after this time in ms
#endif

#ifndef WIFI_MAX_NETWORKS
#define WIFI_MAX_NETWORKS           5                   // Max number of WIFI connection configurations
#endif

#ifndef WIFI_AP_CAPTIVE
#define WIFI_AP_CAPTIVE             1                   // Captive portal enabled when in AP mode
#endif

#ifndef WIFI_FALLBACK_APMODE
#define WIFI_FALLBACK_APMODE        1                   // Fallback to AP mode if no STA connection
#endif

#ifndef WIFI_SLEEP_MODE
#define WIFI_SLEEP_MODE             WIFI_PS_NONE        // or WIFI_PS_MIN_MODEM, WIFI_PS_MAX_MODEM
#endif

#ifndef WIFI_SCAN_NETWORKS
#define WIFI_SCAN_NETWORKS          1                   // Perform a network scan before connecting
#endif

// Optional hardcoded configuration (up to 2 networks)
#ifndef WIFI1_SSID
#define WIFI1_SSID                  ""
#endif

#ifndef WIFI1_PASS
#define WIFI1_PASS                  ""
#endif

#ifndef WIFI1_IP
#define WIFI1_IP                    ""
#endif

#ifndef WIFI1_GW
#define WIFI1_GW                    ""
#endif

#ifndef WIFI1_MASK
#define WIFI1_MASK                  ""
#endif

#ifndef WIFI1_DNS
#define WIFI1_DNS                   ""
#endif

#ifndef WIFI2_SSID
#define WIFI2_SSID                  ""
#endif

#ifndef WIFI2_PASS
#define WIFI2_PASS                  ""
#endif

#ifndef WIFI2_IP
#define WIFI2_IP                    ""
#endif

#ifndef WIFI2_GW
#define WIFI2_GW                    ""
#endif

#ifndef WIFI2_MASK
#define WIFI2_MASK                  ""
#endif

#ifndef WIFI2_DNS
#define WIFI2_DNS                   ""
#endif

#ifndef WIFI3_SSID
#define WIFI3_SSID                  ""
#endif

#ifndef WIFI3_PASS
#define WIFI3_PASS                  ""
#endif

#ifndef WIFI3_IP
#define WIFI3_IP                    ""
#endif

#ifndef WIFI3_GW
#define WIFI3_GW                    ""
#endif

#ifndef WIFI3_MASK
#define WIFI3_MASK                  ""
#endif

#ifndef WIFI3_DNS
#define WIFI3_DNS                   ""
#endif

#ifndef WIFI4_SSID
#define WIFI4_SSID                  ""
#endif

#ifndef WIFI4_PASS
#define WIFI4_PASS                  ""
#endif

#ifndef WIFI4_IP
#define WIFI4_IP                    ""
#endif

#ifndef WIFI4_GW
#define WIFI4_GW                    ""
#endif

#ifndef WIFI4_MASK
#define WIFI4_MASK                  ""
#endif

#ifndef WIFI4_DNS
#define WIFI4_DNS                   ""
#endif

#ifndef WIFI_RSSI_1M
#define WIFI_RSSI_1M                -30         // Calibrate it with your router reading the RSSI at 1m
#endif

#ifndef WIFI_PROPAGATION_CONST
#define WIFI_PROPAGATION_CONST      4           // This is typically something between 2.7 to 4.3 (free space is 2)
#endif

// ref: https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/kconfig.html#config-lwip-esp-gratuitous-arp
// ref: https://github.com/xoseperez/espurna/pull/1877#issuecomment-525612546 
//
// Broadcast gratuitous ARP periodically to update ARP tables on the AP and all devices on the same network.
// Helps to solve compatibility issues when ESP fails to timely reply to ARP requests, causing the device's ARP table entry to expire.

#ifndef WIFI_GRATUITOUS_ARP_SUPPORT
#define WIFI_GRATUITOUS_ARP_SUPPORT              0
#endif

// Interval is randomized on each boot in range from ..._MIN to ..._MAX (ms)
#ifndef WIFI_GRATUITOUS_ARP_INTERVAL_MIN
#define WIFI_GRATUITOUS_ARP_INTERVAL_MIN         15000
#endif

#ifndef WIFI_GRATUITOUS_ARP_INTERVAL_MAX
#define WIFI_GRATUITOUS_ARP_INTERVAL_MAX         30000
#endif

// ref: https://github.com/esp8266/Arduino/issues/6471
// ref: https://github.com/esp8266/Arduino/issues/6366
//
// Issue #6366 turned out to be high tx power causing weird behavior. Lowering tx power achieved stability.
// ESP32 definitions
#ifndef WIFI_OUTPUT_POWER_DBM
/*
    WIFI_POWER_19_5dBm  => 19.5dBm
    WIFI_POWER_19dBm    => 19dBm
    WIFI_POWER_18_5dBm  => 18.5dBm
    WIFI_POWER_17dBm    => 17dBm
    WIFI_POWER_15dBm    => 15dBm
    WIFI_POWER_13dBm    => 13dBm
    WIFI_POWER_11dBm    => 11dBm
    WIFI_POWER_8_5dBm   => 8.5dBm
    WIFI_POWER_7dBm     => 7dBm
    WIFI_POWER_5dBm     => 5dBm
    WIFI_POWER_2dBm     => 2dBm
    WIFI_POWER_MINUS_1dBm   => -1dBm
*/
#define WIFI_OUTPUT_POWER_DBM                    WIFI_POWER_19_5dBm
#endif

#define WIFI_STATE_AP               1
#define WIFI_STATE_STA              2
#define WIFI_STATE_AP_STA           3
#define WIFI_STATE_WPS              4
#define WIFI_STATE_SMARTCONFIG      8

#define WIFI_AP_ALLWAYS             1
#define WIFI_AP_FALLBACK            2

#include <Arduino.h>
#include "JustWifi.h"

struct wifi_scan_info_t {
    String ssid_scan;
    int32_t rssi_scan;
    uint8_t sec_scan;
    uint8_t* BSSID_scan;
    int32_t chan_scan;
    bool hidden_scan;
    char buffer[128];
};

using wifi_scan_f = std::function<void(wifi_scan_info_t& info)>;
using wifi_callback_f = std::function<void(justwifi_messages_t code, char * parameter)>;

bool _wifiClean(unsigned char num);

void wifiDebug(WiFiMode_t modes);
void wifiDebug();
String getIP();
String getNetwork();
bool wifiConnected();
void wifiDisconnect();
void wifiStartSTA();
void wifiStartAP(bool only);
void wifiStartAP();

#if defined(JUSTWIFI_ENABLE_WPS)
    void wifiStartWPS();
#endif // defined(JUSTWIFI_ENABLE_WPS)

#if defined(JUSTWIFI_ENABLE_SMARTCONFIG)
    void wifiStartSmartConfig();
#endif // defined(JUSTWIFI_ENABLE_SMARTCONFIG)

void wifiReconnectCheck();
uint8_t wifiState();

void wifiRegister(wifi_callback_f callback);
void wifiSetup();

void wifiLoop();

#endif /* INC_WIFI_HPP */