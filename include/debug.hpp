#ifndef INC_DEBUG_HPP
#define INC_DEBUG_HPP
// -----------------------------------------------------------------------------
// Debug
// -----------------------------------------------------------------------------
#include <Arduino.h>
#include <ArduinoJson.h>

#ifndef DEBUG_SERIAL_SUPPORT
#define DEBUG_SERIAL_SUPPORT    1               // Enable serial debug log
#endif

#ifndef DEBUG_PORT
#define DEBUG_PORT              Serial          // Default debugging port
#endif

#ifndef SERIAL_BAUDRATE
#define SERIAL_BAUDRATE         115200          // Default baudrate
#endif

#ifndef DEBUG_ADD_TIMESTAMP
#define DEBUG_ADD_TIMESTAMP     1               // Add timestamp to debug messages
                                                // (in millis overflowing every 1000 seconds)
#endif

// Second serial port (used for RX)

#ifndef SERIAL_RX_ENABLED
#define SERIAL_RX_ENABLED       0               // Secondary serial port for RX
#endif

#ifndef SERIAL_RX_PORT
#define SERIAL_RX_PORT          Serial          // This setting is usually defined
                                                // in the hardware.h file for those
                                                // boards that require it
#endif

#ifndef SERIAL_RX_BAUDRATE
#define SERIAL_RX_BAUDRATE      115200          // Default baudrate
#endif

//------------------------------------------------------------------------------

// UDP debug log
// To receive the message son the destination computer use nc:
// nc -ul 8113

#ifndef DEBUG_UDP_SUPPORT
#define DEBUG_UDP_SUPPORT       0               // Enable UDP debug log
#endif

#ifndef DEBUG_UDP_IP
#define DEBUG_UDP_IP            IPAddress(192, 168, 1, 100)
#endif

#ifndef DEBUG_UDP_PORT
#define DEBUG_UDP_PORT          514
#endif

// If DEBUG_UDP_PORT is set to 514 syslog format is assumed
// (https://tools.ietf.org/html/rfc3164)
// DEBUG_UDP_FAC_PRI is the facility+priority
#define DEBUG_UDP_FAC_PRI       (SYSLOG_LOCAL0 | SYSLOG_DEBUG)

//------------------------------------------------------------------------------

#ifndef DEBUG_TELNET_SUPPORT
#define DEBUG_TELNET_SUPPORT    0               // Enable telnet debug log (will only work if TELNET_SUPPORT is also 1)
#endif

//------------------------------------------------------------------------------

#ifndef DEBUG_WEB_SUPPORT
#define DEBUG_WEB_SUPPORT       0               // Enable web debug log (will only work if WEB_SUPPORT is also 1)
#endif


#define DEBUG_SUPPORT           DEBUG_SERIAL_SUPPORT || DEBUG_UDP_SUPPORT || DEBUG_TELNET_SUPPORT || DEBUG_WEB_SUPPORT

#if DEBUG_SUPPORT
    #define DEBUG_MSG(...) debugSend(__VA_ARGS__)
    #define DEBUG_MSG_P(...) debugSend_P(__VA_ARGS__)
#endif

#ifndef DEBUG_MSG
    #define DEBUG_MSG(...)
    #define DEBUG_MSG_P(...)
#endif

void _debugSend(const char * format, va_list args);
void debugSend(const char* format, ...);
void debugSend_P(PGM_P format_P, ...);

void debugSetup();
void debugSendImpl(const char * message);

#if DEBUG_SERIAL_SUPPORT
    void _debugSendSerial(const char* prefix, const char* data);
#endif

#if DEBUG_TELNET_SUPPORT
    void _debugSendTelnet(const char* prefix, const char* data);
#endif

#if DEBUG_WEB_SUPPORT
    void _debugWebSocketOnAction(uint32_t client_id, const char * action, JsonObject& data);
    void debugWebSetup();
#endif

#endif /* INC_DEBUG_HPP */