#include <bitset>
#include "config/hardware.h"
#include "helperfn.hpp"

// -----------------------------------------------------------------------------
// Relays
// -----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// RELAY
//------------------------------------------------------------------------------

#define RELAY_BOOT_OFF              0
#define RELAY_BOOT_ON               1
#define RELAY_BOOT_SAME             2
#define RELAY_BOOT_TOGGLE           3
#define RELAY_BOOT_LOCKED_OFF       4
#define RELAY_BOOT_LOCKED_ON        5

#define RELAY_TYPE_NORMAL           0
#define RELAY_TYPE_INVERSE          1
#define RELAY_TYPE_LATCHED          2
#define RELAY_TYPE_LATCHED_INVERSE  3

#define RELAY_SYNC_ANY              0
#define RELAY_SYNC_NONE_OR_ONE      1
#define RELAY_SYNC_ONE              2
#define RELAY_SYNC_SAME             3
#define RELAY_SYNC_FIRST            4

#define RELAY_PULSE_NONE            0
#define RELAY_PULSE_OFF             1
#define RELAY_PULSE_ON              2

#define RELAY_PROVIDER_RELAY        0
#define RELAY_PROVIDER_DUAL         1
#define RELAY_PROVIDER_LIGHT        2
#define RELAY_PROVIDER_RFBRIDGE     3
#define RELAY_PROVIDER_STM          4

#define RELAY_GROUP_SYNC_NORMAL      0
#define RELAY_GROUP_SYNC_INVERSE     1
#define RELAY_GROUP_SYNC_RECEIVEONLY 2

#define RELAY_LOCK_DISABLED          0
#define RELAY_LOCK_OFF               1
#define RELAY_LOCK_ON                2

#ifndef DUMMY_RELAY_COUNT
#define DUMMY_RELAY_COUNT     0
#endif

#ifndef RELAY1_PIN
#define RELAY1_PIN            GPIO_NONE
#endif
#ifndef RELAY2_PIN
#define RELAY2_PIN            GPIO_NONE
#endif
#ifndef RELAY3_PIN
#define RELAY3_PIN            GPIO_NONE
#endif
#ifndef RELAY4_PIN
#define RELAY4_PIN            GPIO_NONE
#endif
#ifndef RELAY5_PIN
#define RELAY5_PIN            GPIO_NONE
#endif
#ifndef RELAY6_PIN
#define RELAY6_PIN            GPIO_NONE
#endif
#ifndef RELAY7_PIN
#define RELAY7_PIN            GPIO_NONE
#endif
#ifndef RELAY8_PIN
#define RELAY8_PIN            GPIO_NONE
#endif

#ifndef RELAY1_TYPE
#define RELAY1_TYPE           RELAY_TYPE_NORMAL
#endif
#ifndef RELAY2_TYPE
#define RELAY2_TYPE           RELAY_TYPE_NORMAL
#endif
#ifndef RELAY3_TYPE
#define RELAY3_TYPE           RELAY_TYPE_NORMAL
#endif
#ifndef RELAY4_TYPE
#define RELAY4_TYPE           RELAY_TYPE_NORMAL
#endif
#ifndef RELAY5_TYPE
#define RELAY5_TYPE           RELAY_TYPE_NORMAL
#endif
#ifndef RELAY6_TYPE
#define RELAY6_TYPE           RELAY_TYPE_NORMAL
#endif
#ifndef RELAY7_TYPE
#define RELAY7_TYPE           RELAY_TYPE_NORMAL
#endif
#ifndef RELAY8_TYPE
#define RELAY8_TYPE           RELAY_TYPE_NORMAL
#endif

#ifndef RELAY1_RESET_PIN
#define RELAY1_RESET_PIN      GPIO_NONE
#endif
#ifndef RELAY2_RESET_PIN
#define RELAY2_RESET_PIN      GPIO_NONE
#endif
#ifndef RELAY3_RESET_PIN
#define RELAY3_RESET_PIN      GPIO_NONE
#endif
#ifndef RELAY4_RESET_PIN
#define RELAY4_RESET_PIN      GPIO_NONE
#endif
#ifndef RELAY5_RESET_PIN
#define RELAY5_RESET_PIN      GPIO_NONE
#endif
#ifndef RELAY6_RESET_PIN
#define RELAY6_RESET_PIN      GPIO_NONE
#endif
#ifndef RELAY7_RESET_PIN
#define RELAY7_RESET_PIN      GPIO_NONE
#endif
#ifndef RELAY8_RESET_PIN
#define RELAY8_RESET_PIN      GPIO_NONE
#endif

#ifndef RELAY1_DELAY_ON
#define RELAY1_DELAY_ON       0
#endif
#ifndef RELAY2_DELAY_ON
#define RELAY2_DELAY_ON       0
#endif
#ifndef RELAY3_DELAY_ON
#define RELAY3_DELAY_ON       0
#endif
#ifndef RELAY4_DELAY_ON
#define RELAY4_DELAY_ON       0
#endif
#ifndef RELAY5_DELAY_ON
#define RELAY5_DELAY_ON       0
#endif
#ifndef RELAY6_DELAY_ON
#define RELAY6_DELAY_ON       0
#endif
#ifndef RELAY7_DELAY_ON
#define RELAY7_DELAY_ON       0
#endif
#ifndef RELAY8_DELAY_ON
#define RELAY8_DELAY_ON       0
#endif

