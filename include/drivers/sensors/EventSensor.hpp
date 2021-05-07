// -----------------------------------------------------------------------------
// Event Counter Sensor
// Copyright (C) 2017-2019 by Xose Pérez <xose dot perez at gmail dot com>
// -----------------------------------------------------------------------------
#ifndef INC_EVENTSENSOR_HPP
#define INC_EVENTSENSOR_HPP

#include "config/hardware.h"

#ifndef EVENTS_SUPPORT
#define EVENTS_SUPPORT                  0       // Do not build with counter support by default
#endif

#if SENSOR_SUPPORT && EVENTS_SUPPORT

#include "Arduino.h"
#include "drivers/drv-gpio.h"
#include "drivers/sensors/BaseSensor.hpp"
#include "drivers/sensors/SensorsConfig.hpp"

// we are bound by usable GPIOs
#define EVENTS_SENSORS_MAX 5


class EventSensor : public BaseSensor {

    public:
        EventSensor();
        ~EventSensor();

        void setGPIO(unsigned char gpio);
        void setTrigger(bool trigger);
        void setPinMode(unsigned char pin_mode);
        void setInterruptMode(unsigned char interrupt_mode);
        void setDebounceTime(unsigned long ms);
        bool setChannel(unsigned char channel);

        unsigned char getGPIO();
        bool getTrigger();
        unsigned char getPinMode();
        unsigned char getInterruptMode();
        unsigned long getDebounceTime();

        void begin();
        void tick();
        String description();
        String slot(unsigned char index);
        String address(unsigned char index);
        unsigned char type(unsigned char index);
        double value(unsigned char index);
        void ICACHE_RAM_ATTR handleInterrupt();
        void onEventCallback(unsigned char type, double value);

    protected:

        // ---------------------------------------------------------------------
        // Interrupt management
        // ---------------------------------------------------------------------

        void _attach();
        void _detach();

        void _enableInterrupts(bool value);

        // ---------------------------------------------------------------------
        // Protected
        // ---------------------------------------------------------------------

        volatile unsigned long _counter = 0;
        unsigned char _value = 0;
        unsigned long _last = 0;
        unsigned long _debounce = microsecondsToClockCycles(EVENTS1_DEBOUNCE * 1000);

        bool _trigger = false;
        bool _trigger_flag = false;
        unsigned char _trigger_value = false;

        unsigned char _gpio = GPIO_NONE;
        unsigned char _pin_mode = INPUT;
        unsigned char _interrupt_mode = RISING;

        unsigned char _isr_channel = 0x0f;
};

// -----------------------------------------------------------------------------
// Interrupt helpers
// -----------------------------------------------------------------------------

#endif // SENSOR_SUPPORT && EVENTS_SUPPORT

#endif /* INC_EVENTSENSOR_HPP */