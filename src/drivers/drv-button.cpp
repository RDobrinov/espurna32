/*

BUTTON MODULE

Copyright (C) 2016-2019 by Xose Pérez <xose dot perez at gmail dot com>

*/

// -----------------------------------------------------------------------------
// BUTTON
// -----------------------------------------------------------------------------
#include <config/hardware.h>

#if BUTTON_SUPPORT

#include <Arduino.h>

#include <DebounceEvent.h>
#include <vector>

#include "espurna32.hpp"

#include "drivers/drv-button.hpp"
#include "drivers/drv-relay.hpp"
//#include "light.h"

typedef struct {
    DebounceEvent * button;
    unsigned long actions;
    unsigned int relayID;
} button_t;

std::vector<button_t> _buttons;

#if MQTT_SUPPORT

void buttonMQTT(unsigned char id, uint8_t event) {
    if (id >= _buttons.size()) return;
    char payload[2];
    itoa(event, payload, 10);
    mqttSend(MQTT_TOPIC_BUTTON, id, payload, false, false); // 1st bool = force, 2nd = retain
}

#endif

#if WEB_SUPPORT

unsigned char _buttonCount() {
    return _buttons.size();
}

void _buttonWebSocketOnVisible(JsonObject& root) {
    if (_buttonCount() > 0) {
        root["btnVisible"] = 1;
    }
}

bool _buttonWebSocketOnKeyCheck(const char * key, JsonVariant& value) {
    return (strncmp(key, "btn", 3) == 0);
}

#endif

int buttonFromRelay(unsigned int relayID) {
    for (unsigned int i=0; i < _buttons.size(); i++) {
        if (_buttons[i].relayID == relayID) return i;
    }
    return -1;
}

bool buttonState(unsigned char id) {
    if (id >= _buttons.size()) return false;
    return _buttons[id].button->pressed();
}

unsigned char buttonAction(unsigned char id, unsigned char event) {
    if (id >= _buttons.size()) return BUTTON_MODE_NONE;
    unsigned long actions = _buttons[id].actions;
    if (event == BUTTON_EVENT_PRESSED) return (actions) & 0x0F;
    if (event == BUTTON_EVENT_CLICK) return (actions >> 4) & 0x0F;
    if (event == BUTTON_EVENT_DBLCLICK) return (actions >> 8) & 0x0F;
    if (event == BUTTON_EVENT_LNGCLICK) return (actions >> 12) & 0x0F;
    if (event == BUTTON_EVENT_LNGLNGCLICK) return (actions >> 16) & 0x0F;
    if (event == BUTTON_EVENT_TRIPLECLICK) return (actions >> 20) & 0x0F;
    return BUTTON_MODE_NONE;
}

unsigned long buttonStore(unsigned long pressed, unsigned long click, unsigned long dblclick, unsigned long lngclick, unsigned long lnglngclick, unsigned long tripleclick) {
    unsigned int value;
    value  = pressed;
    value += click << 4;
    value += dblclick << 8;
    value += lngclick << 12;
    value += lnglngclick << 16;
    value += tripleclick << 20;
    return value;
}

uint8_t mapEvent(uint8_t event, uint8_t count, uint16_t length) {
    if (event == EVENT_PRESSED) return BUTTON_EVENT_PRESSED;
    if (event == EVENT_CHANGED) return BUTTON_EVENT_CLICK;
    if (event == EVENT_RELEASED) {
        if (1 == count) {
            if (length > BUTTON_LNGLNGCLICK_DELAY) return BUTTON_EVENT_LNGLNGCLICK;
            if (length > BUTTON_LNGCLICK_DELAY) return BUTTON_EVENT_LNGCLICK;
            return BUTTON_EVENT_CLICK;
        }
        if (2 == count) return BUTTON_EVENT_DBLCLICK;
        if (3 == count) return BUTTON_EVENT_TRIPLECLICK;
    }
    return BUTTON_EVENT_NONE;
}

