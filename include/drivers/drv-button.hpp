
#ifndef BUTTON_SUPPORT
#define BUTTON_SUPPORT              1
#endif

#ifndef BUTTON_DEBOUNCE_DELAY
#define BUTTON_DEBOUNCE_DELAY       50          // Debounce delay (ms)
#endif

#ifndef BUTTON_DBLCLICK_DELAY
#define BUTTON_DBLCLICK_DELAY       500         // Time in ms to wait for a second (or third...) click
#endif

#ifndef BUTTON_LNGCLICK_DELAY
#define BUTTON_LNGCLICK_DELAY       1000        // Time in ms holding the button down to get a long click
#endif

#ifndef BUTTON_LNGLNGCLICK_DELAY
#define BUTTON_LNGLNGCLICK_DELAY    10000       // Time in ms holding the button down to get a long-long click
#endif

#ifndef BUTTON_MQTT_SEND_ALL_EVENTS
#define BUTTON_MQTT_SEND_ALL_EVENTS 0           // 0 - to send only events the are bound to actions
                                                // 1 - to send all button events to MQTT
#endif
//------------------------------------------------------------------------------
// BUTTONS
//------------------------------------------------------------------------------

#define BUTTON_EVENT_NONE           0
#define BUTTON_EVENT_PRESSED        1
#define BUTTON_EVENT_RELEASED       2
#define BUTTON_EVENT_CLICK          2
#define BUTTON_EVENT_DBLCLICK       3
#define BUTTON_EVENT_LNGCLICK       4
#define BUTTON_EVENT_LNGLNGCLICK    5
#define BUTTON_EVENT_TRIPLECLICK    6

#define BUTTON_MODE_NONE            0
#define BUTTON_MODE_TOGGLE          1
#define BUTTON_MODE_ON              2
#define BUTTON_MODE_OFF             3
#define BUTTON_MODE_AP              4
#define BUTTON_MODE_RESET           5
#define BUTTON_MODE_PULSE           6
#define BUTTON_MODE_FACTORY         7
#define BUTTON_MODE_WPS             8
#define BUTTON_MODE_SMART_CONFIG    9
#define BUTTON_MODE_DIM_UP          10
#define BUTTON_MODE_DIM_DOWN        11


// Needed for ESP8285 boards under Windows using PlatformIO (?)
#ifndef BUTTON_PUSHBUTTON
#define BUTTON_PUSHBUTTON           0
#define BUTTON_SWITCH               1
#define BUTTON_DEFAULT_HIGH         2
#define BUTTON_SET_PULLUP           4
#endif

// -----------------------------------------------------------------------------
// Buttons
// -----------------------------------------------------------------------------

#ifndef BUTTON1_PIN
#define BUTTON1_PIN         GPIO_NONE
#endif
#ifndef BUTTON2_PIN
#define BUTTON2_PIN         GPIO_NONE
#endif
#ifndef BUTTON3_PIN
#define BUTTON3_PIN         GPIO_NONE
#endif
#ifndef BUTTON4_PIN
#define BUTTON4_PIN         GPIO_NONE
#endif
#ifndef BUTTON5_PIN
#define BUTTON5_PIN         GPIO_NONE
#endif
#ifndef BUTTON6_PIN
#define BUTTON6_PIN         GPIO_NONE
#endif
#ifndef BUTTON7_PIN
#define BUTTON7_PIN         GPIO_NONE
#endif
#ifndef BUTTON8_PIN
#define BUTTON8_PIN         GPIO_NONE
#endif

#ifndef BUTTON1_PRESS
#define BUTTON1_PRESS       BUTTON_MODE_NONE
#endif
#ifndef BUTTON2_PRESS
#define BUTTON2_PRESS       BUTTON_MODE_NONE
#endif
#ifndef BUTTON3_PRESS
#define BUTTON3_PRESS       BUTTON_MODE_NONE
#endif
#ifndef BUTTON4_PRESS
#define BUTTON4_PRESS       BUTTON_MODE_NONE
#endif
#ifndef BUTTON5_PRESS
#define BUTTON5_PRESS       BUTTON_MODE_NONE
#endif
#ifndef BUTTON6_PRESS
#define BUTTON6_PRESS       BUTTON_MODE_NONE
#endif
#ifndef BUTTON7_PRESS
#define BUTTON7_PRESS       BUTTON_MODE_NONE
#endif
#ifndef BUTTON8_PRESS
#define BUTTON8_PRESS       BUTTON_MODE_NONE
#endif

#ifndef BUTTON1_CLICK
#define BUTTON1_CLICK       BUTTON_MODE_TOGGLE
#endif
#ifndef BUTTON2_CLICK
#define BUTTON2_CLICK       BUTTON_MODE_TOGGLE
#endif
#ifndef BUTTON3_CLICK
#define BUTTON3_CLICK       BUTTON_MODE_TOGGLE
#endif
#ifndef BUTTON4_CLICK
#define BUTTON4_CLICK       BUTTON_MODE_TOGGLE
#endif
#ifndef BUTTON5_CLICK
#define BUTTON5_CLICK       BUTTON_MODE_TOGGLE
#endif
#ifndef BUTTON6_CLICK
#define BUTTON6_CLICK       BUTTON_MODE_TOGGLE
#endif
#ifndef BUTTON7_CLICK
#define BUTTON7_CLICK       BUTTON_MODE_TOGGLE
#endif
#ifndef BUTTON8_CLICK
#define BUTTON8_CLICK       BUTTON_MODE_TOGGLE
#endif

