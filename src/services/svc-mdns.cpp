/*

MDNS MODULE

Copyright (C) 2017-2019 by Xose Pérez <xose dot perez at gmail dot com>

*/

// -----------------------------------------------------------------------------
// mDNS Server
// -----------------------------------------------------------------------------

#include "config/hardware.h"

#if MDNS_SERVER_SUPPORT

#include <ESPmDNS.h>

#include "espurna32.hpp"

#include "config/version.h"

#if MQTT_SUPPORT

void _mdnsFindMQTT() {
    int count = MDNS.queryService("mqtt", "tcp");
    DEBUG_MSG_P(PSTR("[MQTT] MQTT brokers found: %d\n"), count);
    for (int i=0; i<count; i++) {
        DEBUG_MSG_P(PSTR("[MQTT] Broker at %s:%d\n"), MDNS.IP(i).toString().c_str(), MDNS.port(i));
        mqttSetBrokerIfNone(MDNS.IP(i), MDNS.port(i));
    }
}

#endif

void _mdnsServerStart() {
    if (MDNS.begin((char *) getSetting("hostname").c_str())) {
        DEBUG_MSG_P(PSTR("[MDNS] Service is ready\n"));
    } else {
        DEBUG_MSG_P(PSTR("[MDNS] Service FAIL\n"));
    }
}

// -----------------------------------------------------------------------------

void _mdnsServerServices() {

    #if WEB_SUPPORT
        MDNS.addService("http", "tcp", getSetting("webPort", WEB_PORT).toInt());
    #endif

    #if TELNET_SUPPORT
        MDNS.addService("telnet", "tcp", TELNET_PORT);
    #endif

    // Public ESPurna related txt for OTA discovery
    MDNS.addServiceTxt("arduino", "tcp", "app_name", APP_NAME);
    MDNS.addServiceTxt("arduino", "tcp", "app_version", APP_VERSION);
    MDNS.addServiceTxt("arduino", "tcp", "build_date", buildTime());
    MDNS.addServiceTxt("arduino", "tcp", "mac", WiFi.macAddress());
    MDNS.addServiceTxt("arduino", "tcp", "target_board", getBoardName());
    {
        char buffer[6] = {0};
        itoa(spi_flash_get_chip_size() / 1024, buffer, 10);
        MDNS.addServiceTxt("arduino", "tcp", "mem_size", (const char *) buffer);
    }
    {
        char buffer[6] = {0};
        itoa(ESP.getFlashChipSize() / 1024, buffer, 10);
        MDNS.addServiceTxt("arduino", "tcp", "sdk_size", (const char *) buffer);
    }
    {
        char buffer[6] = {0};
        itoa(ESP.getFreeSketchSpace(), buffer, 10);
        MDNS.addServiceTxt("arduino", "tcp", "free_space", (const char *) buffer);
    }
}

void mdnsServerSetup() 
{
    wifiRegister([](justwifi_messages_t code, char * parameter) {

        if (code == MESSAGE_CONNECTED) {
            _mdnsServerStart();
            _mdnsServerServices();
            #if MQTT_SUPPORT
                _mdnsFindMQTT();
            #endif // MQTT_SUPPORT
        }

        if (code == MESSAGE_ACCESSPOINT_CREATED) {
            _mdnsServerStart();
            _mdnsServerServices();
        }

        if ( (code == MESSAGE_ACCESSPOINT_DESTROYED) || (code == MESSAGE_DISCONNECTED) )
            MDNS.end();

    });

}

#endif // MDNS_SERVER_SUPPORT