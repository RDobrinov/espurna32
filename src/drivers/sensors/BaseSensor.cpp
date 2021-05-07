#include "config/hardware.h"

#if SENSOR_SUPPORT

#include "drivers/sensors/BaseSensor.hpp"

BaseSensor::BaseSensor() 
{
    _callback = NULL;
    _sensor_id = 0x00;
    _error = 0;
    _dirty = true;
    _count = 0;
    _ready = false;
}

BaseSensor::~BaseSensor() {}
void BaseSensor::begin() {}
void BaseSensor::tick() {}
void BaseSensor::pre() {}
void BaseSensor::post() {}
signed char BaseSensor::decimals(unsigned char type) { return -1; }
void BaseSensor::getConfig(JsonObject& root) {};
void BaseSensor::setConfig(JsonObject& root) {};
void BaseSensor::manifest(JsonArray& root) {};
unsigned char BaseSensor::getID() { return _sensor_id; };
bool BaseSensor::status() { return 0 == _error; }
bool BaseSensor::ready() { return _ready; }
int BaseSensor::error() { return _error; }
unsigned char BaseSensor::count() { return _count; }
void BaseSensor::onEvent(TSensorCallback fn) { _callback = fn; };
TSensorCallback BaseSensor::getCallback() { return _callback; };

#endif /* SENSOR_SUPPORT */