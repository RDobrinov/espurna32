// // //------------------------------------------------------------------------------
// Do not change this file unless you know what you are doing
// Configuration settings are in the settings.h file
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// GENERAL
//------------------------------------------------------------------------------

#define DEVICE_NAME             MANUFACTURER "_" DEVICE     // Concatenate both to get a unique device name

// When defined, ADMIN_PASS must be 8..63 printable ASCII characters. See:
// https://en.wikipedia.org/wiki/Wi-Fi_Protected_Access#Target_users_(authentication_key_distribution)
// https://github.com/xoseperez/espurna/issues/1151
#ifndef ADMIN_PASS
#define ADMIN_PASS              "098765Dc"     // Default password (WEB, OTA, WIFI SoftAP)
#endif

#ifndef USE_PASSWORD
#define USE_PASSWORD            1               // Insecurity caution! Disabling this will disable password querying completely.
#endif

#ifndef LOOP_DELAY_TIME
#define LOOP_DELAY_TIME         10              // Delay for the main loop, in millis [0-250]
                                                // Recommended minimum is 10, see:
                                                // https://github.com/xoseperez/espurna/issues/1541
                                                // https://github.com/xoseperez/espurna/issues/1631
                                                // https://github.com/esp8266/Arduino/issues/5825
#endif

//------------------------------------------------------------------------------
// DEBUG
//------------------------------------------------------------------------------

// Serial debug log


//------------------------------------------------------------------------------
// TELNET
//------------------------------------------------------------------------------

#ifndef TELNET_SUPPORT
#define TELNET_SUPPORT          0               // Enable telnet support by default (3.34Kb)
#endif

#ifndef TELNET_STA
#define TELNET_STA              0               // By default, disallow connections via STA interface
#endif

#ifndef TELNET_AUTHENTICATION
#define TELNET_AUTHENTICATION   1               // Request password to start telnet session by default
#endif

#ifndef TELNET_PORT
#define TELNET_PORT             23              // Port to listen to telnet clients
#endif

#ifndef TELNET_MAX_CLIENTS
#define TELNET_MAX_CLIENTS      1               // Max number of concurrent telnet clients
#endif

#ifndef TELNET_SERVER
#define TELNET_SERVER           TELNET_SERVER_ASYNC // Can be either TELNET_SERVER_ASYNC (using ESPAsyncTCP) or TELNET_SERVER_WIFISERVER (using WiFiServer)
#endif

#ifndef TELNET_SERVER_ASYNC_BUFFERED
#define TELNET_SERVER_ASYNC_BUFFERED         0  // Enable buffered output for telnet server (+1Kb)
                                                // Helps to avoid lost data with lwip2 TCP_MSS=536 option
#endif

// Enable this flag to add support for reverse telnet (+800 bytes)
// This is useful to telnet to a device behind a NAT or firewall
// To use this feature, start a listen server on a publicly reachable host with e.g. "ncat -vlp <port>" and use the MQTT reverse telnet command to connect
#ifndef TELNET_REVERSE_SUPPORT
#define TELNET_REVERSE_SUPPORT  0
#endif

//------------------------------------------------------------------------------
// TERMINAL
//------------------------------------------------------------------------------

#ifndef TERMINAL_SUPPORT
#define TERMINAL_SUPPORT         1              // Enable terminal commands (0.97Kb)
#endif

#define TERMINAL_BUFFER_SIZE     128            // Max size for commands commands

//------------------------------------------------------------------------------
// EEPROM
//------------------------------------------------------------------------------

#define EEPROM_SIZE             SPI_FLASH_SEC_SIZE  // EEPROM size in bytes (1 sector = 4096 bytes)

//#define EEPROM_RORATE_SECTORS   2             // Number of sectors to use for EEPROM rotation
                                                // If not defined the firmware will use a number based
                                                // on the number of available sectors

#define EEPROM_RELAY_STATUS     0               // Address for the relay status (1 byte)
#define EEPROM_ENERGY_COUNT     1               // Address for the energy counter (4 bytes)
#define EEPROM_CUSTOM_RESET     5               // Address for the reset reason (1 byte)
#define EEPROM_CRASH_COUNTER    6               // Address for the crash counter (1 byte)
#define EEPROM_MESSAGE_ID       7               // Address for the MQTT message id (4 bytes)
#define EEPROM_ROTATE_DATA      11              // Reserved for the EEPROM_ROTATE library (3 bytes)
#define EEPROM_DATA_END         14              // End of custom EEPROM data block

//------------------------------------------------------------------------------
// THERMOSTAT
//------------------------------------------------------------------------------

#ifndef THERMOSTAT_SUPPORT
#define THERMOSTAT_SUPPORT          0
#endif

#ifndef THERMOSTAT_DISPLAY_SUPPORT
#define THERMOSTAT_DISPLAY_SUPPORT  0
#endif

#define THERMOSTAT_SERVER_LOST_INTERVAL  120000 //server means lost after 2 min from last response
#define THERMOSTAT_REMOTE_TEMP_MAX_WAIT     120 // 2 min

