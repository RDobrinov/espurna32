// -----------------------------------------------------------------------------
// BME280/BMP280 Sensor over I2C
// Copyright (C) 2017-2019 by Xose Pérez <xose dot perez at gmail dot com>
// -----------------------------------------------------------------------------
#include "config/hardware.h"
#if SENSOR_SUPPORT && BMX280_SUPPORT

#include <byteswap.h>
#include "drivers/sensors/BMX280Sensor.hpp"
#include "drivers/sensors/SensorsConfig.hpp"
#include "drivers/drv-i2c_master.hpp"

BMX280Sensor::BMX280Sensor(): I2CSensor() {
    _sensor_id = SENSOR_BMX280_ID;
}

// ---------------------------------------------------------------------
// Sensor API
// ---------------------------------------------------------------------

// Initialization method, must be idempotent
void BMX280Sensor::begin() {
    if (!_dirty) return;
    if(_i2c_port == I2C_NUM_MAX) _i2c_port = I2C_NUM_0;
    _init(_i2c_port);
    _dirty = !_ready;
}

// Descriptive name of the sensor
String BMX280Sensor::description() {
    char buffer[22];
    snprintf(buffer, sizeof(buffer), "%s @ I2C%u (0x%02X)", _chip == BMX280_CHIP_BME280 ? "BME280" : "BMP280", _i2c_port, _address);
    return String(buffer);
}

// Type for slot # index
unsigned char BMX280Sensor::type(unsigned char index) {
    unsigned char i = 0;
    #if BMX280_TEMPERATURE > 0
        if (index == i++) return MAGNITUDE_TEMPERATURE;
    #endif
    #if BMX280_PRESSURE > 0
        if (index == i++) return MAGNITUDE_PRESSURE;
    #endif
    #if BMX280_HUMIDITY > 0
        if (_chip == BMX280_CHIP_BME280) {
            if (index == i) return MAGNITUDE_HUMIDITY;
        }
    #endif
    return MAGNITUDE_NONE;
}
// Number of decimals for a magnitude (or -1 for default)
signed char BMX280Sensor::decimals(unsigned char type) { 
    // These numbers of decimals correspond to maximum sensor resolution settings
    switch (type) {  
        case MAGNITUDE_TEMPERATURE: return 3;
        case MAGNITUDE_PRESSURE:    return 4;
        case MAGNITUDE_HUMIDITY:    return 2;
    }
    return -1;
}

// Pre-read hook (usually to populate registers with up-to-date data)
void BMX280Sensor::pre() {

    if (_run_init) {
        //i2cClearBus();    //Clear bus? ???WHY???
        _init(_i2c_port);
    }

    if (_chip == 0) {
        _error = SENSOR_ERROR_UNKNOWN_ID;
        return;
    }
    _error = SENSOR_ERROR_OK;

    #if BMX280_MODE == 1
        _forceRead();
    #endif

    _error = _read();

    if (_error != SENSOR_ERROR_OK) {
        _run_init = true;
    }

}

// Current value for slot # index
double BMX280Sensor::value(unsigned char index) {
    unsigned char i = 0;
    #if BMX280_TEMPERATURE > 0
        if (index == i++) return _temperature;
    #endif
    #if BMX280_PRESSURE > 0
        if (index == i++) return _pressure / 100;
    #endif
    #if BMX280_HUMIDITY > 0
        if (_chip == BMX280_CHIP_BME280) {
            if (index == i) return _humidity;
        }
    #endif
    return 0;
}

