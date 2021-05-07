/*

GPIO MODULE

Copyright (C) 2017-2019 by Xose Pérez <xose dot perez at gmail dot com>

*/
#include "drivers/drv-gpio.h"
#include "espurna32.hpp"

constexpr const size_t GPIO_PINS = NUM_DIGITAL_PINS;

std::bitset<GPIO_PINS> _gpio_locked;
std::bitset<GPIO_PINS> _gpio_available;

bool gpioValid(unsigned char gpio) {
    if (gpio >= GPIO_PINS) return false;

    return _gpio_available.test(gpio);
}

bool gpioGetLock(unsigned char gpio, String requestedFor) {
    if (gpioValid(gpio)) {
        if (!_gpio_locked.test(gpio)) {
            _gpio_locked.set(gpio);
            DEBUG_MSG_P(PSTR("[GPIO] GPIO%u locked for %s\n"), gpio, requestedFor.c_str());
            return true;
        }
    }
    DEBUG_MSG_P(PSTR("[GPIO] Failed getting lock for GPIO%u\n"), gpio);
    return false;
}

bool gpioReleaseLock(unsigned char gpio) {
    if (gpioValid(gpio)) {
        _gpio_locked.reset(gpio);
        DEBUG_MSG_P(PSTR("[GPIO] GPIO%u lock released\n"), gpio);
        return true;
    }
    DEBUG_MSG_P(PSTR("[GPIO] Failed releasing lock for GPIO%u\n"), gpio);
    return false;
}

void gpioSetup() {

    for (unsigned char pin=0; pin < GPIO_PINS; ++pin) 
    {
        if ((pin < 6) || (pin>11)) _gpio_available.set(pin);
        else _gpio_available.reset(pin);
    }
}