//------------------------------------------------------------------------------
// BUTTON
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// ENCODER
//------------------------------------------------------------------------------

#ifndef ENCODER_SUPPORT
#define ENCODER_SUPPORT             0
#endif

#ifndef ENCODER_MINIMUM_DELTA
#define ENCODER_MINIMUM_DELTA       1
#endif

//------------------------------------------------------------------------------
// LED
//------------------------------------------------------------------------------

#ifndef LED_SUPPORT
#define LED_SUPPORT                 1
#endif



// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// WEBSOCKETS
// -----------------------------------------------------------------------------

// This will only be enabled if WEB_SUPPORT is 1 (this is the default value)





// -----------------------------------------------------------------------------
// MDNS / LLMNR / NETBIOS / SSDP
// -----------------------------------------------------------------------------

#ifndef MDNS_SERVER_SUPPORT
#define MDNS_SERVER_SUPPORT         1           // Publish services using mDNS by default (1.48Kb)
#endif

#ifndef MDNS_CLIENT_SUPPORT
#define MDNS_CLIENT_SUPPORT         0           // Resolve mDNS names (3.44Kb)
#endif

#ifndef LLMNR_SUPPORT
#define LLMNR_SUPPORT               0           // Publish device using LLMNR protocol by default (1.95Kb) - requires Core version >= 2.4.0
#endif

#ifndef NETBIOS_SUPPORT
#define NETBIOS_SUPPORT             0           // Publish device using NetBIOS protocol by default (1.26Kb) - requires Core version >= 2.4.0
#endif

#ifndef SSDP_SUPPORT
#define SSDP_SUPPORT                0           // Publish device using SSDP protocol by default (4.59Kb)
                                                // Not compatible with ALEXA_SUPPORT at the moment
#endif

#ifndef SSDP_DEVICE_TYPE
#define SSDP_DEVICE_TYPE            "upnp:rootdevice"
//#define SSDP_DEVICE_TYPE            "urn:schemas-upnp-org:device:BinaryLight:1"
#endif


// -----------------------------------------------------------------------------
// SPIFFS
// -----------------------------------------------------------------------------

#ifndef SPIFFS_SUPPORT
#define SPIFFS_SUPPORT              0           // Do not add support for SPIFFS by default
#endif

// -----------------------------------------------------------------------------
// SSL Client                                                 ** EXPERIMENTAL **
// -----------------------------------------------------------------------------

#ifndef SECURE_CLIENT
#define SECURE_CLIENT                          SECURE_CLIENT_NONE     // What variant of WiFiClient to use
                                                                      // SECURE_CLIENT_NONE    - No secure client support (default)
                                                                      // SECURE_CLIENT_AXTLS   - axTLS client secure support (All Core versions, ONLY TLS 1.1)
                                                                      // SECURE_CLIENT_BEARSSL - BearSSL client secure support (starting with 2.5.0, TLS 1.2)
                                                                      //
                                                                      // axTLS marked for derecation since Arduino Core 2.4.2 and **will** be removed in the future
#endif

// Security check that is performed when the connection is established:
// SECURE_CLIENT_CHECK_CA           - Use Trust Anchor / Root Certificate
//                                    Supported only by the SECURE_CLIENT_BEARSSL
//                                    (See respective ..._SECURE_CLIENT_INCLUDE_CA options per-module)
// SECURE_CLIENT_CHECK_FINGERPRINT  - Check certificate fingerprint
// SECURE_CLIENT_CHECK_NONE         - Allow insecure connections

#ifndef SECURE_CLIENT_CHECK

#if SECURE_CLIENT == SECURE_CLIENT_BEARSSL
#define SECURE_CLIENT_CHECK                    SECURE_CLIENT_CHECK_CA

#else
#define SECURE_CLIENT_CHECK                    SECURE_CLIENT_CHECK_FINGERPRINT

#endif


#endif // SECURE_CLIENT_CHECK

// Support Maximum Fragment Length Negotiation TLS extension
// "...negotiate a smaller maximum fragment length due to memory limitations or bandwidth limitations."
// - https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/bearssl-client-secure-class.html#mfln-or-maximum-fragment-length-negotiation-saving-ram
// - https://tools.ietf.org/html/rfc6066#section-4
#ifndef SECURE_CLIENT_MFLN
#define SECURE_CLIENT_MFLN                     0                      // The only possible values are: 512, 1024, 2048 and 4096
                                                                      // Set to 0 to disable (default)
#endif

// -----------------------------------------------------------------------------
// OTA
// -----------------------------------------------------------------------------

#ifndef OTA_MQTT_SUPPORT
#define OTA_MQTT_SUPPORT            0           // Listen for HTTP(s) URLs at '<root topic>/ota'. Depends on OTA_CLIENT
#endif



#ifndef OTA_CLIENT
#define OTA_CLIENT                  OTA_CLIENT_ASYNCTCP     // Terminal / MQTT OTA support
                                                            // OTA_CLIENT_ASYNCTCP   (ESPAsyncTCP library)
                                                            // OTA_CLIENT_HTTPUPDATE (Arduino Core library)