#ifndef RELAY1_DELAY_OFF
#define RELAY1_DELAY_OFF      0
#endif
#ifndef RELAY2_DELAY_OFF
#define RELAY2_DELAY_OFF      0
#endif
#ifndef RELAY3_DELAY_OFF
#define RELAY3_DELAY_OFF      0
#endif
#ifndef RELAY4_DELAY_OFF
#define RELAY4_DELAY_OFF      0
#endif
#ifndef RELAY5_DELAY_OFF
#define RELAY5_DELAY_OFF      0
#endif
#ifndef RELAY6_DELAY_OFF
#define RELAY6_DELAY_OFF      0
#endif
#ifndef RELAY7_DELAY_OFF
#define RELAY7_DELAY_OFF      0
#endif
#ifndef RELAY8_DELAY_OFF
#define RELAY8_DELAY_OFF      0
#endif

#ifndef RELAY_DELAY_INTERLOCK
#define RELAY_DELAY_INTERLOCK       0
#endif

//------------------------------------------------------------------------------
// RELAY
//------------------------------------------------------------------------------

// Default boot mode: 0 means OFF, 1 ON and 2 whatever was before
#ifndef RELAY_BOOT_MODE
#define RELAY_BOOT_MODE             RELAY_BOOT_OFF
#endif

// 0 means ANY, 1 zero or one and 2 one and only one
#ifndef RELAY_SYNC
#define RELAY_SYNC                  RELAY_SYNC_ANY
#endif

// Default pulse mode: 0 means no pulses, 1 means normally off, 2 normally on
#ifndef RELAY_PULSE_MODE
#define RELAY_PULSE_MODE            RELAY_PULSE_NONE
#endif

// Default pulse time in seconds
#ifndef RELAY_PULSE_TIME
#define RELAY_PULSE_TIME            1.0
#endif

// Relay requests flood protection window - in seconds
#ifndef RELAY_FLOOD_WINDOW
#define RELAY_FLOOD_WINDOW          3
#endif

// Allowed actual relay changes inside requests flood protection window
#ifndef RELAY_FLOOD_CHANGES
#define RELAY_FLOOD_CHANGES         5
#endif

// Pulse with in milliseconds for a latched relay
#ifndef RELAY_LATCHING_PULSE
#define RELAY_LATCHING_PULSE        10
#endif

// Do not save relay state after these many milliseconds
#ifndef RELAY_SAVE_DELAY
#define RELAY_SAVE_DELAY            1000
#endif

#ifndef RELAY_REPORT_STATUS
#define RELAY_REPORT_STATUS         1
#endif

// Configure the MQTT payload for ON, OFF and TOGGLE
#ifndef RELAY_MQTT_OFF
#define RELAY_MQTT_OFF              "0"
#endif

#ifndef RELAY_MQTT_ON
#define RELAY_MQTT_ON               "1"
#endif

#ifndef RELAY_MQTT_TOGGLE
#define RELAY_MQTT_TOGGLE           "2"
#endif
//#include "utils.h"

constexpr size_t RELAYS_MAX = 32; 

enum class RelayStatus : unsigned char {
    OFF = 0,
    ON = 1,
    TOGGLE = 2,
    UNKNOWN = 0xFF
};

struct RelayMask {

    explicit RelayMask(const String& string) :
        as_string(string),
        as_u32(u32fromString(string))
    {}

    explicit RelayMask(String&& string) :
        as_string(std::move(string)),
        as_u32(u32fromString(as_string))
    {}

    explicit RelayMask(uint32_t value) :
        as_string(std::move(u32toString(value, 2))),
        as_u32(value)
    {}

    explicit RelayMask(std::bitset<RELAYS_MAX> bitset) :
        RelayMask(bitset.to_ulong())
    {}

    RelayMask(String&& string, uint32_t value) :
        as_string(std::move(string)),
        as_u32(value)
    {}

    const String as_string;
    uint32_t as_u32;

};

RelayStatus relayParsePayload(const char * payload);

bool relayStatus(unsigned char id, bool status, bool report, bool group_report);
bool relayStatus(unsigned char id, bool status);
bool relayStatus(unsigned char id);

void relayToggle(unsigned char id, bool report, bool group_report);
void relayToggle(unsigned char id);

unsigned char relayCount();

const String& relayPayloadOn();
const String& relayPayloadOff();
const String& relayPayloadToggle();

const char* relayPayload(RelayStatus status);

void relaySetupDummy(size_t size, bool reconfigure = false);

void relaySetup();
void relayPulse(unsigned char id);
void relaySave(bool eeprom);
void relaySync(unsigned char id);

void relayMQTT();
void relayMQTT(unsigned char id);