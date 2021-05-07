
#ifndef INC_MQTTBROKER_HPP
#define INC_MQTTBROKER_HPP
// -----------------------------------------------------------------------------
// MQTT OVER SSL
// -----------------------------------------------------------------------------
//
// Requires SECURE_CLIENT set to SECURE_CLIENT_AXTLS or SECURE_CLIENT_BEARSSL
// It is recommended to use MQTT_LIBRARY_ARDUINOMQTT or MQTT_LIBRARY_PUBSUBCLIENT
// It is recommended to use SECURE_CLIENT_BEARSSL
// It is recommended to use ESP8266 Arduino Core >= 2.5.2 with SECURE_CLIENT_BEARSSL
//
// Current version of MQTT_LIBRARY_ASYNCMQTTCLIENT only supports SECURE_CLIENT_AXTLS
//
// It is recommended to use WEB_SUPPORT=0 with either SECURE_CLIENT option, as there are miscellaneous problems when using them simultaneously
// (although, things might've improved, and I'd encourage to check whether this is true or not)
//
// When using MQTT_LIBRARY_PUBSUBCLIENT or MQTT_LIBRARY_ARDUINOMQTT, you will have to disable every module that uses ESPAsyncTCP:
// ALEXA_SUPPORT=0, INFLUXDB_SUPPORT=0, TELNET_SUPPORT=0, THINGSPEAK_SUPPORT=0, DEBUG_TELNET_SUPPORT=0 and WEB_SUPPORT=0
// Or, use "sync" versions instead (note that not every module has this option):
// THINGSPEAK_USE_ASYNC=0, TELNET_SERVER=TELNET_SERVER_WIFISERVER
//
// See SECURE_CLIENT_CHECK for all possible connection verification options.
//
// The simpliest way to verify SSL connection is to use fingerprinting.
// For example, to get Google's MQTT server certificate fingerprint, run the following command:
// $ echo -n | openssl s_client -connect mqtt.googleapis.com:8883 2>&1 | openssl x509 -noout -fingerprint -sha1 | cut -d\= -f2
// Note that fingerprint will change when certificate changes e.g. LetsEncrypt renewals or when the CSR updates

#ifndef MQTT_SSL_ENABLED
#define MQTT_SSL_ENABLED            0               // By default MQTT over SSL will not be enabled
#endif

#ifndef MQTT_SSL_FINGERPRINT
#define MQTT_SSL_FINGERPRINT        ""              // SSL fingerprint of the server
#endif

#ifndef MQTT_SECURE_CLIENT_CHECK
#define MQTT_SECURE_CLIENT_CHECK    SECURE_CLIENT_CHECK // Use global verification setting by default
#endif

#ifndef MQTT_SECURE_CLIENT_MFLN
#define MQTT_SECURE_CLIENT_MFLN     SECURE_CLIENT_MFLN  // Use global MFLN setting by default 
#endif

#ifndef MQTT_SECURE_CLIENT_INCLUDE_CA
#define MQTT_SECURE_CLIENT_INCLUDE_CA        0           // Use user-provided CA. Only PROGMEM PEM option is supported.
                                                         // When enabled, current implementation includes "static/mqtt_client_trusted_root_ca.h" with
                                                         // const char _mqtt_client_trusted_root_ca[] PROGMEM = "...PEM data...";
                                                         // By default, using LetsEncrypt X3 root in "static/letsencrypt_isrgroot_pem.h"
#endif

#ifndef MQTT_ENABLED
#define MQTT_ENABLED                0               // Do not enable MQTT connection by default
#endif

#ifndef MQTT_AUTOCONNECT
#define MQTT_AUTOCONNECT            1               // If enabled and MDNS_SERVER_SUPPORT=1 will perform an autodiscover and
                                                    // autoconnect to the first MQTT broker found if none defined
#endif

#ifndef MQTT_SERVER
#define MQTT_SERVER                 ""              // Default MQTT broker address
#endif

#ifndef MQTT_USER
#define MQTT_USER                   ""              // Default MQTT broker usename
#endif

#ifndef MQTT_PASS
#define MQTT_PASS                   ""              // Default MQTT broker password
#endif

#ifndef MQTT_PORT
#define MQTT_PORT                   1883            // MQTT broker port
#endif

#ifndef MQTT_TOPIC
#define MQTT_TOPIC                  "{hostname}"    // Default MQTT base topic
#endif

#ifndef MQTT_RETAIN
#define MQTT_RETAIN                 true            // MQTT retain flag
#endif

#ifndef MQTT_QOS
#define MQTT_QOS                    0               // MQTT QoS value for all messages
#endif

