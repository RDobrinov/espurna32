/*

SYSTEM MODULE

Copyright (C) 2019 by Xose Pérez <xose dot perez at gmail dot com>

*/
#include "config/hardware.h"

#include <ArduinoJson.h>
#include <Ticker.h>

#include "system.hpp"

#include "espurna32.hpp"

#include "drivers/drv-relay.hpp"

//#include <config/general.h>
//#include "config/types.h"
#include "config/version.h"
#include "services/svc-ntp.hpp"
#include "services/svc-influxdb.hpp"

// -----------------------------------------------------------------------------

bool _system_send_heartbeat = false;
unsigned char _heartbeat_mode = HEARTBEAT_MODE;
unsigned long _heartbeat_interval = HEARTBEAT_INTERVAL;

// Calculated load average 0 to 100;
unsigned short int _load_average = 100;

// -----------------------------------------------------------------------------

union system_rtcmem_t {
    struct {
        uint8_t stability_counter;
        uint8_t reset_reason;
        uint16_t _reserved_;
    } packed;
    uint32_t value;
};

uint8_t systemStabilityCounter() {
    system_rtcmem_t data;
    data.value = Rtcmem->sys;
    return data.packed.stability_counter;
}

void systemStabilityCounter(uint8_t count) {
    system_rtcmem_t data;
    data.value = Rtcmem->sys;
    data.packed.stability_counter = count;
    Rtcmem->sys = data.value;
}

uint8_t _systemResetReason() {
    system_rtcmem_t data;
    data.value = Rtcmem->sys;
    return data.packed.reset_reason;
}

void _systemResetReason(uint8_t reason) {
    system_rtcmem_t data;
    data.value = Rtcmem->sys;
    data.packed.reset_reason = reason;
    Rtcmem->sys = data.value;
}

#if SYSTEM_CHECK_ENABLED

// Call this method on boot with start=true to increase the crash counter
// Call it again once the system is stable to decrease the counter
// If the counter reaches SYSTEM_CHECK_MAX then the system is flagged as unstable
// setting _systemOK = false;
//
// An unstable system will only have serial access, WiFi in AP mode and OTA

bool _systemStable = true;

void systemCheck(bool stable) {
    uint8_t value = 0;

    if (stable) {
        value = 0;
        DEBUG_MSG_P(PSTR("[MAIN] System OK\n"));
    } else {
        if (!rtcmemStatus()) {
            systemStabilityCounter(1);
            return;
        }

        value = systemStabilityCounter();

        if (++value > SYSTEM_CHECK_MAX) {
            _systemStable = false;
            value = 0;
            DEBUG_MSG_P(PSTR("[MAIN] System UNSTABLE\n"));
        }
    }

    systemStabilityCounter(value);
}

bool systemCheck() {
    return _systemStable;
}

void systemCheckLoop() {
    static bool checked = false;
    if (!checked && (millis() > SYSTEM_CHECK_TIME)) {
        // Flag system as stable
        systemCheck(_systemStable);
        checked = true;
    }
}

#endif

// -----------------------------------------------------------------------------
// Reset
// -----------------------------------------------------------------------------
Ticker _defer_reset;
uint8_t _reset_reason = 0;

// system_get_rst_info() result is cached by the Core init for internal use
uint32_t systemResetReason() {
    //return resetInfo.reason;
    return esp_reset_reason();
}

void customResetReason(unsigned char reason) {
    _reset_reason = reason;
    _systemResetReason(reason);
}

unsigned char customResetReason() {
    static unsigned char status = 255;
    if (status == 255) {
        if (rtcmemStatus()) status = _systemResetReason();
        if (status > 0) customResetReason(0);
        if (status > CUSTOM_RESET_MAX) status = 0;
    }
    return status;
}

void reset() {
    esp_restart();
}

void deferredReset(unsigned long delay, unsigned char reason) {
    _defer_reset.once_ms(delay, customResetReason, reason);
}

bool checkNeedsReset() {
    return _reset_reason > 0;
}

// -----------------------------------------------------------------------------

void systemSendHeartbeat() {
    _system_send_heartbeat = true;
}

bool systemGetHeartbeat() {
    return _system_send_heartbeat;
}

unsigned long systemLoadAverage() {
    return _load_average;
}

void _systemSetupHeartbeat() {
    _heartbeat_mode = getSetting("hbMode", HEARTBEAT_MODE).toInt();
    _heartbeat_interval = getSetting("hbInterval", HEARTBEAT_INTERVAL).toInt();
}

#if WEB_SUPPORT
    bool _systemWebSocketOnKeyCheck(const char * key, JsonVariant& value) {
        if (strncmp(key, "sys", 3) == 0) return true;
        if (strncmp(key, "hb", 2) == 0) return true;
        return false;
    }
#endif