#endif

#define OTA_GITHUB_FP               "CA:06:F5:6B:25:8B:7A:0D:4F:2B:05:47:09:39:47:86:51:15:19:84"

#ifndef OTA_FINGERPRINT
#define OTA_FINGERPRINT             OTA_GITHUB_FP
#endif

#ifndef OTA_SECURE_CLIENT_CHECK
#define OTA_SECURE_CLIENT_CHECK                SECURE_CLIENT_CHECK
#endif

#ifndef OTA_SECURE_CLIENT_MFLN
#define OTA_SECURE_CLIENT_MFLN                 SECURE_CLIENT_MFLN
#endif

#ifndef OTA_SECURE_CLIENT_INCLUDE_CA
#define OTA_SECURE_CLIENT_INCLUDE_CA        0            // Use user-provided CA. Only PROGMEM PEM option is supported.
                                                         // TODO: eventually should be replaced with pre-parsed structs, read directly from flash
                                                         // (ref: https://github.com/earlephilhower/bearssl-esp8266/pull/14)
                                                         //
                                                         // When enabled, current implementation includes "static/ota_client_trusted_root_ca.h" with
                                                         // const char _ota_client_trusted_root_ca[] PROGMEM = "...PEM data...";
                                                         // By default, using DigiCert root in "static/digicert_evroot_pem.h" (for https://github.com)
#endif


// -----------------------------------------------------------------------------
// NOFUSS
// -----------------------------------------------------------------------------

#ifndef NOFUSS_SUPPORT
#define NOFUSS_SUPPORT              0          // Do not enable support for NoFuss by default (12.65Kb)
#endif

#ifndef NOFUSS_ENABLED
#define NOFUSS_ENABLED              0           // Do not perform NoFUSS updates by default
#endif

#ifndef NOFUSS_SERVER
#define NOFUSS_SERVER               ""          // Default NoFuss Server
#endif

#ifndef NOFUSS_INTERVAL
#define NOFUSS_INTERVAL             3600000     // Check for updates every hour
#endif

// -----------------------------------------------------------------------------
// UART <-> MQTT
// -----------------------------------------------------------------------------

#ifndef UART_MQTT_SUPPORT
#define UART_MQTT_SUPPORT           0           // No support by default
#endif

#ifndef UART_MQTT_USE_SOFT
#define UART_MQTT_USE_SOFT          0           // Use SoftwareSerial
#endif

#ifndef UART_MQTT_HW_PORT
#define UART_MQTT_HW_PORT           Serial      // Hardware serial port (if UART_MQTT_USE_SOFT == 0)
#endif

#ifndef UART_MQTT_RX_PIN
#define UART_MQTT_RX_PIN            4           // RX PIN (if UART_MQTT_USE_SOFT == 1)
#endif

#ifndef UART_MQTT_TX_PIN
#define UART_MQTT_TX_PIN            5           // TX PIN (if UART_MQTT_USE_SOFT == 1)
#endif

#ifndef UART_MQTT_BAUDRATE
#define UART_MQTT_BAUDRATE          115200      // Serial speed
#endif

#ifndef UART_MQTT_TERMINATION
#define UART_MQTT_TERMINATION      '\n'         // Termination character
#endif

#define UART_MQTT_BUFFER_SIZE       100         // UART buffer size

// -----------------------------------------------------------------------------
// MQTT
// -----------------------------------------------------------------------------

#ifndef MQTT_SUPPORT
#define MQTT_SUPPORT                1           // MQTT support (22.38Kb async, 12.48Kb sync)
#endif


#ifndef MQTT_LIBRARY
#define MQTT_LIBRARY                MQTT_LIBRARY_ASYNCMQTTCLIENT       // MQTT_LIBRARY_ASYNCMQTTCLIENT (default, https://github.com/marvinroger/async-mqtt-client)
                                                                       // MQTT_LIBRARY_PUBSUBCLIENT (https://github.com/knolleary/pubsubclient)
                                                                       // MQTT_LIBRARY_ARDUINOMQTT (https://github.com/256dpi/arduino-mqtt)
#endif

// -----------------------------------------------------------------------------
// BROKER
// -----------------------------------------------------------------------------

#ifndef BROKER_SUPPORT
#define BROKER_SUPPORT          1           // The broker is a poor-man's pubsub manager
#endif

// -----------------------------------------------------------------------------
// SETTINGS
// -----------------------------------------------------------------------------

#ifndef SETTINGS_AUTOSAVE
#define SETTINGS_AUTOSAVE       1           // Autosave settings or force manual commit
#endif

// -----------------------------------------------------------------------------
// LIGHT
// -----------------------------------------------------------------------------

// LIGHT_PROVIDER_DIMMER can have from 1 to 5 different channels.
// They have to be defined for each device in the hardware.h file.
// If 3 or more channels first 3 will be considered RGB.
// Usual configurations are:
// 1 channels => W
// 2 channels => WW
// 3 channels => RGB
// 4 channels => RGBW
// 5 channels => RGBWW