// Load the configuration manifest
void BMX280Sensor::manifest(JsonArray& sensors) {

    char buffer[10];

    JsonObject& sensor = sensors.createNestedObject();
    sensor["sensor_id"] = SENSOR_BMX280_ID;
    JsonArray& fields = sensor.createNestedArray("fields");

    {
        JsonObject& field = fields.createNestedObject();
        field["tag"] = UI_TAG_SELECT;
        field["name"] = "address";
        field["label"] = "Address";
        JsonArray& options = field.createNestedArray("options");
        {
            JsonObject& option = options.createNestedObject();
            option["name"] = "auto";
            option["value"] = 0;
        }
        for (unsigned char i=0; i< sizeof(BMX280Sensor::addresses); i++) {
            JsonObject& option = options.createNestedObject();
            snprintf(buffer, sizeof(buffer), "0x%02X", BMX280Sensor::addresses[i]);
            option["name"] = String(buffer);
            option["value"] = BMX280Sensor::addresses[i];
        }
    }

};

void BMX280Sensor::getConfig(JsonObject& root) {
    root["sensor_id"] = _sensor_id;
    root["address"] = _address;
};

void BMX280Sensor::setConfig(JsonObject& root) {
    if (root.containsKey("address")) setAddress(root["address"]);
};

void BMX280Sensor::_init(i2c_port_t i2c_port) {

    // Make sure sensor had enough time to turn on. BMX280 requires 2ms to start up
    vTaskDelay(10/portTICK_PERIOD_MS);
    // No chip ID by default
    _chip = 0;

    // I2C auto-discover
    _address = _begin_i2c(_address, sizeof(BMX280Sensor::addresses), BMX280Sensor::addresses, i2c_port);
    if (_address == 0) return;
    // Check sensor correctly initialized
    _chip = i2c::master::read_uint8(_address, BMX280_REGISTER_CHIPID);
    if ((_chip != BMX280_CHIP_BME280) && (_chip != BMX280_CHIP_BMP280)) {

        _chip = 0;
        i2c::master::releaseLock(_address);
        _previous_address = 0;
        _error = SENSOR_ERROR_UNKNOWN_ID;

        // Setting _address to 0 forces auto-discover
        // This might be necessary at this stage if there is a
        // different sensor in the hardcoded address
        _address = 0;

        return;
    }

    _count = 0;
    #if BMX280_TEMPERATURE > 0
        ++_count;
    #endif
    #if BMX280_PRESSURE > 0
        ++_count;
    #endif
    #if BMX280_HUMIDITY > 0
        if (_chip == BMX280_CHIP_BME280) ++_count;
    #endif

    _readCoefficients();

    unsigned char data = 0;
    i2c::master::write_uint8(_address, BMX280_REGISTER_CONTROL, data);

    data =  (BMX280_STANDBY << 0x5) & 0xE0;
    data |= (BMX280_FILTER << 0x02) & 0x1C;
    i2c::master::write_uint8(_address, BMX280_REGISTER_CONFIG, data);

    data =  (BMX280_HUMIDITY) & 0x07;
    i2c::master::write_uint8(_address, BMX280_REGISTER_CONTROLHUMID, data);

    data =  (BMX280_TEMPERATURE << 5) & 0xE0;
    data |= (BMX280_PRESSURE << 2) & 0x1C;
    data |= (BMX280_MODE) & 0x03;
    i2c::master::write_uint8(_address, BMX280_REGISTER_CONTROL, data);

    _measurement_delay = _measurementTime();
    _run_init = false;
    _ready = true;
}
    //uint16_t swapped = __bswap_16(bswap);
