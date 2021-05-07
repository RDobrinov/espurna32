#include "config/hardware.h"
#ifdef I2C_SUPPORT
#include "drivers/drv-i2c_master.hpp"
#include "drivers/drv-gpio.h"

namespace i2c {
    namespace master {

        MultiI2C device;

        unsigned int _i2c_locked[I2C_NUM_MAX][16] = {0};

        esp_err_t write_uint8(uint8_t addr_slave, uint8_t value)
        {
            return device.write(addr_slave, &value, 1);    
        }
        esp_err_t write_uint8(uint8_t addr_slave, uint8_t reg, uint8_t value)
        {
            return device.write_reg(addr_slave, reg, &value, 1);
        }
        esp_err_t write_uint8(i2c_port_t i2c_port, uint8_t addr_slave, uint8_t value)
        {
            return device.write(i2c_port, addr_slave, &value, 1);
        }
        esp_err_t write_uint8(i2c_port_t i2c_port, uint8_t addr_slave, uint8_t reg, uint8_t value)
        {
            return device.write_reg(i2c_port, addr_slave, reg, &value, 1);
        }

        esp_err_t write_uint16(uint8_t addr_slave, uint16_t value)
        {
            uint8_t data_wr[2] = {(uint8_t)(value>>8), (uint8_t)(value & 0xFF)};
            return device.write(addr_slave, data_wr, 2);
        }
        esp_err_t write_uint16(uint8_t addr_slave, uint8_t reg, uint16_t value)
        {
            uint8_t data_wr[2] = {(uint8_t)(value>>8), (uint8_t)(value & 0xFF)};
            return device.write_reg(addr_slave, reg, data_wr, 2);
        }
        esp_err_t write_uint16(i2c_port_t i2c_port, uint8_t addr_slave, uint16_t value)
        {
            uint8_t data_wr[2] = {(uint8_t)(value>>8), (uint8_t)(value & 0xFF)};
            return device.write(i2c_port, addr_slave, data_wr, 2);
        }
        esp_err_t write_uint16(i2c_port_t i2c_port, uint8_t addr_slave, uint8_t reg, uint8_t value)
        {
            uint8_t data_wr[2] = {(uint8_t)(value>>8), (uint8_t)(value & 0xFF)};
            return device.write_reg(i2c_port,addr_slave, reg, data_wr, 2);
        }
        esp_err_t write_blob(uint8_t addr_slave, uint8_t *data_wr, size_t size)
        {
            return device.write(addr_slave, data_wr, size);
        }
        esp_err_t write_blob(uint8_t addr_slave, uint8_t reg, uint8_t *data_wr, size_t size)
        {
            return device.write_reg(addr_slave, reg, data_wr, size);
        }
        esp_err_t write_blob(i2c_port_t i2c_port, uint8_t addr_slave, uint8_t *data_wr, size_t size)
        {
            return device.write(i2c_port, addr_slave, data_wr, size);
        }
        esp_err_t write_blob(i2c_port_t i2c_port, uint8_t addr_slave, uint8_t reg, uint8_t *data_wr, size_t size)
        {
            return device.write_reg(i2c_port, addr_slave, reg, data_wr, size);
        }

        uint8_t read_uint8(uint8_t addr_slave)
        {
            uint8_t result;
            device.read(addr_slave, &result, 1);
            return result;
        }
        uint8_t read_uint8(uint8_t addr_slave, uint8_t reg)
        {
            uint8_t result;
            device.read_reg(addr_slave, reg, &result, 1);
            return result;
        }
        uint8_t read_uint8(i2c_port_t i2c_port, uint8_t addr_slave)
        {
            uint8_t result;
            device.read(i2c_port, addr_slave, &result, 1);
            return result;
        }
        uint8_t read_uint8(i2c_port_t i2c_port, uint8_t addr_slave, uint8_t reg)
        {
            uint8_t result;
            device.read_reg(i2c_port, addr_slave, reg, &result, 1);
            return result;
        }

        uint16_t read_uint16(uint8_t addr_slave)
        {
            uint8_t result[2];
            device.read(addr_slave, result, 2);
            return (uint16_t)result[0]<<8 | (uint16_t)result[1];
        }
        uint16_t read_uint16(uint8_t addr_slave, uint8_t reg)
        {
            uint8_t result[2];
            device.read_reg(addr_slave, reg, result, 2);
            return (uint16_t)result[0]<<8 | (uint16_t)result[1];
        }
        uint16_t read_uint16(i2c_port_t i2c_port, uint8_t addr_slave)
        {
            uint8_t result[2];
            device.read(i2c_port, addr_slave, result, 2);
            return (uint16_t)result[0]<<8 | (uint16_t)result[1];
        }
        uint16_t read_uint16(i2c_port_t i2c_port, uint8_t addr_slave, uint8_t reg)
        {
            uint8_t result[2];
            device.read_reg(i2c_port, addr_slave, reg, result, 2);
            return (uint16_t)result[0]<<8 | (uint16_t)result[1];
        }