#ifndef LIGHT_SAVE_ENABLED
#define LIGHT_SAVE_ENABLED      1           // Light channel values saved by default after each change
#endif

#ifndef LIGHT_COMMS_DELAY
#define LIGHT_COMMS_DELAY       100         // Delay communication after light update (in ms)
#endif

#ifndef LIGHT_SAVE_DELAY
#define LIGHT_SAVE_DELAY        5           // Persist color after 5 seconds to avoid wearing out
#endif

#ifndef LIGHT_MIN_PWM
#define LIGHT_MIN_PWM           0
#endif

#ifndef LIGHT_MAX_PWM

#if LIGHT_PROVIDER == LIGHT_PROVIDER_MY92XX
#define LIGHT_MAX_PWM           255
#elif LIGHT_PROVIDER == LIGHT_PROVIDER_DIMMER
#define LIGHT_MAX_PWM           10000        // 10000 * 200ns => 2 kHz
#else
#define LIGHT_MAX_PWM           0
#endif

#endif // LIGHT_MAX_PWM

#ifndef LIGHT_LIMIT_PWM
#define LIGHT_LIMIT_PWM         LIGHT_MAX_PWM   // Limit PWM to this value (prevent 100% power)
#endif

#ifndef LIGHT_MIN_VALUE
#define LIGHT_MIN_VALUE         0           // Minimum light value
#endif

#ifndef LIGHT_MAX_VALUE
#define LIGHT_MAX_VALUE         255         // Maximum light value
#endif

#ifndef LIGHT_MIN_BRIGHTNESS
#define LIGHT_MIN_BRIGHTNESS    0           // Minimum brightness value
#endif

#ifndef LIGHT_MAX_BRIGHTNESS
#define LIGHT_MAX_BRIGHTNESS    255         // Maximum brightness value
#endif

// Default mireds & kelvin to the Philips Hue limits
// https://developers.meethue.com/documentation/core-concepts
//
// Home Assistant also uses these, see Light::min_mireds, Light::max_mireds
// https://github.com/home-assistant/home-assistant/blob/dev/homeassistant/components/light/__init__.py

// Used when LIGHT_USE_WHITE AND LIGHT_USE_CCT is 1 - (1000000/Kelvin = MiReds)
// Warning! Don't change this yet, NOT FULLY IMPLEMENTED!
#ifndef LIGHT_COLDWHITE_MIRED
#define LIGHT_COLDWHITE_MIRED   153         // Coldwhite Strip, Value must be __BELOW__ W2!! (Default: 6535 Kelvin/153 MiRed)
#endif

#ifndef LIGHT_WARMWHITE_MIRED
#define LIGHT_WARMWHITE_MIRED   500         // Warmwhite Strip, Value must be __ABOVE__ W1!! (Default: 2000 Kelvin/500 MiRed)
#endif

#ifndef LIGHT_STEP
#define LIGHT_STEP              32          // Step size
#endif

#ifndef LIGHT_USE_COLOR
#define LIGHT_USE_COLOR         1           // Use 3 first channels as RGB
#endif

#ifndef LIGHT_USE_WHITE
#define LIGHT_USE_WHITE         0           // Use the 4th channel as (Warm-)White LEDs
#endif

#ifndef LIGHT_USE_CCT
#define LIGHT_USE_CCT           0           // Use the 5th channel as Coldwhite LEDs, LIGHT_USE_WHITE must be 1.
#endif

#ifndef LIGHT_USE_GAMMA
#define LIGHT_USE_GAMMA         0           // Use gamma correction for color channels
#endif

#ifndef LIGHT_USE_CSS
#define LIGHT_USE_CSS           1           // Use CSS style to report colors (1=> "#FF0000", 0=> "255,0,0")
#endif

#ifndef LIGHT_USE_RGB
#define LIGHT_USE_RGB           0           // Use RGB color selector (1=> RGB, 0=> HSV)
#endif

#ifndef LIGHT_WHITE_FACTOR
#define LIGHT_WHITE_FACTOR      1           // When using LIGHT_USE_WHITE with uneven brightness LEDs,
                                            // this factor is used to scale the white channel to match brightness
#endif


#ifndef LIGHT_USE_TRANSITIONS
#define LIGHT_USE_TRANSITIONS   1           // Transitions between colors
#endif

#ifndef LIGHT_TRANSITION_STEP
#define LIGHT_TRANSITION_STEP   10          // Time in millis between each transtion step
#endif

#ifndef LIGHT_TRANSITION_TIME
#define LIGHT_TRANSITION_TIME   500         // Time in millis from color to color
#endif


// -----------------------------------------------------------------------------
// DOMOTICZ
// -----------------------------------------------------------------------------

#ifndef DOMOTICZ_SUPPORT
#define DOMOTICZ_SUPPORT        MQTT_SUPPORT    // Build with domoticz (if MQTT) support (1.72Kb)
#endif