void BMX280Sensor::_readCoefficients() {
    _bmx280_calib.dig_T1 = __bswap_16(i2c::master::read_uint16(_address, BMX280_REGISTER_DIG_T1));
    _bmx280_calib.dig_T2 = (short) __bswap_16(i2c::master::read_uint16(_address, BMX280_REGISTER_DIG_T2));
    _bmx280_calib.dig_T3 = (short) __bswap_16(i2c::master::read_uint16(_address, BMX280_REGISTER_DIG_T3));

    _bmx280_calib.dig_P1 = __bswap_16(i2c::master::read_uint16(_address, BMX280_REGISTER_DIG_P1));
    _bmx280_calib.dig_P2 = (short) __bswap_16(i2c::master::read_uint16(_address, BMX280_REGISTER_DIG_P2));
    _bmx280_calib.dig_P3 = (short) __bswap_16(i2c::master::read_uint16(_address, BMX280_REGISTER_DIG_P3));
    _bmx280_calib.dig_P4 = (short) __bswap_16(i2c::master::read_uint16(_address, BMX280_REGISTER_DIG_P4));
    _bmx280_calib.dig_P5 = (short) __bswap_16(i2c::master::read_uint16(_address, BMX280_REGISTER_DIG_P5));
    _bmx280_calib.dig_P6 = (short) __bswap_16(i2c::master::read_uint16(_address, BMX280_REGISTER_DIG_P6));
    _bmx280_calib.dig_P7 = (short) __bswap_16(i2c::master::read_uint16(_address, BMX280_REGISTER_DIG_P7));
    _bmx280_calib.dig_P8 = (short) __bswap_16(i2c::master::read_uint16(_address, BMX280_REGISTER_DIG_P8));
    _bmx280_calib.dig_P9 = (short) __bswap_16(i2c::master::read_uint16(_address, BMX280_REGISTER_DIG_P9));

    _bmx280_calib.dig_H1 = i2c::master::read_uint8(_address, BMX280_REGISTER_DIG_H1);
    _bmx280_calib.dig_H2 = (short) __bswap_16(i2c::master::read_uint16(_address, BMX280_REGISTER_DIG_H2));
    _bmx280_calib.dig_H3 = i2c::master::read_uint8(_address, BMX280_REGISTER_DIG_H3);
    _bmx280_calib.dig_H4 = (short)((uint16_t) i2c::master::read_uint8(_address, BMX280_REGISTER_DIG_H4) << 4) | (i2c::master::read_uint8(_address, BMX280_REGISTER_DIG_H4+1) & 0x0F);
    _bmx280_calib.dig_H5 = (short)((uint16_t)i2c::master::read_uint8(_address, BMX280_REGISTER_DIG_H5+1) << 4) | (i2c::master::read_uint8(_address, BMX280_REGISTER_DIG_H5) >> 4);
    _bmx280_calib.dig_H6 = (int8_t) i2c::master::read_uint8(_address, BMX280_REGISTER_DIG_H6);
}

unsigned long BMX280Sensor::_measurementTime() {

    // Measurement Time (as per BMX280 datasheet section 9.1)
    // T_max(ms) = 1.25
    //  + (2.3 * T_oversampling)
    //  + (2.3 * P_oversampling + 0.575)
    //  + (2.4 * H_oversampling + 0.575)
    //  ~ 9.3ms for current settings

    double t = 1.25;
    #if BMX280_TEMPERATURE > 0
        t += (2.3 * BMX280_TEMPERATURE);
    #endif
    #if BMX280_PRESSURE > 0
        t += (2.3 * BMX280_PRESSURE + 0.575);
    #endif
    #if BMX280_HUMIDITY > 0
        if (_chip == BMX280_CHIP_BME280) {
            t += (2.4 * BMX280_HUMIDITY + 0.575);
        }
    #endif

    return round(t + 1); // round up

}

void BMX280Sensor::_forceRead() {

    // We set the sensor in "forced mode" to force a reading.
    // After the reading the sensor will go back to sleep mode.
    uint8_t value = i2c::master::read_uint8(_address, BMX280_REGISTER_CONTROL);
    value = (value & 0xFC) + 0x01;
    i2c::master::write_uint8(_address, BMX280_REGISTER_CONTROL, value);

    delay(_measurement_delay);

}

