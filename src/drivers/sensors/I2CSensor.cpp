// -----------------------------------------------------------------------------
// Abstract I2C sensor class (other sensor classes extend this class)
// Copyright (C) 2017-2019 by Xose Pérez <xose dot perez at gmail dot com>
// -----------------------------------------------------------------------------

#include "drivers/sensors/I2CSensor.hpp"

#if SENSOR_SUPPORT && ( I2C_SUPPORT || EMON_ANALOG_SUPPORT )

void I2CSensor::setAddress(unsigned char address, i2c_port_t i2c_port) {
    if ((_address == address) && (_i2c_port == i2c_port)) return;
    //Serial.printf("setAddress(unsigned char address=%02X, i2c_port_t i2c_port=%u)\n", address, i2c_port);
    _address = address;
    _i2c_port = i2c_port;
    _dirty = true;
}

unsigned char I2CSensor::getAddress() {
    return _address;
}
i2c_port_t I2CSensor::getPort() {
    return _i2c_port;
}

// Descriptive name of the slot # index
String I2CSensor::slot(unsigned char index) {
    return description();
};

// Address of the sensor (it could be the GPIO or I2C address)
String I2CSensor::address(unsigned char index) {
    char buffer[5];
    snprintf(buffer, sizeof(buffer), "0x%02X", _address);
    return String(buffer);
}



// Specific for I2C sensors protected:
unsigned char I2CSensor::_begin_i2c(unsigned char address, size_t size, unsigned char * addresses, i2c_port_t i2c_port) {

    // If we have already locked this address for this sensor quit
    if ((address > 0) && (address == _previous_address) && ( _i2c_port == i2c_port )){
        return _previous_address;             
    }
    //Serial.printf("[1] I2CSensor::_begin_i2c() address=0x%02X, i2c_port=%u)\n", _previous_address, _i2c_port);
    // Check if we should release a previously locked address
    if ((_previous_address > 0) && (_previous_address != address)) {
        i2c::master::releaseLock(_i2c_port, _previous_address);
        _previous_address = 0;
        _i2c_port = I2C_NUM_MAX;
    }
    //Serial.printf("[2] I2CSensor::_begin_i2c() address=0x%02X, i2c_port=%u)\n", _previous_address, _i2c_port);
    // If requesting a specific address, try to get a lock to it
    if ((0 < address) && i2c::master::getLock(i2c_port, address)) {
        _previous_address = address;
        _i2c_port = i2c_port;
        //Serial.printf("[3] I2CSensor::_begin_i2c() address=0x%02X, i2c_port=%u)\n", _previous_address, _i2c_port);
        return _previous_address;
    }

    // If everything else fails, perform an auto-discover
    if(i2c::master::device.getState(i2c_port))
    {
        _previous_address = i2c::master::FindAndLock(i2c_port, size, addresses);
        _i2c_port = i2c_port;
    }
    //Serial.printf("[4] I2CSensor::_begin_i2c() address=0x%02X, i2c_port=%u)\n", _previous_address, _i2c_port);
    // Flag error
    if (0 == _previous_address) {
        _error = SENSOR_ERROR_I2C;
    }
    return _previous_address;

}
#endif // SENSOR_SUPPORT && I2C_SUPPORT