#ifndef DOMOTICZ_ENABLED
#define DOMOTICZ_ENABLED        0               // Disable domoticz by default
#endif

#ifndef DOMOTICZ_IN_TOPIC
#define DOMOTICZ_IN_TOPIC       "domoticz/in"   // Default subscription topic
#endif

#ifndef DOMOTICZ_OUT_TOPIC
#define DOMOTICZ_OUT_TOPIC      "domoticz/out"  // Default publication topic
#endif




// -----------------------------------------------------------------------------
// THINGSPEAK
// -----------------------------------------------------------------------------

#ifndef THINGSPEAK_SUPPORT
#define THINGSPEAK_SUPPORT          1               // Enable Thingspeak support by default (2.56Kb)
#endif

#ifndef THINGSPEAK_ENABLED
#define THINGSPEAK_ENABLED          0               // Thingspeak disabled by default
#endif

#ifndef THINGSPEAK_APIKEY
#define THINGSPEAK_APIKEY           ""              // Default API KEY
#endif

#ifndef THINGSPEAK_CLEAR_CACHE
#define THINGSPEAK_CLEAR_CACHE      1               // Clear cache after sending values
                                                    // Not clearing it will result in latest values for each field being sent every time
#endif

#define THINGSPEAK_USE_ASYNC        1               // Use AsyncClient instead of WiFiClientSecure

// THINGSPEAK OVER SSL
// Using THINGSPEAK over SSL works well but generates problems with the web interface,
// so you should compile it with WEB_SUPPORT to 0.
// When THINGSPEAK_USE_ASYNC is 1, requires EspAsyncTCP to be built with ASYNC_TCP_SSL_ENABLED=1 and ESP8266 Arduino Core >= 2.4.0.
#define THINGSPEAK_USE_SSL          0               // Use secure connection

#define THINGSPEAK_FINGERPRINT      "78 60 18 44 81 35 BF DF 77 84 D4 0A 22 0D 9B 4E 6C DC 57 2C"

#define THINGSPEAK_HOST             "api.thingspeak.com"
#if THINGSPEAK_USE_SSL
#define THINGSPEAK_PORT             443
#else
#define THINGSPEAK_PORT             80
#endif

#define THINGSPEAK_URL              "/update"

#define THINGSPEAK_MIN_INTERVAL     15000           // Minimum interval between POSTs (in millis)
#define THINGSPEAK_FIELDS           8               // Number of fields

#ifndef THINGSPEAK_TRIES
#define THINGSPEAK_TRIES            3               // Number of tries when sending data (minimum 1)
#endif


// -----------------------------------------------------------------------------
// ALEXA
// -----------------------------------------------------------------------------

// This setting defines whether Alexa support should be built into the firmware
#ifndef ALEXA_SUPPORT
#define ALEXA_SUPPORT               1               // Enable Alexa support by default (10.84Kb)
#endif

// This is default value for the alexaEnabled setting that defines whether
// this device should be discoberable and respond to Alexa commands.
// Both ALEXA_SUPPORT and alexaEnabled should be 1 for Alexa support to work.
#ifndef ALEXA_ENABLED
#define ALEXA_ENABLED               1
#endif

#ifndef ALEXA_HOSTNAME
#define ALEXA_HOSTNAME              ""
#endif


// -----------------------------------------------------------------------------
// MQTT RF BRIDGE
// -----------------------------------------------------------------------------

#ifndef RF_SUPPORT
#define RF_SUPPORT                  0
#endif

#ifndef RF_DEBOUNCE
#define RF_DEBOUNCE                 500
#endif

#ifndef RF_LEARN_TIMEOUT
#define RF_LEARN_TIMEOUT            60000
#endif

#ifndef RF_SEND_TIMES
#define RF_SEND_TIMES               4               // How many times to send the message
#endif

#ifndef RF_SEND_DELAY
#define RF_SEND_DELAY               500             // Interval between sendings in ms
#endif

#ifndef RF_RECEIVE_DELAY
#define RF_RECEIVE_DELAY            500             // Interval between recieving in ms (avoid debouncing)
#endif

// Enable RCSwitch support
// Originally implemented for SONOFF BASIC
// https://tinkerman.cat/adding-rf-to-a-non-rf-itead-sonoff/
// Also possible to use with SONOFF RF BRIDGE, thanks to @wildwiz
// https://github.com/xoseperez/espurna/wiki/Hardware-Itead-Sonoff-RF-Bridge---Direct-Hack
#ifndef RFB_DIRECT
#define RFB_DIRECT                  0
#endif

#ifndef RFB_RX_PIN
#define RFB_RX_PIN                  GPIO_NONE
#endif

#ifndef RFB_TX_PIN
#define RFB_TX_PIN                  GPIO_NONE
#endif


// -----------------------------------------------------------------------------
// IR Bridge
// -----------------------------------------------------------------------------

