/*

INFLUXDB MODULE

Copyright (C) 2017-2019 by Xose Pérez <xose dot perez at gmail dot com>

*/

#include "config/hardware.h"

#if INFLUXDB_SUPPORT

#include <map>
#include <memory>
#include <FreeRTOS.h>
#include <AsyncTCP.h>

#include "espurna32.hpp"

#include "services/svc-influxdb.hpp"


const char INFLUXDB_REQUEST_TEMPLATE[] PROGMEM = "POST /write?db=%s&u=%s&p=%s HTTP/1.1\r\nHost: %s:%u\r\nContent-Length: %d\r\n\r\n";
const char INFLUXDB_REQUEST_TEMPLATE_V2[] PROGMEM = "POST /api/v2/write?org=%s&bucket=%s&precision=s HTTP/1.1\r\nHost: %s:%u\r\nAuthorization: Token %s\r\nContent-Length: %d\r\n\r\n";

constexpr const unsigned long INFLUXDB_CLIENT_TIMEOUT = 5000;
constexpr const size_t INFLUXDB_DATA_BUFFER_SIZE = 256;

bool _idb_enabled = false;
bool _idb_usev2 = false; //INFLUXDB_USE_V2
String _idb_host;
uint16_t _idb_port = 0;

std::map<String, String> _idb_values;
String _idb_data;
bool _idb_flush = false;

std::unique_ptr<AsyncClient> _idb_client = nullptr;
bool _idb_connecting = false;
bool _idb_connected = false;

uint32_t _idb_client_ts = 0;

// -----------------------------------------------------------------------------

void _idbInitClient() {

    _idb_client = std::make_unique<AsyncClient>();

    _idb_client->onDisconnect([](void * s, AsyncClient * client) {
        DEBUG_MSG_P(PSTR("[INFLUXDB] Disconnected\n"));
        _idb_flush = false;
        _idb_data = "";
        _idb_client_ts = 0;
        _idb_connected = false;
        _idb_connecting = false;
    }, nullptr);

    _idb_client->onTimeout([](void * s, AsyncClient * client, uint32_t time) {
        DEBUG_MSG_P(PSTR("[INFLUXDB] Network timeout after %ums\n"), time);
        client->close(true);
    }, nullptr);

    _idb_client->onData([](void * arg, AsyncClient * client, void * response, size_t len) {
        // ref: https://docs.influxdata.com/influxdb/v1.7/tools/api/#summary-table-1
        const char idb_success[] = "HTTP/1.1 204";
        const bool result = (len > sizeof(idb_success) && (0 == strncmp((char*) response, idb_success, sizeof(idb_success)-1)));
        //DEBUG_MSG_P(PSTR("[INFLUXDB] Response %s\n"), response);
        DEBUG_MSG_P(PSTR("[INFLUXDB] %s response after %ums\n"), result ? "Success" : "Failure", millis() - _idb_client_ts);
        _idb_client_ts = millis();
        client->close();
    }, nullptr);

    _idb_client->onPoll([](void * arg, AsyncClient * client) {
        unsigned long ts = millis() - _idb_client_ts;
        if (ts > INFLUXDB_CLIENT_TIMEOUT) {
            DEBUG_MSG_P(PSTR("[INFLUXDB] No response after %ums\n"), ts);
            client->close(true);
            return;
        }

        if (_idb_data.length()) {
            client->write(_idb_data.c_str(), _idb_data.length());
            _idb_data = "";
        }
    });

    _idb_client->onConnect([](void * arg, AsyncClient * client) {

        _idb_client_ts = millis();
        _idb_connected = true;
        _idb_connecting = false;

        DEBUG_MSG_P(PSTR("[INFLUXDB] Connected to %s:%u\n"),
            IPAddress(client->getRemoteAddress()).toString().c_str(),
            client->getRemotePort()
        );

        constexpr const int BUFFER_SIZE = 256;
        char headers[BUFFER_SIZE];
        int len;
        if(_idb_usev2)
        {
            len = snprintf_P(headers, sizeof(headers), INFLUXDB_REQUEST_TEMPLATE_V2,
                getSetting("idbOrg", INFLUXDB_ORG).c_str(),
                getSetting("idbBucket", INFLUXDB_BUCKET).c_str(),
                _idb_host.c_str(), _idb_port, getSetting("idbToken", INFLUXDB_TOKEN).c_str(), _idb_data.length()
            );
        }
        else
        {
            len = snprintf_P(headers, sizeof(headers), INFLUXDB_REQUEST_TEMPLATE,
                getSetting("idbDatabase", INFLUXDB_DATABASE).c_str(),
                getSetting("idbUsername", INFLUXDB_USERNAME).c_str(),
                getSetting("idbPassword", INFLUXDB_PASSWORD).c_str(),
                _idb_host.c_str(), _idb_port, _idb_data.length()
            );
        }
        if ((len < 0) || (len > BUFFER_SIZE - 1)) {
            client->close(true);
            return;
        }

        client->write(headers, len);

    });

}


// -----------------------------------------------------------------------------

bool _idbWebSocketOnKeyCheck(const char * key, JsonVariant& value) {
    return (strncmp(key, "idb", 3) == 0);
}

void _idbWebSocketOnVisible(JsonObject& root) {
    root["idbVisible"] = 1;
}

