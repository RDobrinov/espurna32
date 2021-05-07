
#ifndef INC_MULTII2C_HPP
#define INC_MULTII2C_HPP

#include "driver/i2c.h"

#define _I2C_NUMBER(num) I2C_NUM_##num
#define I2C_NUMBER(num) _I2C_NUMBER(num)

#ifndef CONFIG_I2C_SCL_0
#define CONFIG_I2C_SCL_0        GPIO_NUM_22         
#endif

#ifndef CONFIG_I2C_SDA_0
#define CONFIG_I2C_SDA_0        GPIO_NUM_21
#endif

#ifndef CONFIG_I2C_SCL_1
#define CONFIG_I2C_SCL_1        GPIO_NUM_26
#endif

#ifndef CONFIG_I2C_SDA_1
#define CONFIG_I2C_SDA_1        GPIO_NUM_18
#endif

#ifndef CONFIG_I2C_DEFAULT_PORT_NUM
#define CONFIG_I2C_DEFAULT_PORT_NUM I2C_NUM_0
#endif

#ifndef CONFIG_I2C_DEFAULT_FREQUENCY
#define CONFIG_I2C_DEFAULT_FREQUENCY 500000
#endif

#ifndef CONFIG_I2C_DEFAULT_TIMEOUT
#define CONFIG_I2C_DEFAULT_TIMEOUT  1000
#endif

#define I2C_DEFAULT_PORT_NUM        CONFIG_I2C_DEFAULT_PORT_NUM                 /* I2C port number for master dev */

#define I2C_SCL_IO_0                CONFIG_I2C_SCL_0                    /* gpio number for I2C master clock */
#define I2C_SDA_IO_0                CONFIG_I2C_SDA_0                    /* gpio number for I2C master data  */
#define I2C_SCL_IO_1                CONFIG_I2C_SCL_1                    /* gpio number for I2C master clock */
#define I2C_SDA_IO_1                CONFIG_I2C_SDA_1                    /* gpio number for I2C master data  */
#define I2C_PORT0_FREQ_HZ           CONFIG_I2C_DEFAULT_FREQUENCY        /* I2C master clock frequency */
#define I2C_PORT1_FREQ_HZ           CONFIG_I2C_DEFAULT_FREQUENCY        /* I2C master clock frequency */
#define I2C_PORT0_BUS_TIMEOUT       CONFIG_I2C_DEFAULT_TIMEOUT
#define I2C_PORT1_BUS_TIMEOUT       CONFIG_I2C_DEFAULT_TIMEOUT

#define I2C_TX_BUF_DISABLE          0           /* I2C master doesn't need buffer */
#define I2C_RX_BUF_DISABLE          0           /* I2C master doesn't need buffer */
#define WRITE_BIT I2C_MASTER_WRITE              /* I2C master write */
#define READ_BIT I2C_MASTER_READ                /* I2C master read */
#define ACK_CHECK_EN                0x1         /* I2C master will check ack from slave */
#define ACK_CHECK_DIS               0x0         /* I2C master will not check ack from slave */
#define ACK_VAL                     0x0         /* I2C ack value */
#define NACK_VAL                    0x1         /* I2C nack value */

#ifndef I2C_SCAN_ON_STARTUP
#define I2C_SCAN_ON_STARTUP         0x0
#endif

typedef struct
{
    bool port_enabled = false;
    bool port_activated = false;
    i2c_port_t i2c_port;
    i2c_config_t port_config;    
} port_config_t;

typedef struct 
{
    port_config_t port[I2C_NUM_MAX];
} driver_state_t;


class MultiI2C
{
    public:
        MultiI2C(void);
        ~MultiI2C(void);

        void setConfiguration(i2c_config_t &port_config);
        void setConfiguration(i2c_port_t i2c_port, i2c_config_t &port_config);
        void setIOPins(gpio_num_t scl_io_num, gpio_num_t sda_io_num);
        void setIOPins(i2c_port_t i2c_port, gpio_num_t scl_io_num, gpio_num_t sda_io_num);
        void setPortFrequency(unsigned int port_freq);
        void setPortFrequency(i2c_port_t i2c_port, unsigned int port_freq);
        bool init(void);
        bool init(i2c_port_t i2c_port);
        esp_err_t unload(i2c_port_t i2c_port);
        void begin(void);

        esp_err_t read(uint8_t addr_slave, uint8_t *data_rd, size_t size);
        esp_err_t read(i2c_port_t i2c_port, uint8_t addr_slave, uint8_t *data_rd, size_t size);
        esp_err_t read(i2c_port_t i2c_port, uint8_t addr_slave, uint8_t *data_wr, size_t size_wr, uint8_t *data_rd, size_t size_rd);
        esp_err_t read_reg(uint8_t addr_slave, uint8_t reg, uint8_t *data_rd, size_t size);
        esp_err_t read_reg(i2c_port_t i2c_port, uint8_t addr_slave, uint8_t reg, uint8_t *data_rd, size_t size);

        esp_err_t write(uint8_t addr_slave, uint8_t *data_wr, size_t size);
        esp_err_t write(i2c_port_t i2c_port, uint8_t addr_slave, uint8_t *data_wr, size_t size);
        esp_err_t write_reg(uint8_t addr_slave, uint8_t reg, uint8_t *data_wr, size_t size);
        esp_err_t write_reg(i2c_port_t i2c_port, uint8_t addr_slave, uint8_t reg, uint8_t *data_wr, size_t size);

        bool checkSlave(uint8_t addr_slave);
        bool checkSlave(i2c_port_t i2c_port, uint8_t addr_slave);
        bool clearBus(void);
        bool clearBus(i2c_port_t i2c_port);

        bool getState(i2c_port_t i2c_port);
        port_config_t getConfig(i2c_port_t i2c_port);

    private:

        void _config_driver(i2c_port_t i2c_port);
        void _init_driver(void);

        driver_state_t _state;
};

#endif /* INC_MULTII2C_HPP */