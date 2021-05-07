// -----------------------------------------------------------------------------
// BME280/BMP280 Sensor over I2C
// Copyright (C) 2017-2019 by Xose Pérez <xose dot perez at gmail dot com>
// -----------------------------------------------------------------------------

#ifndef INC_BMX280SENSOR_HPP
#define INC_BMX280SENSOR_HPP

#include "config/hardware.h"
#if SENSOR_SUPPORT && BMX280_SUPPORT

#undef I2C_SUPPORT
#define I2C_SUPPORT 1 // Explicitly request I2C support.

#include "Arduino.h"
#include "drivers/sensors/I2CSensor.hpp"

#define UI_TAG_INPUT                0
#define UI_TAG_CHECKBOX             1
#define UI_TAG_SELECT               2

#define BMX280_CHIP_BMP280              0x58
#define BMX280_CHIP_BME280              0x60

#define BMX280_REGISTER_DIG_T1          0x88
#define BMX280_REGISTER_DIG_T2          0x8A
#define BMX280_REGISTER_DIG_T3          0x8C

#define BMX280_REGISTER_DIG_P1          0x8E
#define BMX280_REGISTER_DIG_P2          0x90
#define BMX280_REGISTER_DIG_P3          0x92
#define BMX280_REGISTER_DIG_P4          0x94
#define BMX280_REGISTER_DIG_P5          0x96
#define BMX280_REGISTER_DIG_P6          0x98
#define BMX280_REGISTER_DIG_P7          0x9A
#define BMX280_REGISTER_DIG_P8          0x9C
#define BMX280_REGISTER_DIG_P9          0x9E

#define BMX280_REGISTER_DIG_H1          0xA1
#define BMX280_REGISTER_DIG_H2          0xE1
#define BMX280_REGISTER_DIG_H3          0xE3
#define BMX280_REGISTER_DIG_H4          0xE4
#define BMX280_REGISTER_DIG_H5          0xE5
#define BMX280_REGISTER_DIG_H6          0xE7

#define BMX280_REGISTER_CHIPID          0xD0
#define BMX280_REGISTER_VERSION         0xD1
#define BMX280_REGISTER_SOFTRESET       0xE0

#define BMX280_REGISTER_CAL26           0xE1

#define BMX280_REGISTER_CONTROLHUMID    0xF2
#define BMX280_REGISTER_CONTROL         0xF4
#define BMX280_REGISTER_CONFIG          0xF5
#define BMX280_REGISTER_PRESSUREDATA    0xF7
#define BMX280_REGISTER_TEMPDATA        0xFA
#define BMX280_REGISTER_HUMIDDATA       0xFD

typedef struct
{
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;

    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;

    uint8_t  dig_H1;
    int16_t  dig_H2;
    uint8_t  dig_H3;
    int16_t  dig_H4;
    int16_t  dig_H5;
    int8_t   dig_H6;

} bmx280_calib_t;

class BMX280Sensor : public I2CSensor {

    public:

        BMX280Sensor();
        void begin();
        String description();
        unsigned char type(unsigned char index);
        signed char decimals(unsigned char type);
        virtual void pre();
        double value(unsigned char index);
        static void manifest(JsonArray& sensors);
        void getConfig(JsonObject& root);
        void setConfig(JsonObject& root);

        static unsigned char addresses[2];

    protected:

        void _init(i2c_port_t i2c_port);
        void _readCoefficients();
        unsigned long _measurementTime();
        void _forceRead();
        unsigned char _read();

        unsigned char _chip;
        unsigned long _measurement_delay;
        bool _run_init = false;
        double _temperature = 0;
        double _pressure = 0;
        double _humidity = 0;

        bmx280_calib_t _bmx280_calib;
};

#endif /* SENSOR_SUPPORT && BMX280_SUPPORT */

#endif /* INC_BMX280SENSOR_HPP */