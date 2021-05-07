// -----------------------------------------------------------------------------
// HOME ASSISTANT
// -----------------------------------------------------------------------------

#ifndef HOMEASSISTANT_SUPPORT
#define HOMEASSISTANT_SUPPORT   MQTT_SUPPORT    // Build with home assistant support (if MQTT, 1.64Kb)
#endif

#ifndef HOMEASSISTANT_ENABLED
#define HOMEASSISTANT_ENABLED   0               // Integration not enabled by default
#endif

#ifndef HOMEASSISTANT_PREFIX
#define HOMEASSISTANT_PREFIX    "homeassistant" // Default MQTT prefix
#endif

void haSetup();