#ifndef MQTT_KEEPALIVE
#define MQTT_KEEPALIVE              300             // MQTT keepalive value
#endif


#ifndef MQTT_RECONNECT_DELAY_MIN
#define MQTT_RECONNECT_DELAY_MIN    5000            // Try to reconnect in 5 seconds upon disconnection
#endif

#ifndef MQTT_RECONNECT_DELAY_STEP
#define MQTT_RECONNECT_DELAY_STEP   5000            // Increase the reconnect delay in 5 seconds after each failed attempt
#endif

#ifndef MQTT_RECONNECT_DELAY_MAX
#define MQTT_RECONNECT_DELAY_MAX    120000          // Set reconnect time to 2 minutes at most
#endif


#ifndef MQTT_SKIP_RETAINED
#define MQTT_SKIP_RETAINED          1               // Skip retained messages on connection
#endif

#ifndef MQTT_SKIP_TIME
#define MQTT_SKIP_TIME              1000            // Skip messages for 1 second anter connection
#endif


#if THERMOSTAT_SUPPORT == 1
    #ifndef MQTT_USE_JSON
    #define MQTT_USE_JSON               1           // Group messages in a JSON body
    #endif
#else
    #ifndef MQTT_USE_JSON
    #define MQTT_USE_JSON               0           // Don't group messages in a JSON body (default)
    #endif
#endif

#ifndef MQTT_USE_JSON_DELAY
#define MQTT_USE_JSON_DELAY         100             // Wait this many ms before grouping messages
#endif

#ifndef MQTT_QUEUE_MAX_SIZE
#define MQTT_QUEUE_MAX_SIZE         20              // Size of the MQTT queue when MQTT_USE_JSON is enabled
#endif


// These are the properties that will be sent when useJson is true
#ifndef MQTT_ENQUEUE_IP
#define MQTT_ENQUEUE_IP             1
#endif

#ifndef MQTT_ENQUEUE_MAC
#define MQTT_ENQUEUE_MAC            1
#endif

#ifndef MQTT_ENQUEUE_HOSTNAME
#define MQTT_ENQUEUE_HOSTNAME       1
#endif

#ifndef MQTT_ENQUEUE_DATETIME
#define MQTT_ENQUEUE_DATETIME       1
#endif

#ifndef MQTT_ENQUEUE_MESSAGE_ID
#define MQTT_ENQUEUE_MESSAGE_ID     1
#endif

// These particles will be concatenated to the MQTT_TOPIC base to form the actual topic
#define MQTT_TOPIC_JSON             "data"
#define MQTT_TOPIC_ACTION           "action"
#define MQTT_TOPIC_RELAY            "relay"
#define MQTT_TOPIC_LED              "led"
#define MQTT_TOPIC_BUTTON           "button"
#define MQTT_TOPIC_IP               "ip"
#define MQTT_TOPIC_SSID             "ssid"
#define MQTT_TOPIC_BSSID            "bssid"
#define MQTT_TOPIC_VERSION          "version"
#define MQTT_TOPIC_UPTIME           "uptime"
#define MQTT_TOPIC_DATETIME         "datetime"
#define MQTT_TOPIC_TIMESTAMP        "timestamp"
#define MQTT_TOPIC_FREEHEAP         "freeheap"
#define MQTT_TOPIC_VCC              "vcc"
#ifndef MQTT_TOPIC_STATUS
#define MQTT_TOPIC_STATUS           "status"
#endif
#define MQTT_TOPIC_MAC              "mac"
#define MQTT_TOPIC_RSSI             "rssi"
#define MQTT_TOPIC_MESSAGE_ID       "id"
#define MQTT_TOPIC_APP              "app"
#define MQTT_TOPIC_INTERVAL         "interval"
#define MQTT_TOPIC_HOSTNAME         "host"
#define MQTT_TOPIC_DESCRIPTION      "desc"
#define MQTT_TOPIC_TIME             "time"
#define MQTT_TOPIC_RFOUT            "rfout"
#define MQTT_TOPIC_RFIN             "rfin"
#define MQTT_TOPIC_RFLEARN          "rflearn"
#define MQTT_TOPIC_RFRAW            "rfraw"
#define MQTT_TOPIC_UARTIN           "uartin"
#define MQTT_TOPIC_UARTOUT          "uartout"
#define MQTT_TOPIC_LOADAVG          "loadavg"
#define MQTT_TOPIC_BOARD            "board"
#define MQTT_TOPIC_PULSE            "pulse"
#define MQTT_TOPIC_SPEED            "speed"
#define MQTT_TOPIC_IRIN             "irin"
#define MQTT_TOPIC_IROUT            "irout"
#define MQTT_TOPIC_OTA              "ota"
#define MQTT_TOPIC_TELNET_REVERSE   "telnet_reverse"