void buttonEvent(unsigned int id, unsigned char event) {

    DEBUG_MSG_P(PSTR("[BTNS] Button #%u event %u\n"), id, event);
    if (event == 0) return;

    unsigned char action = buttonAction(id, event);

    #if MQTT_SUPPORT
       if (action != BUTTON_MODE_NONE || BUTTON_MQTT_SEND_ALL_EVENTS) {
           buttonMQTT(id, event);
       }
    #endif

    if (BUTTON_MODE_TOGGLE == action) {
        if (_buttons[id].relayID > 0) {
            relayToggle(_buttons[id].relayID - 1);
        }
    }

    if (BUTTON_MODE_ON == action) {
        if (_buttons[id].relayID > 0) {
            relayStatus(_buttons[id].relayID - 1, true);
        }
    }

    if (BUTTON_MODE_OFF == action) {
        if (_buttons[id].relayID > 0) {
            relayStatus(_buttons[id].relayID - 1, false);
        }
    }
    
    if (BUTTON_MODE_AP == action) {
        if (wifiState() & WIFI_STATE_AP) {
            wifiStartSTA();
        } else {
            wifiStartAP();
        }
    }
    
    if (BUTTON_MODE_RESET == action) {
        deferredReset(100, CUSTOM_RESET_HARDWARE);
    }

    if (BUTTON_MODE_FACTORY == action) {
        DEBUG_MSG_P(PSTR("\n[KRNL] FACTORY RESET\n\n"));
        resetSettings();
        deferredReset(100, CUSTOM_RESET_FACTORY);
    }

    #if defined(JUSTWIFI_ENABLE_WPS)
        if (BUTTON_MODE_WPS == action) {
            wifiStartWPS();
        }
    #endif // defined(JUSTWIFI_ENABLE_WPS)
    
    #if defined(JUSTWIFI_ENABLE_SMARTCONFIG)
        if (BUTTON_MODE_SMART_CONFIG == action) {
            wifiStartSmartConfig();
        }
    #endif // defined(JUSTWIFI_ENABLE_SMARTCONFIG)
    
    #if LIGHT_PROVIDER != LIGHT_PROVIDER_NONE
    if (BUTTON_MODE_DIM_UP == action) {
        lightBrightnessStep(1);
        lightUpdate(true, true);
    }
    if (BUTTON_MODE_DIM_DOWN == action) {
        lightBrightnessStep(-1);
        lightUpdate(true, true);
    }
    #endif // LIGHT_PROVIDER != LIGHT_PROVIDER_NONE

}

void buttonLoop() {
    for (unsigned int i=0; i < _buttons.size(); i++) {
        if (unsigned char event = _buttons[i].button->loop()) {
            unsigned char count = _buttons[i].button->getEventCount();
            unsigned long length = _buttons[i].button->getEventLength();
            unsigned char mapped = mapEvent(event, count, length);
            buttonEvent(i, mapped);
        }
    }
}