        esp_err_t read_blob(uint8_t addr_slave, uint8_t *data_rd, size_t size)
        {
            return device.read(addr_slave, data_rd, size);
        }
        esp_err_t read_blob(uint8_t addr_slave, uint8_t reg, uint8_t *data_rd, size_t size)
        {
            return device.read_reg(addr_slave, reg, data_rd, size);
        }
        esp_err_t read_blob(i2c_port_t i2c_port, uint8_t addr_slave, uint8_t *data_rd, size_t size)
        {
            return device.read(i2c_port, addr_slave, data_rd, size);
        }
        esp_err_t read_blob(i2c_port_t i2c_port, uint8_t addr_slave, uint8_t reg, uint8_t *data_rd, size_t size)
        {
            return device.read_reg(i2c_port, addr_slave, reg, data_rd, size);
        }

        esp_err_t init(i2c_port_t i2c_port)
        {
            port_config_t port_state = device.getConfig(i2c_port);
            if( gpioGetLock( port_state.port_config.scl_io_num, "I2C_CLK") && gpioGetLock( port_state.port_config.sda_io_num, "I2C_SDA"))
                if( device.init(i2c_port) )
                    return ESP_OK;
            gpioReleaseLock(port_state.port_config.scl_io_num);
            gpioReleaseLock(port_state.port_config.sda_io_num);
            return ESP_ERR_INVALID_ARG;
        }

        // Address locks and release
        void clearBus(void)
        {
            device.clearBus();
        }
        void clearBus(i2c_port_t i2c_port)
        {
            device.clearBus(i2c_port);
        }
        bool checkSlave(uint8_t addr_slave)
        {
            return device.checkSlave(addr_slave);
        }
        bool checkSlave(i2c_port_t i2c_port, uint8_t addr_slave)
        {
            return device.checkSlave(i2c_port, addr_slave);
        }
        bool getLock(uint8_t addr_slave)
        {
            if(device.getState(I2C_NUM_0))
                return getLock(I2C_NUM_0, addr_slave);
            else
                if(device.getState(I2C_NUM_1))
                    return getLock(I2C_NUM_1, addr_slave);
            return false;
        }
        bool getLock(i2c_port_t i2c_port, uint8_t addr_slave)
        {
            unsigned char index = addr_slave / 8;
            unsigned char mask = 1 << (addr_slave % 8);
            if (_i2c_locked[i2c_port][index] & mask) return false;
            _i2c_locked[i2c_port][index] = _i2c_locked[i2c_port][index] | mask;
            return true;
        }
        bool releaseLock(uint8_t addr_slave)
        {
            if(device.getState(I2C_NUM_0))
                return releaseLock(I2C_NUM_0, addr_slave);
            else
                if(device.getState(I2C_NUM_1))
                    return releaseLock(I2C_NUM_1, addr_slave);
            return false;
        }
        bool releaseLock(i2c_port_t i2c_port, uint8_t addr_slave)
        {
            unsigned char index = addr_slave / 8;
            unsigned char mask = 1 << (addr_slave % 8);
            if (_i2c_locked[i2c_port][index] & mask) 
            {
                _i2c_locked[i2c_port][index] = _i2c_locked[i2c_port][index] & ~mask;
                return true;
            }
            return false;
        }

        unsigned char Find(size_t size, unsigned char * addr_slave)
        {
            unsigned char start = 0;
            return Find(size, addr_slave, start);
        }

        unsigned char Find(size_t size, unsigned char * addr_slave, unsigned char &start)
        {
            for (unsigned char i=start; i<size; i++) {
                if (device.checkSlave(addr_slave[i])) 
                {
                    start = i;
                    return addr_slave[i];
                }
            }
            return 0;
        }

        unsigned char Find(i2c_port_t i2c_port, size_t size, unsigned char * addr_slave)
        {
            unsigned char start = 0;
            return Find(i2c_port, size, addr_slave, start);
        }

        unsigned char Find(i2c_port_t i2c_port, size_t size, unsigned char * addr_slave, unsigned char &start)
        {
            for (unsigned char i=start; i<size; i++) {
                if (device.checkSlave(i2c_port, addr_slave[i])) 
                {
                    start = i;
                    return addr_slave[i];
                }
            }
            return 0;
        }

        unsigned char FindAndLock(size_t size, unsigned char * addr_slave)
        {
            if(device.getState(I2C_NUM_0))
                return FindAndLock(I2C_NUM_0, size, addr_slave);
            else
                if(device.getState(I2C_NUM_1))
                    return FindAndLock(I2C_NUM_1, size, addr_slave);
            return false;
        }

        unsigned char FindAndLock(i2c_port_t i2c_port, size_t size, unsigned char * addr_slave)
        {
            unsigned char start = 0;
            unsigned char address = 0;
            while ((address = Find(i2c_port, size, addr_slave, start))) 
            {
                Serial.printf("FindAndLock: Port%u, %#02X", i2c_port, address);
                if (getLock(i2c_port, address)) break;
                start++;
            }
            return address;    
        }
    }
}

#endif /* I2C_SUPPORT */