#ifndef IR_SUPPORT
#define IR_SUPPORT                  0               // Do not build with IR support by default (10.25Kb)
#endif

//#define IR_RX_PIN                   5               // GPIO the receiver is connected to
//#define IR_TX_PIN                   4               // GPIO the transmitter is connected to

#ifndef IR_USE_RAW
#define IR_USE_RAW                  0               // Use raw codes
#endif

#ifndef IR_BUFFER_SIZE
#define IR_BUFFER_SIZE              1024
#endif

#ifndef IR_TIMEOUT
#define IR_TIMEOUT                  15U
#endif

#ifndef IR_REPEAT
#define IR_REPEAT                   1
#endif

#ifndef IR_DELAY
#define IR_DELAY                    100
#endif

#ifndef IR_DEBOUNCE
#define IR_DEBOUNCE                 500             // IR debounce time in milliseconds
#endif

#ifndef IR_BUTTON_SET
#define IR_BUTTON_SET               0               // IR button set to use (see below)
#endif

// -----------------------------------------------------------------------------

// Remote Buttons SET 1 (for the original Remote shipped with the controller)
#if IR_BUTTON_SET == 1

/*
   +------+------+------+------+
   |  UP  | Down | OFF  |  ON  |
   +------+------+------+------+
   |  R   |  G   |  B   |  W   |
   +------+------+------+------+
   |  1   |  2   |  3   |FLASH |
   +------+------+------+------+
   |  4   |  5   |  6   |STROBE|
   +------+------+------+------+
   |  7   |  8   |  9   | FADE |
   +------+------+------+------+
   |  10  |  11  |  12  |SMOOTH|
   +------+------+------+------+
*/

    #define IR_BUTTON_COUNT 24

    const uint32_t IR_BUTTON[IR_BUTTON_COUNT][3] PROGMEM = {

        { 0xFF906F, IR_BUTTON_MODE_BRIGHTER, 1 },
        { 0xFFB847, IR_BUTTON_MODE_BRIGHTER, 0 },
        { 0xFFF807, IR_BUTTON_MODE_STATE, 0 },
        { 0xFFB04F, IR_BUTTON_MODE_STATE, 1 },

        { 0xFF9867, IR_BUTTON_MODE_RGB, 0xFF0000 },
        { 0xFFD827, IR_BUTTON_MODE_RGB, 0x00FF00 },
        { 0xFF8877, IR_BUTTON_MODE_RGB, 0x0000FF },
        { 0xFFA857, IR_BUTTON_MODE_RGB, 0xFFFFFF },

        { 0xFFE817, IR_BUTTON_MODE_RGB, 0xD13A01 },
        { 0xFF48B7, IR_BUTTON_MODE_RGB, 0x00E644 },
        { 0xFF6897, IR_BUTTON_MODE_RGB, 0x0040A7 },
        { 0xFFB24D, IR_BUTTON_MODE_EFFECT, LIGHT_EFFECT_FLASH },

        { 0xFF02FD, IR_BUTTON_MODE_RGB, 0xE96F2A },
        { 0xFF32CD, IR_BUTTON_MODE_RGB, 0x00BEBF },
        { 0xFF20DF, IR_BUTTON_MODE_RGB, 0x56406F },
        { 0xFF00FF, IR_BUTTON_MODE_EFFECT, LIGHT_EFFECT_STROBE },

        { 0xFF50AF, IR_BUTTON_MODE_RGB, 0xEE9819 },
        { 0xFF7887, IR_BUTTON_MODE_RGB, 0x00799A },
        { 0xFF708F, IR_BUTTON_MODE_RGB, 0x944E80 },
        { 0xFF58A7, IR_BUTTON_MODE_EFFECT, LIGHT_EFFECT_FADE },

        { 0xFF38C7, IR_BUTTON_MODE_RGB, 0xFFFF00 },
        { 0xFF28D7, IR_BUTTON_MODE_RGB, 0x0060A1 },
        { 0xFFF00F, IR_BUTTON_MODE_RGB, 0xEF45AD },
        { 0xFF30CF, IR_BUTTON_MODE_EFFECT, LIGHT_EFFECT_SMOOTH }

    };

#endif

//Remote Buttons SET 2 (another identical IR Remote shipped with another controller)
#if IR_BUTTON_SET == 2