// Light module
#define MQTT_TOPIC_CHANNEL          "channel"
#define MQTT_TOPIC_COLOR_RGB        "rgb"
#define MQTT_TOPIC_COLOR_HSV        "hsv"
#define MQTT_TOPIC_ANIM_MODE        "anim_mode"
#define MQTT_TOPIC_ANIM_SPEED       "anim_speed"
#define MQTT_TOPIC_BRIGHTNESS       "brightness"
#define MQTT_TOPIC_MIRED            "mired"
#define MQTT_TOPIC_KELVIN           "kelvin"
#define MQTT_TOPIC_TRANSITION       "transition"

// Thermostat module
#define MQTT_TOPIC_HOLD_TEMP        "hold_temp"
#define MQTT_TOPIC_HOLD_TEMP_MIN    "min"
#define MQTT_TOPIC_HOLD_TEMP_MAX    "max"
#define MQTT_TOPIC_REMOTE_TEMP      "remote_temp"
#define MQTT_TOPIC_ASK_TEMP_RANGE   "ask_temp_range"
#define MQTT_TOPIC_NOTIFY_TEMP_RANGE_MIN "notify_temp_range_min"
#define MQTT_TOPIC_NOTIFY_TEMP_RANGE_MAX "notify_temp_range_max"


#ifndef MQTT_STATUS_ONLINE
#define MQTT_STATUS_ONLINE          "1"         // Value for the device ON message
#endif

#ifndef MQTT_STATUS_OFFLINE
#define MQTT_STATUS_OFFLINE         "0"         // Value for the device OFF message (will)
#endif

#define MQTT_ACTION_RESET           "reboot"    // RESET MQTT topic particle

// Custom get and set postfixes
// Use something like "/status" or "/set", with leading slash
// Since 1.9.0 the default value is "" for getter and "/set" for setter
#ifndef MQTT_GETTER
#define MQTT_GETTER                 ""
#endif

#ifndef MQTT_SETTER
#define MQTT_SETTER                 "/set"
#endif

// Internal MQTT events
#define MQTT_CONNECT_EVENT          0
#define MQTT_DISCONNECT_EVENT       1
#define MQTT_MESSAGE_EVENT          2

// MQTT_LIBRARY
#define MQTT_LIBRARY_ASYNCMQTTCLIENT        0
#define MQTT_LIBRARY_ARDUINOMQTT            1
#define MQTT_LIBRARY_PUBSUBCLIENT           2

#include <Arduino.h>
#include <functional>

struct settings_cfg_t {
    String& setting;
    const char* key;
    const char* default_value;
};

using settings_filter_t = std::function<String(String& value)>;
using settings_cfg_list_t = std::initializer_list<settings_cfg_t>;

void settingsProcessConfig(const settings_cfg_list_t& config, settings_filter_t filter = nullptr);

bool mqttForward();
void mqttSetup();

using mqtt_callback_f = std::function<void(unsigned int type, const char * topic, char * payload)>;
using mqtt_msg_t = std::pair<String, String>; // topic, payload

void mqttRegister(mqtt_callback_f callback);

String mqttTopic(const char * magnitude, bool is_set);
String mqttTopic(const char * magnitude, unsigned int index, bool is_set);

String mqttMagnitude(char * topic);

bool mqttSendRaw(const char * topic, const char * message, bool retain);
bool mqttSendRaw(const char * topic, const char * message);

void mqttSend(const char * topic, const char * message, bool force, bool retain);
void mqttSend(const char * topic, const char * message, bool force);
void mqttSend(const char * topic, const char * message);

void mqttSend(const char * topic, unsigned int index, const char * message, bool force, bool retain);
void mqttSend(const char * topic, unsigned int index, const char * message, bool force);
void mqttSend(const char * topic, unsigned int index, const char * message);

const String& mqttPayloadOnline();
const String& mqttPayloadOffline();
const char* mqttPayloadStatus(bool status);

void mqttSendStatus();
void mqttSubscribeRaw(const char * topic);
void mqttSubscribe(const char * topic);
int8_t mqttEnqueue(const char * topic, const char * message);
int8_t mqttEnqueue(const char * topic, const char * message, unsigned char parent);

void mqttDisconnect();
void mqttFlush();

void mqttEnabled(bool status);
bool mqttEnabled();
bool mqttConnected();
void mqttDisconnect();;

void mqttUnsubscribeRaw(const char * topic);
void mqttUnsubscribe(const char * topic);

void mqttSetBrokerIfNone(IPAddress ip, unsigned int port);
void mqttLoop();

#endif /* INC_MQTTBROKER_HPP */