unsigned char BMX280Sensor::_read() {

    #if BMX280_TEMPERATURE > 0
        int32_t adc_T = i2c::master::read_uint16(_address, BMX280_REGISTER_TEMPDATA);
        if (0xFFFF == adc_T) return SENSOR_ERROR_OUT_OF_RANGE;
        adc_T <<= 8;
        adc_T |= i2c::master::read_uint8(_address, BMX280_REGISTER_TEMPDATA+2);
        adc_T >>= 4;

        int32_t var1t = ((((adc_T>>3) -
            ((int32_t)_bmx280_calib.dig_T1 <<1))) *
            ((int32_t)_bmx280_calib.dig_T2)) >> 11;

        int32_t var2t = (((((adc_T>>4) -
            ((int32_t)_bmx280_calib.dig_T1)) *
            ((adc_T>>4) - ((int32_t)_bmx280_calib.dig_T1))) >> 12) *
            ((int32_t)_bmx280_calib.dig_T3)) >> 14;

        int32_t t_fine = var1t + var2t;

        double T  = (t_fine * 5 + 128) >> 8;
        _temperature = T / 100;
    #else
        int32_t t_fine = 102374; // ~20ºC
    #endif

    // -----------------------------------------------------------------

    #if BMX280_PRESSURE > 0
        int64_t var1, var2, p;

        int32_t adc_P = i2c::master::read_uint16(_address, BMX280_REGISTER_PRESSUREDATA);
        if (0xFFFF == adc_P) return SENSOR_ERROR_OUT_OF_RANGE;
        adc_P <<= 8;
        adc_P |= i2c::master::read_uint8(_address, BMX280_REGISTER_PRESSUREDATA+2);
        adc_P >>= 4;

        var1 = ((int64_t)t_fine) - 128000;
        var2 = var1 * var1 * (int64_t)_bmx280_calib.dig_P6;
        var2 = var2 + ((var1*(int64_t)_bmx280_calib.dig_P5)<<17);
        var2 = var2 + (((int64_t)_bmx280_calib.dig_P4)<<35);
        var1 = ((var1 * var1 * (int64_t)_bmx280_calib.dig_P3)>>8) +
            ((var1 * (int64_t)_bmx280_calib.dig_P2)<<12);
        var1 = (((((int64_t)1)<<47)+var1))*((int64_t)_bmx280_calib.dig_P1)>>33;
        if (var1 == 0) return SENSOR_ERROR_OUT_OF_RANGE;  // avoid exception caused by division by zero

        p = 1048576 - adc_P;
        p = (((p<<31) - var2)*3125) / var1;
        var1 = (((int64_t)_bmx280_calib.dig_P9) * (p>>13) * (p>>13)) >> 25;
        var2 = (((int64_t)_bmx280_calib.dig_P8) * p) >> 19;

        p = ((p + var1 + var2) >> 8) + (((int64_t)_bmx280_calib.dig_P7)<<4);
        _pressure = (double) p / 256;
    #endif

    // -----------------------------------------------------------------

    #if BMX280_HUMIDITY > 0
    if (_chip == BMX280_CHIP_BME280) {

        int32_t adc_H = i2c::master::read_uint16(_address, BMX280_REGISTER_HUMIDDATA);
        if (0xFFFF == adc_H) return SENSOR_ERROR_OUT_OF_RANGE;

        int32_t v_x1_u32r;

        v_x1_u32r = (t_fine - ((int32_t)76800));

        v_x1_u32r = (((((adc_H << 14) - (((int32_t)_bmx280_calib.dig_H4) << 20) -
            (((int32_t)_bmx280_calib.dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) *
            (((((((v_x1_u32r * ((int32_t)_bmx280_calib.dig_H6)) >> 10) *
            (((v_x1_u32r * ((int32_t)_bmx280_calib.dig_H3)) >> 11) + ((int32_t)32768))) >> 10) +
            ((int32_t)2097152)) * ((int32_t)_bmx280_calib.dig_H2) + 8192) >> 14));

        v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
            ((int32_t)_bmx280_calib.dig_H1)) >> 4));

        v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
        v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;
        double h = (v_x1_u32r >> 12);
        _humidity = h / 1024.0;

    }
    #endif

    return SENSOR_ERROR_OK;

}

unsigned char BMX280Sensor::addresses[2] = {0x76, 0x77};

#endif // SENSOR_SUPPORT && BMX280_SUPPORT