void buttonSetup() 
{
    unsigned long btnDelay = getSetting("btnDelay", BUTTON_DBLCLICK_DELAY).toInt();
    UNUSED(btnDelay);

    #if BUTTON1_PIN != GPIO_NONE
    {
        unsigned int actions = buttonStore(BUTTON1_PRESS, BUTTON1_CLICK, BUTTON1_DBLCLICK, BUTTON1_LNGCLICK, BUTTON1_LNGLNGCLICK, BUTTON1_TRIPLECLICK);
        _buttons.push_back({new DebounceEvent(BUTTON1_PIN, BUTTON1_MODE, BUTTON_DEBOUNCE_DELAY, btnDelay), actions, BUTTON1_RELAY});
    }
    #endif
    #if BUTTON2_PIN != GPIO_NONE
    {
        unsigned int actions = buttonStore(BUTTON2_PRESS, BUTTON2_CLICK, BUTTON2_DBLCLICK, BUTTON2_LNGCLICK, BUTTON2_LNGLNGCLICK, BUTTON2_TRIPLECLICK);
        _buttons.push_back({new DebounceEvent(BUTTON2_PIN, BUTTON2_MODE, BUTTON_DEBOUNCE_DELAY, btnDelay), actions, BUTTON2_RELAY});
    }
    #endif
    #if BUTTON3_PIN != GPIO_NONE
    {
        unsigned int actions = buttonStore(BUTTON3_PRESS, BUTTON3_CLICK, BUTTON3_DBLCLICK, BUTTON3_LNGCLICK, BUTTON3_LNGLNGCLICK, BUTTON3_TRIPLECLICK);
        _buttons.push_back({new DebounceEvent(BUTTON3_PIN, BUTTON3_MODE, BUTTON_DEBOUNCE_DELAY, btnDelay), actions, BUTTON3_RELAY});
    }
    #endif
    #if BUTTON4_PIN != GPIO_NONE
    {
        unsigned int actions = buttonStore(BUTTON4_PRESS, BUTTON4_CLICK, BUTTON4_DBLCLICK, BUTTON4_LNGCLICK, BUTTON4_LNGLNGCLICK, BUTTON4_TRIPLECLICK);
        _buttons.push_back({new DebounceEvent(BUTTON4_PIN, BUTTON4_MODE, BUTTON_DEBOUNCE_DELAY, btnDelay), actions, BUTTON4_RELAY});
    }
    #endif
    #if BUTTON5_PIN != GPIO_NONE
    {
        unsigned int actions = buttonStore(BUTTON5_PRESS, BUTTON5_CLICK, BUTTON5_DBLCLICK, BUTTON5_LNGCLICK, BUTTON5_LNGLNGCLICK, BUTTON5_TRIPLECLICK);
        _buttons.push_back({new DebounceEvent(BUTTON5_PIN, BUTTON5_MODE, BUTTON_DEBOUNCE_DELAY, btnDelay), actions, BUTTON5_RELAY});
    }
    #endif
    #if BUTTON6_PIN != GPIO_NONE
    {
        unsigned int actions = buttonStore(BUTTON6_PRESS, BUTTON6_CLICK, BUTTON6_DBLCLICK, BUTTON6_LNGCLICK, BUTTON6_LNGLNGCLICK, BUTTON6_TRIPLECLICK);
        _buttons.push_back({new DebounceEvent(BUTTON6_PIN, BUTTON6_MODE, BUTTON_DEBOUNCE_DELAY, btnDelay), actions, BUTTON6_RELAY});
    }
    #endif
    #if BUTTON7_PIN != GPIO_NONE
    {
        unsigned int actions = buttonStore(BUTTON7_PRESS, BUTTON7_CLICK, BUTTON7_DBLCLICK, BUTTON7_LNGCLICK, BUTTON7_LNGLNGCLICK, BUTTON7_TRIPLECLICK);
        _buttons.push_back({new DebounceEvent(BUTTON7_PIN, BUTTON7_MODE, BUTTON_DEBOUNCE_DELAY, btnDelay), actions, BUTTON7_RELAY});
    }
    #endif
    #if BUTTON8_PIN != GPIO_NONE
    {
        unsigned int actions = buttonStore(BUTTON8_PRESS, BUTTON8_CLICK, BUTTON8_DBLCLICK, BUTTON8_LNGCLICK, BUTTON8_LNGLNGCLICK, BUTTON8_TRIPLECLICK);
        _buttons.push_back({new DebounceEvent(BUTTON8_PIN, BUTTON8_MODE, BUTTON_DEBOUNCE_DELAY, btnDelay), actions, BUTTON8_RELAY});
    }
    #endif

    DEBUG_MSG_P(PSTR("[BTNS] Number of buttons: %u\n"), _buttons.size());

    // Websocket Callbacks
    #if WEB_SUPPORT
        wsRegister()
            .onVisible(_buttonWebSocketOnVisible)
            .onKeyCheck(_buttonWebSocketOnKeyCheck);
    #endif

    // Register loop
    espurna32RegisterLoop(buttonLoop);

}

#endif // BUTTON_SUPPORT
