#ifndef INC_I2C_COMMON_HPP
#define INC_I2C_COMMON_HPP

#ifdef I2C_SUPPORT

#include "MultiI2C.hpp"

#ifndef I2C_PORT_A
#define I2C_PORT_A  I2C_NUM_0
#endif

#ifndef I2C_PORT_B
#define I2C_PORT_B  I2C_NUM_1
#endif

namespace i2c {
    namespace master {

            extern MultiI2C device;

            esp_err_t write_uint8(uint8_t addr_slave, uint8_t value);
            esp_err_t write_uint8(uint8_t addr_slave, uint8_t reg, uint8_t value);
            esp_err_t write_uint8(i2c_port_t i2c_port, uint8_t addr_slave, uint8_t value);
            esp_err_t write_uint8(i2c_port_t i2c_port, uint8_t addr_slave, uint8_t reg, uint8_t value);

            esp_err_t write_uint16(uint8_t addr_slave, uint16_t value);
            esp_err_t write_uint16(uint8_t addr_slave, uint8_t reg, uint16_t value);
            esp_err_t write_uint16(i2c_port_t i2c_port, uint8_t addr_slave, uint16_t value);
            esp_err_t write_uint16(i2c_port_t i2c_port, uint8_t addr_slave, uint8_t reg, uint8_t value);
            esp_err_t write_blob(uint8_t addr_slave, uint8_t *data_wr, size_t size);
            esp_err_t write_blob(uint8_t addr_slave, uint8_t reg, uint8_t *data_wr, size_t size);
            esp_err_t write_blob(i2c_port_t i2c_port, uint8_t addr_slave, uint8_t *data_wr, size_t size);
            esp_err_t write_blob(i2c_port_t i2c_port, uint8_t addr_slave, uint8_t reg, uint8_t *data_wr, size_t size);

            uint8_t read_uint8(uint8_t addr_slave);
            uint8_t read_uint8(uint8_t addr_slave, uint8_t reg);
            uint8_t read_uint8(i2c_port_t i2c_port, uint8_t addr_slave);
            uint8_t read_uint8(i2c_port_t i2c_port, uint8_t addr_slave, uint8_t reg);
            uint16_t read_uint16(uint8_t addr_slave);
            uint16_t read_uint16(uint8_t addr_slave, uint8_t reg);
            uint16_t read_uint16(i2c_port_t i2c_port, uint8_t addr_slave);
            uint16_t read_uint16(i2c_port_t i2c_port, uint8_t addr_slave, uint8_t reg);
            esp_err_t read_blob(uint8_t addr_slave, uint8_t *data_rd, size_t size);
            esp_err_t read_blob(uint8_t addr_slave, uint8_t reg, uint8_t *data_rd, size_t size);
            esp_err_t read_blob(i2c_port_t i2c_port, uint8_t addr_slave, uint8_t *data_rd, size_t size);
            esp_err_t read_blob(i2c_port_t i2c_port, uint8_t addr_slave, uint8_t reg, uint8_t *data_rd, size_t size);

            esp_err_t init(i2c_port_t i2c_port=I2C_NUM_0);
            
            void clearBus(void);
            void clearBus(i2c_port_t i2c_port);
            bool checkSlave(uint8_t addr_slave);
            bool checkSlave(i2c_port_t i2c_port, uint8_t addr_slave);
            bool getLock(uint8_t addr_slave);
            bool getLock(i2c_port_t i2c_port, uint8_t addr_slave);
            bool releaseLock(uint8_t addr_slave);
            bool releaseLock(i2c_port_t i2c_port, uint8_t addr_slave);

            unsigned char Find(size_t size, unsigned char * addr_slave);
            unsigned char Find(size_t size, unsigned char * addr_slave, unsigned char &start);
            unsigned char Find(i2c_port_t i2c_port, size_t size, unsigned char * addr_slave);
            unsigned char Find(i2c_port_t i2c_port, size_t size, unsigned char * addr_slave, unsigned char &start);

            unsigned char FindAndLock(size_t size, unsigned char * addr_slave);
            unsigned char FindAndLock(i2c_port_t i2c_port, size_t size, unsigned char * addr_slave);
    }
}

#endif /* I2C_SUPPORT */

#endif /* INC_I2C_COMMON_HPP */