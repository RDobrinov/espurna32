// -----------------------------------------------------------------------------
// Dallas OneWire Sensor
// Uses OneWire library
// Copyright (C) 2017-2019 by Xose Pérez <xose dot perez at gmail dot com>
// -----------------------------------------------------------------------------

#ifndef INC_DALLASSENSOR_HPP
#define INC_DALLASSENSOR_HPP

#include "config/hardware.h"


#ifndef DALLAS_SUPPORT
#define DALLAS_SUPPORT                  0
#endif

#if SENSOR_SUPPORT && DALLAS_SUPPORT

#include <vector>
#include <OneWire.h>
#include <Arduino.h>
#include "drivers/sensors/SensorsConfig.hpp"
#include "drivers/sensors/BaseSensor.hpp"
#include "drivers/drv-gpio.h"

#define DS_CHIP_DS18S20             0x10
#define DS_CHIP_DS1822              0x22
#define DS_CHIP_DS18B20             0x28
#define DS_CHIP_DS1825              0x3B

#define DS_DATA_SIZE                9
#define DS_PARASITE                 1
#define DS_DISCONNECTED             -127

#define DS_CMD_START_CONVERSION     0x44
#define DS_CMD_READ_SCRATCHPAD      0xBE

typedef struct {
    uint8_t address[8];
    uint8_t data[DS_DATA_SIZE];
} ds_device_t;

class DallasSensor : public BaseSensor {

    public:

        DallasSensor();
        ~DallasSensor();

        void setGPIO(unsigned char gpio) ;
        unsigned char getGPIO();
        void begin();
        void tick();
        String description();
        String address(unsigned char index);
        String slot(unsigned char index);
        unsigned char type(unsigned char index);
        signed char decimals(unsigned char type);
        void pre();
        double value(unsigned char index);

    protected:

        bool validateID(unsigned char id);
        unsigned char chip(unsigned char index);
        String chipAsString(unsigned char index);
        void loadDevices();

        std::vector<ds_device_t> _devices;

        unsigned char _gpio = GPIO_NONE;
        unsigned char _previous = GPIO_NONE;
        OneWire * _wire = NULL;
};    
#endif /* SENSOR_SUPPORT && DALLAS_SUPPORT */

#endif /* INC_DALLASSENSOR_HPP */