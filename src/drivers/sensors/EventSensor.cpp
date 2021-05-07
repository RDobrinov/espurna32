#include "drivers/sensors/EventSensor.hpp"

// ---------------------------------------------------------------------
// Public
// ---------------------------------------------------------------------
#if SENSOR_SUPPORT && EVENTS_SUPPORT
#include "espurna32.hpp"

EventSensor * _event_sensor_instance[EVENTS_SENSORS_MAX] = {nullptr};

void IRAM_ATTR _event_sensor_isr(unsigned char channel) {
    if(channel>=EVENTS_SENSORS_MAX) return;
    if (_event_sensor_instance[channel]) {
        _event_sensor_instance[channel]->handleInterrupt();
    }
}

void IRAM_ATTR _event_sensor_isr_0() { _event_sensor_isr(0); }
void IRAM_ATTR _event_sensor_isr_1() { _event_sensor_isr(1); }
void IRAM_ATTR _event_sensor_isr_2() { _event_sensor_isr(2); }
void IRAM_ATTR _event_sensor_isr_3() { _event_sensor_isr(3); }
void IRAM_ATTR _event_sensor_isr_4() { _event_sensor_isr(4); }

static void (*_event_sensor_isr_list[EVENTS_SENSORS_MAX])() = {
    _event_sensor_isr_0, _event_sensor_isr_1, _event_sensor_isr_2,
    _event_sensor_isr_3, _event_sensor_isr_4
};

EventSensor::EventSensor(): BaseSensor() {
    _count = 1;
    _sensor_id = SENSOR_EVENTS_ID;
}

EventSensor::~EventSensor() {
    _enableInterrupts(false);
}

// ---------------------------------------------------------------------

void EventSensor::setGPIO(unsigned char gpio) {
    _gpio = gpio;
}

void EventSensor::setTrigger(bool trigger) {
    _trigger = trigger;
    onEvent([this](unsigned char type, double value) {
        this->onEventCallback(type, value);
    });
}

void EventSensor::setPinMode(unsigned char pin_mode) {
    _pin_mode = pin_mode;
}

void EventSensor::setInterruptMode(unsigned char interrupt_mode) {
    _interrupt_mode = interrupt_mode;
}

void EventSensor::setDebounceTime(unsigned long ms) {
    _debounce = microsecondsToClockCycles(ms * 1000);
}

bool EventSensor::setChannel(unsigned char channel)
{
    if( channel >= EVENTS_SENSORS_MAX ) return false;
    if((_event_sensor_instance[channel] != this) && (_event_sensor_instance[channel] != nullptr)) return false;
    _isr_channel = channel;
    return true;
}

// ---------------------------------------------------------------------

unsigned char EventSensor::getGPIO() {
    return _gpio;
}

bool EventSensor::getTrigger() {
    return _trigger;
}

unsigned char EventSensor::getPinMode() {
    return _pin_mode;
}

unsigned char EventSensor::getInterruptMode() {
    return _interrupt_mode;
}

unsigned long EventSensor::getDebounceTime() {
    return _debounce;
}

// ---------------------------------------------------------------------
// Sensors API
// ---------------------------------------------------------------------

// Initialization method, must be idempotent
// Defined outside the class body
void EventSensor::begin() {
    pinMode(_gpio, _pin_mode);
    _enableInterrupts(true);
    _count = _trigger ? 2 : 1;
    _ready = true;
}

void EventSensor::tick() {
    if (!_trigger || !_callback) return;
    if (!_trigger_flag) return;

    noInterrupts();
    _callback(MAGNITUDE_EVENT, _trigger_value);
    _trigger_flag = false;
    interrupts();
}

// Descriptive name of the sensor
String EventSensor::description() {
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "ISR @ GPIO%d", _gpio);
    return String(buffer);
}

// Descriptive name of the slot # index
String EventSensor::slot(unsigned char index) {
    return description();
};

// Address of the sensor (it could be the GPIO or I2C address)
String EventSensor::address(unsigned char index) {
    return String(_gpio);
}

// Type for slot # index
unsigned char EventSensor::type(unsigned char index) {
    if (index == 0) return MAGNITUDE_COUNT;
    if (index == 1) return MAGNITUDE_EVENT;
    return MAGNITUDE_NONE;
}

// Current value for slot # index
double EventSensor::value(unsigned char index) {
    if (index == 0) {
        double value = _counter;
        _counter = 0;
        return value;
    };
    if (index == 1) {
        return _value;
    }
    return 0;
}

// Handle interrupt calls from isr[GPIO] functions
void ICACHE_RAM_ATTR EventSensor::handleInterrupt() {
    //(void)(gpio);

    unsigned long cycles = ESP.getCycleCount();

    if (cycles - _last > _debounce) {
        _last = cycles;
        _counter += 1;

        // we are handling callbacks in tick()
        if (_trigger) {
            _trigger_value = digitalRead(_gpio);
            _trigger_flag = true;
        }
    }
}

void EventSensor::onEventCallback(unsigned char type, double value)
{
    DEBUG_MSG_P(PSTR("[SENSOR] GPIO%u %s Event %s\n"), _gpio, value ? "Rising Edge" : "Falling Edge", String(value).c_str());
}

void EventSensor::_attach() {
    if (!gpioValid(_gpio)) return;

    if (_event_sensor_instance[_isr_channel] == this) return;
    if (_event_sensor_instance[_isr_channel]) detachInterrupt(_gpio);

    _event_sensor_instance[_isr_channel] = this;
    attachInterrupt(_gpio, _event_sensor_isr_list[_isr_channel], _interrupt_mode);

    #if SENSOR_DEBUG
        DEBUG_MSG_P(PSTR("[SENSOR] GPIO%d interrupt attached to %s\n"), gpio, this->description().c_str());
    #endif
}

void EventSensor::_detach() {
    if (!gpioValid(_gpio)) return;

    if (_event_sensor_instance[_isr_channel]) {
        detachInterrupt(_gpio);
        _event_sensor_instance[_isr_channel] = nullptr;

        #if SENSOR_DEBUG
            DEBUG_MSG_P(PSTR("[SENSOR] GPIO%d interrupt detached from %s\n"), gpio, _event_sensor_instance[index]->description().c_str());
        #endif
    }
}

void EventSensor::_enableInterrupts(bool value) {

    if (value) {
        _detach();
        _attach();
    } else {
        _detach();
    }

}

#endif /* SENSOR_SUPPORT && EVENTS_SUPPORT */