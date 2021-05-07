#ifndef INC_DRVGPIO_HPP
#define INC_DRVGPIO_HPP

#define GPIO_NONE           0x99

#include <Arduino.h>
#include <bitset>

bool gpioValid(unsigned char gpio);
bool gpioGetLock(unsigned char gpio, String requestedFor = "GPIO");
bool gpioReleaseLock(unsigned char gpio);

void gpioSetup();

#endif /* INC_DRVGPIO_HPP */