// -----------------------------------------------------------------------------
// Heartbeat helper
// -----------------------------------------------------------------------------
#ifndef THERMOSTAT_SUPPORT
    #define THERMOSTAT_SUPPORT 0    //remove in final
#endif

namespace Heartbeat {

    enum Report : uint32_t { 
        Status = 1 << 1,
        Ssid = 1 << 2,
        Ip = 1 << 3,
        Mac = 1 << 4,
        Rssi = 1 << 5,
        Uptime = 1 << 6,
        Datetime = 1 << 7,
        Freeheap = 1 << 8,
        Vcc = 1 << 9,
        Relay = 1 << 10,
        Light = 1 << 11,
        Hostname = 1 << 12,
        App = 1 << 13,
        Version = 1 << 14,
        Board = 1 << 15,
        Loadavg = 1 << 16,
        Interval = 1 << 17,
        Description = 1 << 18,
        Range = 1 << 19,
        RemoteTemp = 1 << 20,
        Bssid = 1 << 21
    };

    constexpr uint32_t defaultValue() {
        return (Status * (HEARTBEAT_REPORT_STATUS)) | \
            (Ssid * (HEARTBEAT_REPORT_SSID)) | \
            (Ip * (HEARTBEAT_REPORT_IP)) | \
            (Mac * (HEARTBEAT_REPORT_MAC)) | \
            (Rssi * (HEARTBEAT_REPORT_RSSI)) | \
            (Uptime * (HEARTBEAT_REPORT_UPTIME)) | \
            (Datetime * (HEARTBEAT_REPORT_DATETIME)) | \
            (Freeheap * (HEARTBEAT_REPORT_FREEHEAP)) | \
            (Vcc * (HEARTBEAT_REPORT_VCC)) | \
            (Relay * (HEARTBEAT_REPORT_RELAY)) | \
            (Light * (HEARTBEAT_REPORT_LIGHT)) | \
            (Hostname * (HEARTBEAT_REPORT_HOSTNAME)) | \
            (Description * (HEARTBEAT_REPORT_DESCRIPTION)) | \
            (App * (HEARTBEAT_REPORT_APP)) | \
            (Version * (HEARTBEAT_REPORT_VERSION)) | \
            (Board * (HEARTBEAT_REPORT_BOARD)) | \
            (Loadavg * (HEARTBEAT_REPORT_LOADAVG)) | \
            (Interval * (HEARTBEAT_REPORT_INTERVAL)) | \
            (Range * (HEARTBEAT_REPORT_RANGE)) | \
            (RemoteTemp * (HEARTBEAT_REPORT_REMOTE_TEMP)) | \
            (Bssid * (HEARTBEAT_REPORT_BSSID));
    }

    uint32_t currentValue() {
        // use default without any setting / when it is empty
        const String cfg = getSetting("hbReport");
        if (!cfg.length()) {
            return defaultValue();
        }

        const auto value = u32fromString(cfg);

        // because we start shifting from 1, we could use the
        // first bit as a flag to enable all of the messages
        if (value == 1) {
            return std::numeric_limits<uint32_t>::max();
        }

        return value;
    }

}

