//------------------------------------------------------------------------------
// LED
//------------------------------------------------------------------------------

#define LED_MODE_MQTT               0       // LED will be managed from MQTT (OFF by default)
#define LED_MODE_WIFI               1       // LED will blink according to the WIFI status
#define LED_MODE_FOLLOW             2       // LED will follow state of linked relay (check RELAY#_LED)
#define LED_MODE_FOLLOW_INVERSE     3       // LED will follow the opposite state of linked relay (check RELAY#_LED)
#define LED_MODE_FINDME             4       // LED will be ON if all relays are OFF
#define LED_MODE_FINDME_WIFI        5       // A mixture between WIFI and FINDME
#define LED_MODE_ON                 6       // LED always ON
#define LED_MODE_OFF                7       // LED always OFF
#define LED_MODE_RELAY              8       // If any relay is ON, LED will be ON, otherwise OFF
#define LED_MODE_RELAY_WIFI         9       // A mixture between WIFI and RELAY, the reverse of MIXED


// -----------------------------------------------------------------------------
// LEDs
// -----------------------------------------------------------------------------

#include "drivers/drv-gpio.h"

#ifndef LED1_PIN
#define LED1_PIN            GPIO_NONE
#endif
#ifndef LED2_PIN
#define LED2_PIN            GPIO_NONE
#endif
#ifndef LED3_PIN
#define LED3_PIN            GPIO_NONE
#endif
#ifndef LED4_PIN
#define LED4_PIN            GPIO_NONE
#endif
#ifndef LED5_PIN
#define LED5_PIN            GPIO_NONE
#endif
#ifndef LED6_PIN
#define LED6_PIN            GPIO_NONE
#endif
#ifndef LED7_PIN
#define LED7_PIN            GPIO_NONE
#endif
#ifndef LED8_PIN
#define LED8_PIN            GPIO_NONE
#endif

#ifndef LED1_MODE
#define LED1_MODE           LED_MODE_WIFI
#endif
#ifndef LED2_MODE
#define LED2_MODE           LED_MODE_MQTT
#endif
#ifndef LED3_MODE
#define LED3_MODE           LED_MODE_MQTT
#endif
#ifndef LED4_MODE
#define LED4_MODE           LED_MODE_MQTT
#endif
#ifndef LED5_MODE
#define LED5_MODE           LED_MODE_MQTT
#endif
#ifndef LED6_MODE
#define LED6_MODE           LED_MODE_MQTT
#endif
#ifndef LED7_MODE
#define LED7_MODE           LED_MODE_MQTT
#endif
#ifndef LED8_MODE
#define LED8_MODE           LED_MODE_MQTT
#endif

#ifndef LED1_RELAY
#define LED1_RELAY          1
#endif
#ifndef LED2_RELAY
#define LED2_RELAY          2
#endif
#ifndef LED3_RELAY
#define LED3_RELAY          3
#endif
#ifndef LED4_RELAY
#define LED4_RELAY          4
#endif
#ifndef LED5_RELAY
#define LED5_RELAY          5
#endif
#ifndef LED6_RELAY
#define LED6_RELAY          6
#endif
#ifndef LED7_RELAY
#define LED7_RELAY          7
#endif
#ifndef LED8_RELAY
#define LED8_RELAY          8
#endif

#ifndef LED1_PIN_INVERSE
#define LED1_PIN_INVERSE            0
#endif
#ifndef LED2_PIN_INVERSE
#define LED2_PIN_INVERSE            0
#endif
#ifndef LED3_PIN_INVERSE
#define LED3_PIN_INVERSE            0
#endif
#ifndef LED4_PIN_INVERSE
#define LED4_PIN_INVERSE            0
#endif
#ifndef LED5_PIN_INVERSE
#define LED5_PIN_INVERSE            0
#endif
#ifndef LED6_PIN_INVERSE
#define LED6_PIN_INVERSE            0
#endif
#ifndef LED7_PIN_INVERSE
#define LED7_PIN_INVERSE            0
#endif
#ifndef LED8_PIN_INVERSE
#define LED8_PIN_INVERSE            0
#endif


unsigned char _ledCount();
void ledLoop();
void ledSetup();
void ledUpdate(bool value);