/*
   +------+------+------+------+
   |  UP  | Down | OFF  |  ON  |
   +------+------+------+------+
   |  R   |  G   |  B   |  W   |
   +------+------+------+------+
   |  1   |  2   |  3   |FLASH |
   +------+------+------+------+
   |  4   |  5   |  6   |STROBE|
   +------+------+------+------+
   |  7   |  8   |  9   | FADE |
   +------+------+------+------+
   |  10  |  11  |  12  |SMOOTH|
   +------+------+------+------+
*/

    #define IR_BUTTON_COUNT 24

    const unsigned long IR_BUTTON[IR_BUTTON_COUNT][3] PROGMEM = {

        { 0xFF00FF, IR_BUTTON_MODE_BRIGHTER, 1 },
        { 0xFF807F, IR_BUTTON_MODE_BRIGHTER, 0 },
        { 0xFF40BF, IR_BUTTON_MODE_STATE, 0 },
        { 0xFFC03F, IR_BUTTON_MODE_STATE, 1 },

        { 0xFF20DF, IR_BUTTON_MODE_RGB, 0xFF0000 },
        { 0xFFA05F, IR_BUTTON_MODE_RGB, 0x00FF00 },
        { 0xFF609F, IR_BUTTON_MODE_RGB, 0x0000FF },
        { 0xFFE01F, IR_BUTTON_MODE_RGB, 0xFFFFFF },

        { 0xFF10EF, IR_BUTTON_MODE_RGB, 0xD13A01 },
        { 0xFF906F, IR_BUTTON_MODE_RGB, 0x00E644 },
        { 0xFF50AF, IR_BUTTON_MODE_RGB, 0x0040A7 },
        { 0xFFD02F, IR_BUTTON_MODE_EFFECT, LIGHT_EFFECT_FLASH },

        { 0xFF30CF, IR_BUTTON_MODE_RGB, 0xE96F2A },
        { 0xFFB04F, IR_BUTTON_MODE_RGB, 0x00BEBF },
        { 0xFF708F, IR_BUTTON_MODE_RGB, 0x56406F },
        { 0xFFF00F, IR_BUTTON_MODE_EFFECT, LIGHT_EFFECT_STROBE },

        { 0xFF08F7, IR_BUTTON_MODE_RGB, 0xEE9819 },
        { 0xFF8877, IR_BUTTON_MODE_RGB, 0x00799A },
        { 0xFF48B7, IR_BUTTON_MODE_RGB, 0x944E80 },
        { 0xFFC837, IR_BUTTON_MODE_EFFECT, LIGHT_EFFECT_FADE },

        { 0xFF28D7, IR_BUTTON_MODE_RGB, 0xFFFF00 },
        { 0xFFA857, IR_BUTTON_MODE_RGB, 0x0060A1 },
        { 0xFF6897, IR_BUTTON_MODE_RGB, 0xEF45AD },
        { 0xFFE817, IR_BUTTON_MODE_EFFECT, LIGHT_EFFECT_SMOOTH }

    };

#endif

//Remote Buttons SET 3 (samsung AA59-00608A 8 Toggle Buttons for generic 8CH module)
#if IR_BUTTON_SET == 3
/*
   +------+------+------+
   |  1   |  2   |  3   |
   +------+------+------+
   |  4   |  5   |  6   |
   +------+------+------+
   |  7   |  8   |  9   |
   +------+------+------+
   |      |  0   |      |
   +------+------+------+
*/
#define IR_BUTTON_COUNT 10

 const unsigned long IR_BUTTON[IR_BUTTON_COUNT][3] PROGMEM = {

        { 0xE0E020DF, IR_BUTTON_MODE_TOGGLE, 0 }, // Toggle Relay #0
        { 0xE0E0A05F, IR_BUTTON_MODE_TOGGLE, 1 }, // Toggle Relay #1
        { 0xE0E0609F, IR_BUTTON_MODE_TOGGLE, 2 }, // Toggle Relay #2

        { 0xE0E010EF, IR_BUTTON_MODE_TOGGLE, 3 }, // Toggle Relay #3
        { 0xE0E0906F, IR_BUTTON_MODE_TOGGLE, 4 }, // Toggle Relay #4
        { 0xE0E050AF, IR_BUTTON_MODE_TOGGLE, 5 }, // Toggle Relay #5

        { 0xE0E030CF, IR_BUTTON_MODE_TOGGLE, 6 }, // Toggle Relay #6
        { 0xE0E0B04F, IR_BUTTON_MODE_TOGGLE, 7 } // Toggle Relay #7
      //{ 0xE0E0708F, IR_BUTTON_MODE_TOGGLE, 8 } //Extra Button

      //{ 0xE0E08877, IR_BUTTON_MODE_TOGGLE, 9 } //Extra Button
 };
#endif

//Remote Buttons SET 4
#if IR_BUTTON_SET == 4
/*
   +------+------+------+
   | OFF  | SRC  | MUTE |
   +------+------+------+
   ...
   +------+------+------+
*/
#define IR_BUTTON_COUNT 1

 const unsigned long IR_BUTTON[IR_BUTTON_COUNT][3] PROGMEM = {

        { 0xFFB24D, IR_BUTTON_MODE_TOGGLE, 0 } // Toggle Relay #0

 };

#endif

//Remote Buttons SET 5 (another identical IR Remote shipped with another controller as SET 1 and 2)
#if IR_BUTTON_SET == 5