void heartbeat() {

    unsigned long uptime_seconds = getUptime();
    //heap_stats_t heap_stats = getHeapStats();
    /*typedef struct {
    size_t total_free_bytes;      ///<  Total free bytes in the heap. Equivalent to multi_free_heap_size().
    size_t total_allocated_bytes; ///<  Total bytes allocated to data in the heap.
    size_t largest_free_block;    ///<  Size of largest free block in the heap. This is the largest malloc-able size.
    size_t minimum_free_bytes;    ///<  Lifetime minimum free heap size. Equivalent to multi_minimum_free_heap_size().
    size_t allocated_blocks;      ///<  Number of (variable size) blocks allocated in the heap.
    size_t free_blocks;           ///<  Number of (variable size) free blocks in the heap.
    size_t total_blocks;          ///<  Total number of (variable size) blocks in the heap.
} multi_heap_info_t;*/
    multi_heap_info_t heap_info;
    heap_caps_get_info(&heap_info, MALLOC_CAP_8BIT); //TODO: Add SPI RAM
    UNUSED(uptime_seconds);
    //UNUSED(heap_stats);

    #if MQTT_SUPPORT
        unsigned char _heartbeat_mode = getHeartbeatMode();
        bool serial = !mqttConnected();
    #else
        bool serial = true;
    #endif

    // -------------------------------------------------------------------------
    // Serial
    // -------------------------------------------------------------------------

    if (serial) {
        DEBUG_MSG_P(PSTR("[MAIN] Uptime: %lu seconds\n"), uptime_seconds);
        DEBUG_MSG_P(PSTR("[MAIN] [Heap Blocks] Allocated: %8u | Free: %8u | Total  : %8u\n"), heap_info.allocated_blocks, heap_info.free_blocks, heap_info.total_blocks);
        DEBUG_MSG_P(PSTR("[MAIN] [Heap Bytes ] Allocated: %8u | Free: %8u | Largest: %8u\n"), heap_info.total_allocated_bytes, heap_info.total_free_bytes, heap_info.largest_free_block);
        //infoHeapStats();
        /*#if ADC_MODE_VALUE == ADC_VCC
            DEBUG_MSG_P(PSTR("[MAIN] Power: %lu mV\n"), ESP.getVcc());
        #endif*/
        #if NTP_SUPPORT
            if (ntpSynced()) DEBUG_MSG_P(PSTR("[MAIN] Time: %s\n"), (char *) ntpDateTime().c_str());
        #endif
    }

    const uint32_t hb_cfg = Heartbeat::currentValue();
    if (!hb_cfg) return;

    // -------------------------------------------------------------------------
    // MQTT
    // -------------------------------------------------------------------------

    #if MQTT_SUPPORT
        if (!serial && (_heartbeat_mode == HEARTBEAT_REPEAT || systemGetHeartbeat())) {
            if (hb_cfg & Heartbeat::Interval)
                mqttSend(MQTT_TOPIC_INTERVAL, String(getHeartbeatInterval()).c_str());

            if (hb_cfg & Heartbeat::App)
                mqttSend(MQTT_TOPIC_APP, APP_NAME);

            if (hb_cfg & Heartbeat::Version)
                mqttSend(MQTT_TOPIC_VERSION, APP_VERSION);

            if (hb_cfg & Heartbeat::Board)
                mqttSend(MQTT_TOPIC_BOARD, getBoardName().c_str());

            if (hb_cfg & Heartbeat::Hostname)
                mqttSend(MQTT_TOPIC_HOSTNAME, getSetting("hostname", getIdentifier()).c_str());

            if (hb_cfg & Heartbeat::Description) {
                if (hasSetting("desc")) {
                    mqttSend(MQTT_TOPIC_DESCRIPTION, getSetting("desc").c_str());
                }
            }

            if (hb_cfg & Heartbeat::Ssid)
                mqttSend(MQTT_TOPIC_SSID, WiFi.SSID().c_str());

            if (hb_cfg & Heartbeat::Bssid)
                mqttSend(MQTT_TOPIC_BSSID, WiFi.BSSIDstr().c_str());

            if (hb_cfg & Heartbeat::Ip)
                mqttSend(MQTT_TOPIC_IP, getIP().c_str());

            if (hb_cfg & Heartbeat::Mac)
                mqttSend(MQTT_TOPIC_MAC, WiFi.macAddress().c_str());

            if (hb_cfg & Heartbeat::Rssi)
                mqttSend(MQTT_TOPIC_RSSI, String(WiFi.RSSI()).c_str());

            if (hb_cfg & Heartbeat::Uptime)
                mqttSend(MQTT_TOPIC_UPTIME, String(uptime_seconds).c_str());

            #if NTP_SUPPORT
                if ((hb_cfg & Heartbeat::Datetime) && (ntpSynced()))
                    mqttSend(MQTT_TOPIC_DATETIME, ntpDateTime().c_str());
            #endif

            if (hb_cfg & Heartbeat::Freeheap)
                mqttSend(MQTT_TOPIC_FREEHEAP, String(ESP.getFreeHeap()).c_str());

            if (hb_cfg & Heartbeat::Relay)
                relayMQTT();

            #if (LIGHT_PROVIDER != LIGHT_PROVIDER_NONE)
                if (hb_cfg & Heartbeat::Light)
                    lightMQTT();
            #endif

            /*if ((hb_cfg & Heartbeat::Vcc) && (ADC_MODE_VALUE == ADC_VCC))
                mqttSend(MQTT_TOPIC_VCC, String(ESP.getVcc()).c_str());
            */ //SKIPPING ERROR
            if (hb_cfg & Heartbeat::Status)
                mqttSendStatus();

            if (hb_cfg & Heartbeat::Loadavg)
                mqttSend(MQTT_TOPIC_LOADAVG, String(systemLoadAverage()).c_str());

            #if THERMOSTAT_SUPPORT
                if (hb_cfg & Heartbeat::Range) {
                    mqttSend(MQTT_TOPIC_HOLD_TEMP "_" MQTT_TOPIC_HOLD_TEMP_MIN, String(_temp_range.min).c_str());
                    mqttSend(MQTT_TOPIC_HOLD_TEMP "_" MQTT_TOPIC_HOLD_TEMP_MAX, String(_temp_range.max).c_str());
                }

                if (hb_cfg & Heartbeat::RemoteTemp) {
                    char remote_temp[16];
                    dtostrf(_remote_temp.temp, 1, 1, remote_temp);
                    mqttSend(MQTT_TOPIC_REMOTE_TEMP, remote_temp);
                }
            #endif

        } else if (!serial && _heartbeat_mode == HEARTBEAT_REPEAT_STATUS) {
            mqttSendStatus();
        }

    #endif

    // -------------------------------------------------------------------------
    // InfluxDB
    // -------------------------------------------------------------------------

    #if INFLUXDB_SUPPORT
        if (hb_cfg & Heartbeat::Uptime)
            idbSend(MQTT_TOPIC_UPTIME, String(uptime_seconds).c_str());

        if (hb_cfg & Heartbeat::Freeheap)
            idbSend(MQTT_TOPIC_FREEHEAP, String(ESP.getFreeHeap()).c_str());

        if (hb_cfg & Heartbeat::Rssi)
            idbSend(MQTT_TOPIC_RSSI, String(WiFi.RSSI()).c_str());

        /*if ((hb_cfg & Heartbeat::Vcc) && (ADC_MODE_VALUE == ADC_VCC))
            idbSend(MQTT_TOPIC_VCC, String(ESP.getVcc()).c_str());
        */ //class EspClass' has no member named 'getVcc'            
        if (hb_cfg & Heartbeat::Loadavg)
            idbSend(MQTT_TOPIC_LOADAVG, String(systemLoadAverage()).c_str());

        if (hb_cfg & Heartbeat::Ssid)
            idbSend(MQTT_TOPIC_SSID, WiFi.SSID().c_str());

        if (hb_cfg & Heartbeat::Bssid)
            idbSend(MQTT_TOPIC_BSSID, WiFi.BSSIDstr().c_str());
    #endif

}
///End Heartbeat