#ifndef BUTTON1_DBLCLICK
#define BUTTON1_DBLCLICK    BUTTON_MODE_AP
#endif
#ifndef BUTTON2_DBLCLICK
#define BUTTON2_DBLCLICK    BUTTON_MODE_NONE
#endif
#ifndef BUTTON3_DBLCLICK
#define BUTTON3_DBLCLICK    BUTTON_MODE_NONE
#endif
#ifndef BUTTON4_DBLCLICK
#define BUTTON4_DBLCLICK    BUTTON_MODE_NONE
#endif
#ifndef BUTTON5_DBLCLICK
#define BUTTON5_DBLCLICK    BUTTON_MODE_NONE
#endif
#ifndef BUTTON6_DBLCLICK
#define BUTTON6_DBLCLICK    BUTTON_MODE_NONE
#endif
#ifndef BUTTON7_DBLCLICK
#define BUTTON7_DBLCLICK    BUTTON_MODE_NONE
#endif
#ifndef BUTTON8_DBLCLICK
#define BUTTON8_DBLCLICK    BUTTON_MODE_NONE
#endif

#ifndef BUTTON1_TRIPLECLICK
#define BUTTON1_TRIPLECLICK    BUTTON_MODE_SMART_CONFIG
#endif
#ifndef BUTTON2_TRIPLECLICK
#define BUTTON2_TRIPLECLICK    BUTTON_MODE_NONE
#endif
#ifndef BUTTON3_TRIPLECLICK
#define BUTTON3_TRIPLECLICK    BUTTON_MODE_NONE
#endif
#ifndef BUTTON4_TRIPLECLICK
#define BUTTON4_TRIPLECLICK    BUTTON_MODE_NONE
#endif
#ifndef BUTTON5_TRIPLECLICK
#define BUTTON5_TRIPLECLICK    BUTTON_MODE_NONE
#endif
#ifndef BUTTON6_TRIPLECLICK
#define BUTTON6_TRIPLECLICK    BUTTON_MODE_NONE
#endif
#ifndef BUTTON7_TRIPLECLICK
#define BUTTON7_TRIPLECLICK    BUTTON_MODE_NONE
#endif
#ifndef BUTTON8_TRIPLECLICK
#define BUTTON8_TRIPLECLICK    BUTTON_MODE_NONE
#endif

#ifndef BUTTON1_LNGCLICK
#define BUTTON1_LNGCLICK    BUTTON_MODE_RESET
#endif
#ifndef BUTTON2_LNGCLICK
#define BUTTON2_LNGCLICK    BUTTON_MODE_NONE
#endif
#ifndef BUTTON3_LNGCLICK
#define BUTTON3_LNGCLICK    BUTTON_MODE_NONE
#endif
#ifndef BUTTON4_LNGCLICK
#define BUTTON4_LNGCLICK    BUTTON_MODE_NONE
#endif
#ifndef BUTTON5_LNGCLICK
#define BUTTON5_LNGCLICK    BUTTON_MODE_NONE
#endif
#ifndef BUTTON6_LNGCLICK
#define BUTTON6_LNGCLICK    BUTTON_MODE_NONE
#endif
#ifndef BUTTON7_LNGCLICK
#define BUTTON7_LNGCLICK    BUTTON_MODE_NONE
#endif
#ifndef BUTTON8_LNGCLICK
#define BUTTON8_LNGCLICK    BUTTON_MODE_NONE
#endif

#ifndef BUTTON1_LNGLNGCLICK
#define BUTTON1_LNGLNGCLICK BUTTON_MODE_FACTORY
#endif
#ifndef BUTTON2_LNGLNGCLICK
#define BUTTON2_LNGLNGCLICK BUTTON_MODE_NONE
#endif
#ifndef BUTTON3_LNGLNGCLICK
#define BUTTON3_LNGLNGCLICK BUTTON_MODE_NONE
#endif
#ifndef BUTTON4_LNGLNGCLICK
#define BUTTON4_LNGLNGCLICK BUTTON_MODE_NONE
#endif
#ifndef BUTTON5_LNGLNGCLICK
#define BUTTON5_LNGLNGCLICK BUTTON_MODE_NONE
#endif
#ifndef BUTTON6_LNGLNGCLICK
#define BUTTON6_LNGLNGCLICK BUTTON_MODE_NONE
#endif
#ifndef BUTTON7_LNGLNGCLICK
#define BUTTON7_LNGLNGCLICK BUTTON_MODE_NONE
#endif
#ifndef BUTTON8_LNGLNGCLICK
#define BUTTON8_LNGLNGCLICK BUTTON_MODE_NONE
#endif

#ifndef BUTTON1_RELAY
#define BUTTON1_RELAY       0
#endif
#ifndef BUTTON2_RELAY
#define BUTTON2_RELAY       0
#endif
#ifndef BUTTON3_RELAY
#define BUTTON3_RELAY       0
#endif
#ifndef BUTTON4_RELAY
#define BUTTON4_RELAY       0
#endif
#ifndef BUTTON5_RELAY
#define BUTTON5_RELAY       0
#endif
#ifndef BUTTON6_RELAY
#define BUTTON6_RELAY       0
#endif
#ifndef BUTTON7_RELAY
#define BUTTON7_RELAY       0
#endif
#ifndef BUTTON8_RELAY
#define BUTTON8_RELAY       0
#endif

void buttonSetup();