void _idbWebSocketOnConnected(JsonObject& root) {
    root["idbEnabled"] = getSetting("idbEnabled", INFLUXDB_ENABLED).toInt() == 1;
    root["idbHost"] = getSetting("idbHost", INFLUXDB_HOST);
    root["idbPort"] = getSetting("idbPort", INFLUXDB_PORT).toInt();
    root["idbDatabase"] = getSetting("idbDatabase", INFLUXDB_DATABASE);
    root["idbUsername"] = getSetting("idbUsername", INFLUXDB_USERNAME);
    root["idbPassword"] = getSetting("idbPassword", INFLUXDB_PASSWORD);
        // Influx ver. 2 engine
    root["idbUseV2"] = getSetting("idbUseV2", INFLUXDB_USE_V2).toInt() == 1;
    root["idbOrg"] = getSetting("idbOrg", INFLUXDB_ORG);
    root["idbBucket"] = getSetting("idbBucket", INFLUXDB_BUCKET);
    root["idbToken"] = getSetting("idbToken", INFLUXDB_TOKEN);
}

void _idbConfigure() {
    _idb_usev2 = getSetting("idbUseV2", INFLUXDB_USE_V2).toInt() == 1;
    _idb_enabled = getSetting("idbEnabled", INFLUXDB_ENABLED).toInt() == 1;
    if (_idb_enabled && (getSetting("idbHost", INFLUXDB_HOST).length() == 0)) {
        _idb_enabled = false;
        setSetting("idbEnabled", 0);
    }
    if (_idb_enabled && !_idb_client) _idbInitClient();
}

#if BROKER_SUPPORT

void _idbBrokerSensor(const String& topic, unsigned char id, double, const char* value) {
    idbSend(topic.c_str(), id, value);
}

void _idbBrokerStatus(const String& topic, unsigned char id, unsigned int value) {
    idbSend(topic.c_str(), id, String(int(value)).c_str());
}

#endif // BROKER_SUPPORT

// -----------------------------------------------------------------------------

bool idbSend(const char * topic, const char * payload) {
    if (!_idb_enabled) return false;
    if (_idb_connecting || _idb_connected) return false;

    _idb_values[topic] = payload;
    _idb_flush = true;

    return true;
}

void _idbSend(const String& host, const uint16_t port) {
    if (_idb_connected || _idb_connecting) return;

    DEBUG_MSG_P(PSTR("[INFLUXDB] Sending to %s:%u\n"), host.c_str(), port);

    // TODO: cache `Host: <host>:<port>` instead of storing things separately?
    _idb_host = host;
    _idb_port = port;

    _idb_client_ts = millis();
    _idb_connecting = _idb_client->connect(host.c_str(), port);

    if (!_idb_connecting) {
        DEBUG_MSG_P(PSTR("[INFLUXDB] Connection to %s:%u failed\n"), host.c_str(), port);
        _idb_client->close(true);
    }
}

void _idbFlush() {
    // Clean-up client object when not in use
    if (_idb_client && !_idb_enabled && !_idb_connected && !_idb_connecting) {
        _idb_client = nullptr;
    }

    // Wait until current connection is finished
    if (!_idb_flush) return;
    if (_idb_connected || _idb_connecting) return;

    // Wait until connected
    if (!wifiConnected()) return;

    // TODO: MDNS_CLIENT_SUPPORT is deprecated
    String host = getSetting("idbHost", INFLUXDB_HOST);
    //#if MDNS_CLIENT_SUPPORT
    //    host = mdnsResolve(host);
    //#endif

    const uint16_t port = getSetting("idbPort", INFLUXDB_PORT).toInt();

    // TODO: should we always store specific pairs like tspk keeps relay / sensor readings?
    //       note that we also send heartbeat data, persistent values should be flagged
    const String device = getSetting("hostname");

    _idb_data = "";
    for (auto& pair : _idb_values) {
        if (!isNumber(pair.second.c_str())) {
            String quoted;
            quoted.reserve(pair.second.length() + 2);
            quoted += '"';
            quoted += pair.second;
            quoted += '"';
            pair.second = quoted;
        }

        char buffer[128] = {0};
        snprintf_P(buffer, sizeof(buffer),
            PSTR("%s,device=%s value=%s\n"),
            pair.first.c_str(), device.c_str(), pair.second.c_str()
        );
        _idb_data += buffer;
    }
    _idb_values.clear();

    _idbSend(host, port);
}

bool idbSend(const char * topic, unsigned char id, const char * payload) {
    char measurement[64];
    snprintf(measurement, sizeof(measurement), "%s,id=%d", topic, id);
    return idbSend(measurement, payload);
}

bool idbEnabled() {
    return _idb_enabled;
}

void idbSetup() {

    _idbConfigure();

    #if WEB_SUPPORT
        wsRegister()
            .onVisible(_idbWebSocketOnVisible)
            .onConnected(_idbWebSocketOnConnected)
            .onKeyCheck(_idbWebSocketOnKeyCheck);
    #endif

    #if BROKER_SUPPORT
        StatusBroker::Register(_idbBrokerStatus);
        SensorReportBroker::Register(_idbBrokerSensor);
    #endif

    espurna32RegisterReload(_idbConfigure);
    espurna32RegisterLoop(_idbFlush);

    _idb_data.reserve(INFLUXDB_DATA_BUFFER_SIZE);

    #if TERMINAL_SUPPORT
        terminalRegisterCommand(F("IDB.SEND"), [](Embedis* e) {
            if (e->argc != 4) {
                terminalError(F("idb.send <topic> <id> <value>"));
                return;
            }

            const String topic = e->argv[1];
            const auto id = atoi(e->argv[2]);
            const String value = e->argv[3];

            idbSend(topic.c_str(), id, value.c_str());
        });
    #endif

}

#endif
