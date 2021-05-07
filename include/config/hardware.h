// -----------------------------------------------------------------------------
// Configuration HELP
// -----------------------------------------------------------------------------
//
// MANUFACTURER: Name of the manufacturer of the board ("string")
// DEVICE: Name of the device ("string")
// BUTTON#_PIN: GPIO for the n-th button (1-based, up to 4 buttons)
// BUTTON#_RELAY: Relay number that will be bind to the n-th button (1-based)
// BUTTON#_MODE: A mask of options (BUTTON_PUSHBUTTON and BUTTON_SWITCH cannot be together)
//   - BUTTON_PUSHBUTTON: button event is fired when released
//   - BUTTON_SWITCH: button event is fired when pressed or released
//   - BUTTON_DEFAULT_HIGH: there is a pull up in place
//   - BUTTON_SET_PULLUP: set pullup by software
// RELAY#_PIN: GPIO for the n-th relay (1-based, up to 8 relays)
// RELAY#_TYPE: Relay can be RELAY_TYPE_NORMAL, RELAY_TYPE_INVERSE, RELAY_TYPE_LATCHED or RELAY_TYPE_LATCHED_INVERSE
// LED#_PIN: GPIO for the n-th LED (1-based, up to 8 LEDs)
// LED#_PIN_INVERSE: LED has inversed logic (lit when pulled down)
// LED#_MODE: Check types.h for LED_MODE_%
// LED#_RELAY: Linked relay (1-based)
//
// Besides, other hardware specific information should be stated here

// -----------------------------------------------------------------------------
// Custom hardware
// -----------------------------------------------------------------------------

#if defined(MANUFACTURER) and defined(DEVICE)

    // user has defined custom hardware, no need to check anything else

// -----------------------------------------------------------------------------
// ESPurna Core
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Development boards
// -----------------------------------------------------------------------------
#elif defined(ESPURNA32_DEV)

    // This is a special device with no specific hardware 
    // with the basics to easily upgrade it to a device-specific image

    // Info
    #define MANUFACTURER            "LILYGO"
    #define DEVICE                  "TTGO_T7"

    // Disable non-core modules
    #define ALEXA_SUPPORT           0
    #define API_SUPPORT             1
    #define BROKER_SUPPORT          1
    #define DOMOTICZ_SUPPORT        0
    #define DEBUG_SERIAL_SUPPORT    1
    #define DEBUG_TELNET_SUPPORT    0
    #define DEBUG_WEB_SUPPORT       0
    #define HOMEASSISTANT_SUPPORT   1
    #define I2C_SUPPORT             1
    #define SPIBUS_SUPPORT          1
    #define MQTT_SUPPORT            1
    #define NTP_SUPPORT             1
    #define SCHEDULER_SUPPORT       1
    #define SENSOR_SUPPORT          1
    #define THINGSPEAK_SUPPORT      0
    #define WEB_SUPPORT             1
    #define INFLUXDB_SUPPORT        1

    // Extra light-weight image
    #define BUTTON_SUPPORT          1
    #define LED_SUPPORT             1
    #define MDNS_SERVER_SUPPORT     1
    #define TELNET_SUPPORT          0
    #define TERMINAL_SUPPORT        0
    #define RPN_RULES_SUPPORT       1

    //-----
    // Buttons
    #define BUTTON1_PIN         0
    #define BUTTON1_MODE        BUTTON_PUSHBUTTON | BUTTON_DEFAULT_HIGH
    #define BUTTON1_RELAY       1

    #define RELAY1_PIN          25
    #define RELAY1_TYPE         RELAY_TYPE_NORMAL

    // LEDs
    #define LED1_PIN            19
    #define LED1_PIN_INVERSE    0

    // Sensors
    #define BMX280_SUPPORT      1

    #define DALLAS_SUPPORT      1

    #define RTD_SUPPORT         1

    #define EVENTS_SUPPORT      1

#else

    #error "UNSUPPORTED HARDWARE!!"

#endif