void systemLoop() {

    // -------------------------------------------------------------------------
    // User requested reset
    // -------------------------------------------------------------------------

    if (checkNeedsReset()) {
        reset();
    }

    // -------------------------------------------------------------------------
    // Check system stability
    // -------------------------------------------------------------------------

    #if SYSTEM_CHECK_ENABLED
        systemCheckLoop();
    #endif

    // -------------------------------------------------------------------------
    // Heartbeat
    // -------------------------------------------------------------------------

    if (_system_send_heartbeat && _heartbeat_mode == HEARTBEAT_ONCE) {
        heartbeat();
        _system_send_heartbeat = false;
    } else if (_heartbeat_mode == HEARTBEAT_REPEAT || _heartbeat_mode == HEARTBEAT_REPEAT_STATUS) {
        static unsigned long last_hbeat = 0;
        #if NTP_SUPPORT
            if ((_system_send_heartbeat && ntpSynced()) || (millis() - last_hbeat > _heartbeat_interval * 1000)) {
        #else
            if (_system_send_heartbeat || (millis() - last_hbeat > _heartbeat_interval * 1000)) {
        #endif
            last_hbeat = millis();
            heartbeat();
           _system_send_heartbeat = false;
        }
    }

    // -------------------------------------------------------------------------
    // Load Average calculation
    // -------------------------------------------------------------------------

    static unsigned long last_loadcheck = 0;
    static unsigned long load_counter_temp = 0;
    load_counter_temp++;

    if (millis() - last_loadcheck > LOADAVG_INTERVAL) {

        static unsigned long load_counter = 0;
        static unsigned long load_counter_max = 1;

        load_counter = load_counter_temp;
        load_counter_temp = 0;
        if (load_counter > load_counter_max) {
            load_counter_max = load_counter;
        }
        _load_average = 100 - (100 * load_counter / load_counter_max);
        last_loadcheck = millis();

    }

}

/*void _systemSetupSpecificHardware() {

    //The ESPLive has an ADC MUX which needs to be configured.
    #if defined(MANCAVEMADE_ESPLIVE)
        pinMode(16, OUTPUT);
        digitalWrite(16, HIGH); //Defualt CT input (pin B, solder jumper B)
    #endif

    // These devices use the hardware UART
    // to communicate to secondary microcontrollers
    #if (RF_SUPPORT && !RFB_DIRECT) || (RELAY_PROVIDER == RELAY_PROVIDER_DUAL) || (RELAY_PROVIDER == RELAY_PROVIDER_STM)
        Serial.begin(SERIAL_BAUDRATE);
    #endif

}*/

void systemSetup() {

    #if SPIFFS_SUPPORT
        SPIFFS.begin();
    #endif

    // Question system stability
    #if SYSTEM_CHECK_ENABLED
        systemCheck(false);
    #endif

    #if WEB_SUPPORT
        wsRegister().onKeyCheck(_systemWebSocketOnKeyCheck);
    #endif

    // Init device-specific hardware
    //_systemSetupSpecificHardware();

    // Register Loop
    espurna32RegisterLoop(systemLoop);

    // Cache Heartbeat values
    _systemSetupHeartbeat();

}
