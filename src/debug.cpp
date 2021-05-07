/*

DEBUG MODULE

Copyright (C) 2016-2019 by Xose Pérez <xose dot perez at gmail dot com>

*/
//#if DEBUG_SUPPORT
//#warning "Debug support"

#include "config/hardware.h"
#include "debug.hpp"

#if DEBUG_SUPPORT

#include "nvkeyvalue.hpp"
#include "websockets.hpp"

#if NTP_SUPPORT
#include <TimeLib.h>
#include "services/svc-ntp.hpp"
#endif

void _debugSend(const char * format, va_list args) {

    char temp[64];
    int len = vsnprintf(temp, sizeof(temp), format, args);
    if (len < 64) { debugSendImpl(temp); return; }

    auto buffer = new char[len + 1];
    vsnprintf(buffer, len + 1, format, args);

    debugSendImpl(buffer);

    delete[] buffer;

}

void debugSend(const char* format, ...) {

    va_list args;
    va_start(args, format);

    _debugSend(format, args);

    va_end(args);

}

void debugSend_P(PGM_P format_P, ...) {

    char format[strlen_P(format_P) + 1];
    memcpy_P(format, format_P, sizeof(format));

    va_list args;
    va_start(args, format_P);

    _debugSend(format, args);

    va_end(args);

}

#if DEBUG_UDP_SUPPORT
#include <WiFiUdp.h>
WiFiUDP _udp_debug;
#if DEBUG_UDP_PORT == 514
char _udp_syslog_header[40] = {0};
#endif
#endif

#if DEBUG_SERIAL_SUPPORT
    void _debugSendSerial(const char* prefix, const char* data) {
        if (prefix && (prefix[0] != '\0')) {
            DEBUG_PORT.print(prefix);
        }
        DEBUG_PORT.print(data);

    }
#endif

#if DEBUG_TELNET_SUPPORT
    void _debugSendTelnet(const char* prefix, const char* data) {
        if (prefix && (prefix[0] != '\0')) {
            _telnetWrite(prefix);
        }
        _telnetWrite(data);

    }
#endif

void debugSendImpl(const char * message) {

    const size_t msg_len = strlen(message);

    bool pause = false;
    #if NTP_SUPPORT
        char timestamp[25] = {0};
    #else
        char timestamp[10] = {0};
    #endif

    #if DEBUG_ADD_TIMESTAMP
        static bool add_timestamp = true;
        if (add_timestamp) //dot@BG
        {   
            #if NTP_SUPPORT
                time_t t = now();
                if (ntpSynced()) snprintf(timestamp, sizeof(timestamp), "[%s] ", ntpDateTime(t).c_str()); // Current time
                else snprintf(timestamp, sizeof(timestamp), "[%8lu] ", millis() % 1000000);
            #else
                snprintf(timestamp, sizeof(timestamp), "[%10lu] ", millis() % 1000000);             //add timestamp
            #endif

        }
        add_timestamp = (message[msg_len - 1] == 10) || (message[msg_len - 1] == 13);
    #endif

    #if DEBUG_SERIAL_SUPPORT
        _debugSendSerial(timestamp, message);
    #endif

    #if DEBUG_UDP_SUPPORT
        #if SYSTEM_CHECK_ENABLED
        if (systemCheck()) {
        #endif
            _udp_debug.beginPacket(DEBUG_UDP_IP, DEBUG_UDP_PORT);
            #if DEBUG_UDP_PORT == 514
                _udp_debug.write(_udp_syslog_header);
            #endif
            _udp_debug.write(message);
            _udp_debug.endPacket();
            pause = true;
        #if SYSTEM_CHECK_ENABLED
        }
        #endif
    #endif

    #if DEBUG_TELNET_SUPPORT
        _debugSendTelnet(timestamp, message);
        pause = true;
    #endif

    #if DEBUG_WEB_SUPPORT
        wsDebugSend(timestamp, message);
        pause = true;
    #endif

    if (pause) optimistic_yield(100);

}


#if DEBUG_WEB_SUPPORT

void _debugWebSocketOnAction(uint32_t client_id, const char * action, JsonObject& data) {

        #if TERMINAL_SUPPORT
            if (strcmp(action, "dbgcmd") == 0) {
                if (!data.containsKey("command") || !data["command"].is<const char*>()) return;
                const char* command = data["command"];
                if (command && strlen(command)) {
                    auto command = data.get<const char*>("command");
                    terminalInject((void*) command, strlen(command));
                    terminalInject('\n');
                }
            }
        #endif
}

void debugWebSetup() {

    wsRegister()
        .onVisible([](JsonObject& root) { root["dbgVisible"] = 1; })
        .onAction(_debugWebSocketOnAction);

    #if DEBUG_UDP_SUPPORT
        #if DEBUG_UDP_PORT == 514
            snprintf_P(_udp_syslog_header, sizeof(_udp_syslog_header), PSTR("<%u>%s ESPurna[0]: "), DEBUG_UDP_FAC_PRI, getSetting("hostname").c_str());
        #endif
    #endif


}

#endif // DEBUG_WEB_SUPPORT

// -----------------------------------------------------------------------------

void debugSetup() {

    #if DEBUG_SERIAL_SUPPORT
        DEBUG_PORT.begin(SERIAL_BAUDRATE);
        DEBUG_PORT.println("...");
        #if DEBUG_ESP_WIFI
            DEBUG_PORT.setDebugOutput(true);
        #endif
    #endif
}

#endif // DEBUG_SUPPORT