/*
   +------+------+------+------+
   |  UP  | Down | OFF  |  ON  |
   +------+------+------+------+
   |  R   |  G   |  B   |  W   |
   +------+------+------+------+
   |  1   |  2   |  3   |FLASH |
   +------+------+------+------+
   |  4   |  5   |  6   |STROBE|
   +------+------+------+------+
   |  7   |  8   |  9   | FADE |
   +------+------+------+------+
   |  10  |  11  |  12  |SMOOTH|
   +------+------+------+------+
*/

    #define IR_BUTTON_COUNT 24

    const unsigned long IR_BUTTON[IR_BUTTON_COUNT][3] PROGMEM = {

        { 0xF700FF, IR_BUTTON_MODE_BRIGHTER, 1 },
        { 0xF7807F, IR_BUTTON_MODE_BRIGHTER, 0 },
        { 0xF740BF, IR_BUTTON_MODE_STATE, 0 },
        { 0xF7C03F, IR_BUTTON_MODE_STATE, 1 },

        { 0xF720DF, IR_BUTTON_MODE_RGB, 0xFF0000 },
        { 0xF7A05F, IR_BUTTON_MODE_RGB, 0x00FF00 },
        { 0xF7609F, IR_BUTTON_MODE_RGB, 0x0000FF },
        { 0xF7E01F, IR_BUTTON_MODE_RGB, 0xFFFFFF },

        { 0xF710EF, IR_BUTTON_MODE_RGB, 0xD13A01 },
        { 0xF7906F, IR_BUTTON_MODE_RGB, 0x00E644 },
        { 0xF750AF, IR_BUTTON_MODE_RGB, 0x0040A7 },
        { 0xF7D02F, IR_BUTTON_MODE_EFFECT, LIGHT_EFFECT_FLASH },

        { 0xF730CF, IR_BUTTON_MODE_RGB, 0xE96F2A },
        { 0xF7B04F, IR_BUTTON_MODE_RGB, 0x00BEBF },
        { 0xF7708F, IR_BUTTON_MODE_RGB, 0x56406F },
        { 0xF7F00F, IR_BUTTON_MODE_EFFECT, LIGHT_EFFECT_STROBE },

        { 0xF708F7, IR_BUTTON_MODE_RGB, 0xEE9819 },
        { 0xF78877, IR_BUTTON_MODE_RGB, 0x00799A },
        { 0xF748B7, IR_BUTTON_MODE_RGB, 0x944E80 },
        { 0xF7C837, IR_BUTTON_MODE_EFFECT, LIGHT_EFFECT_FADE },

        { 0xF728D7, IR_BUTTON_MODE_RGB, 0xFFFF00 },
        { 0xF7A857, IR_BUTTON_MODE_RGB, 0x0060A1 },
        { 0xF76897, IR_BUTTON_MODE_RGB, 0xEF45AD },
        { 0xF7E817, IR_BUTTON_MODE_EFFECT, LIGHT_EFFECT_SMOOTH }

    };

#endif

#ifndef IR_BUTTON_COUNT
#define IR_BUTTON_COUNT 0
#endif

//--------------------------------------------------------------------------------
// Custom RFM69 to MQTT bridge
// Check http://tinkerman.cat/rfm69-wifi-gateway/
// Enable support by passing RFM69_SUPPORT=1 build flag
//--------------------------------------------------------------------------------

#ifndef RFM69_SUPPORT
#define RFM69_SUPPORT               0
#endif

#ifndef RFM69_MAX_TOPICS
#define RFM69_MAX_TOPICS            50
#endif

#ifndef RFM69_MAX_NODES
#define RFM69_MAX_NODES             255
#endif

#ifndef RFM69_DEFAULT_TOPIC
#define RFM69_DEFAULT_TOPIC         "/rfm69gw/{node}/{key}"
#endif

#ifndef RFM69_NODE_ID
#define RFM69_NODE_ID               1
#endif

#ifndef RFM69_GATEWAY_ID
#define RFM69_GATEWAY_ID            1
#endif

#ifndef RFM69_NETWORK_ID
#define RFM69_NETWORK_ID            164
#endif

#ifndef RFM69_PROMISCUOUS
#define RFM69_PROMISCUOUS           0
#endif

#ifndef RFM69_PROMISCUOUS_SENDS
#define RFM69_PROMISCUOUS_SENDS     0
#endif

#ifndef RFM69_FREQUENCY
#define RFM69_FREQUENCY             RF69_868MHZ
#endif

#ifndef RFM69_ENCRYPTKEY
#define RFM69_ENCRYPTKEY            "fibonacci0123456"
#endif

#ifndef RFM69_CS_PIN
#define RFM69_CS_PIN                SS
#endif

#ifndef RFM69_IRQ_PIN
#define RFM69_IRQ_PIN               5
#endif

#ifndef RFM69_RESET_PIN
#define RFM69_RESET_PIN             7
#endif

#ifndef RFM69_IS_RFM69HW
#define RFM69_IS_RFM69HW            0
#endif

//--------------------------------------------------------------------------------
// TUYA switch & dimmer support
//--------------------------------------------------------------------------------
#ifndef TUYA_SUPPORT
#define TUYA_SUPPORT                0
#endif

#ifndef TUYA_SERIAL
#define TUYA_SERIAL                 Serial
#endif
