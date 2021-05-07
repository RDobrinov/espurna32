
// -----------------------------------------------------------------------------
// Abstract I2C sensor class (other sensor classes extend this class)
// Copyright (C) 2017-2019 by Xose Pérez <xose dot perez at gmail dot com>
// -----------------------------------------------------------------------------

#include "config/hardware.h"

#if SENSOR_SUPPORT && ( I2C_SUPPORT || EMON_ANALOG_SUPPORT )

#include "drivers/sensors/BaseSensor.hpp"
#include "drivers/drv-i2c_master.hpp"

#define I2C_TRANS_SUCCESS               0   // success
#define I2C_TRANS_ERROR_BUFFER_OVERLOW  1   // data too long to fit in transmit buffer
#define I2C_TRANS_ERROR_NACK_ADDRESS    2   // received NACK on transmit of address
#define I2C_TRANS_ERROR_NACK_DATA       3   // received NACK on transmit of data
#define I2C_TRANS_ERROR_OTHER           4   // other error


class I2CSensor : public BaseSensor {

    public:

        void setAddress(unsigned char address, i2c_port_t i2c_port = I2C_NUM_0);
        unsigned char getAddress();
        i2c_port_t getPort();

        // Descriptive name of the slot # index
        String slot(unsigned char index);

        // Address of the sensor (it could be the GPIO or I2C address)
        String address(unsigned char index);

    protected:

        // Specific for I2C sensors
        unsigned char _begin_i2c(unsigned char address, size_t size, unsigned char * addresses, i2c_port_t i2c_port=I2C_NUM_0);

        unsigned char _previous_address = 0;
        unsigned char _address = 0;
        i2c_port_t _i2c_port = I2C_NUM_MAX;
};

#endif /* SENSOR SUPPORT */