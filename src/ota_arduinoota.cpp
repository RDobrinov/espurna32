/*

ARDUINO OTA MODULE

Copyright (C) 2016-2019 by Xose Pérez <xose dot perez at gmail dot com>

*/

#include "config/hardware.h"
#include "ota_arduinoota.hpp"

#if OTA_ARDUINOOTA_SUPPORT

#include <ArduinoOTA.h>

#include "espurna32.hpp"

// TODO: allocate ArduinoOTAClass on-demand, stop using global instance

void _arduinoOtaConfigure() {

    ArduinoOTA.setPort(OTA_PORT);
    ArduinoOTA.setHostname(getSetting("hostname").c_str());
    #if USE_PASSWORD
        ArduinoOTA.setPassword(getAdminPass().c_str());
    #endif
}

void _arduinoOtaLoop() {
    ArduinoOTA.handle();
}

void _arduinoOtaOnStart() {

    DEBUG_MSG_P(PSTR("[OTA] Start for %s\n"), (ArduinoOTA.getCommand() == U_FLASH) ? "firmware" : "filesystem");

    #if WEB_SUPPORT
        wsSend_P(PSTR("{\"message\": 2}"));
    #endif

}

void _arduinoOtaOnEnd() {

    DEBUG_MSG_P(PSTR("\n"));
    DEBUG_MSG_P(PSTR("[OTA] Done, restarting...\n"));
    #if WEB_SUPPORT
        wsSend_P(PSTR("{\"action\": \"reload\"}"));
    #endif

    // Note: ArduinoOTA will reset the board after this callback returns.
    customResetReason(CUSTOM_RESET_OTA);
    delay(100);

}

void _arduinoOtaOnProgress(unsigned int progress, unsigned int total) {

    // Removed to avoid websocket ping back during upgrade (see #1574)
    // TODO: implement as separate from debugging message
    #if WEB_SUPPORT
        if (wsConnected()) return;
    #endif

    #if DEBUG_SUPPORT
        static unsigned int _progOld;

        unsigned int _prog = (progress / (total / 100));
        if (_prog != _progOld) {
            DEBUG_MSG_P(PSTR("[OTA] Progress: %u%%\r"), _prog);
            _progOld = _prog;
        }
    #endif

}

void _arduinoOtaOnError(ota_error_t error) {

    #if DEBUG_SUPPORT
        DEBUG_MSG_P(PSTR("\n[OTA] Error #%u: "), error);
        if (error == OTA_AUTH_ERROR) DEBUG_MSG_P(PSTR("Auth Failed\n"));
        else if (error == OTA_BEGIN_ERROR) DEBUG_MSG_P(PSTR("Begin Failed\n"));
        else if (error == OTA_CONNECT_ERROR) DEBUG_MSG_P(PSTR("Connect Failed\n"));
        else if (error == OTA_RECEIVE_ERROR) DEBUG_MSG_P(PSTR("Receive Failed\n"));
        else if (error == OTA_END_ERROR) DEBUG_MSG_P(PSTR("End Failed\n"));
    #endif
}

void _wifiOTACallback(justwifi_messages_t code, char * parameter) 
{
    switch(code)
    {
        case MESSAGE_CONNECTED:
        case MESSAGE_ACCESSPOINT_CREATED:
            ArduinoOTA.begin();
            #if DEBUG_SERIAL_SUPPORT
                DEBUG_MSG_P(PSTR("[OTA] Arduino OTA ready\n"));
            #endif
            break;
        //case MESSAGE_DISCONNECTED:
        //case MESSAGE_ACCESSPOINT_DESTROYED:
        //case MESSAGE_ACCESSPOINT_FAILED
        default:
            break;
    }
}

void arduinoOtaSetup() {
    espurna32RegisterLoop(_arduinoOtaLoop);
    espurna32RegisterReload(_arduinoOtaConfigure);

    ArduinoOTA.onStart(_arduinoOtaOnStart);
    ArduinoOTA.onEnd(_arduinoOtaOnEnd);
    ArduinoOTA.onError(_arduinoOtaOnError);
    ArduinoOTA.onProgress(_arduinoOtaOnProgress);

    _arduinoOtaConfigure();
    wifiRegister(_wifiOTACallback);
}

#endif // OTA_ARDUINOOTA_SUPPORT
