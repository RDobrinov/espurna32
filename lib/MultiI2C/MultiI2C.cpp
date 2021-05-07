#include "MultiI2C.hpp"
#include <Arduino.h>

MultiI2C::MultiI2C(void)
{
    _state.port[I2C_NUM_0].port_config.mode = I2C_MODE_MASTER;
    _state.port[I2C_NUM_0].port_config.sda_io_num = I2C_SDA_IO_0;
    _state.port[I2C_NUM_0].port_config.scl_io_num = I2C_SCL_IO_0;
    _state.port[I2C_NUM_0].port_config.sda_pullup_en = GPIO_PULLUP_ENABLE;
    _state.port[I2C_NUM_0].port_config.scl_pullup_en = GPIO_PULLUP_ENABLE;
    _state.port[I2C_NUM_0].port_config.master.clk_speed = I2C_PORT0_FREQ_HZ;
    _state.port[I2C_NUM_0].port_enabled = false;

    _state.port[I2C_NUM_1].port_config.mode = I2C_MODE_MASTER;
    _state.port[I2C_NUM_1].port_config.sda_io_num = I2C_SDA_IO_1;
    _state.port[I2C_NUM_1].port_config.scl_io_num = I2C_SCL_IO_1;
    _state.port[I2C_NUM_1].port_config.sda_pullup_en = GPIO_PULLUP_ENABLE;
    _state.port[I2C_NUM_1].port_config.scl_pullup_en = GPIO_PULLUP_ENABLE;
    _state.port[I2C_NUM_1].port_config.master.clk_speed = I2C_PORT1_FREQ_HZ;
    _state.port[I2C_NUM_1].port_enabled = false;
}

MultiI2C::~MultiI2C(void)
{
    if(_state.port[I2C_NUM_0].port_activated)
        i2c_driver_delete(I2C_NUM_0);
    if(_state.port[I2C_NUM_1].port_activated)
        i2c_driver_delete(I2C_NUM_1);
}

void MultiI2C::setConfiguration(i2c_config_t &port_config)
{
    setConfiguration(I2C_NUM_0, port_config);
}

void MultiI2C::setConfiguration(i2c_port_t i2c_port, i2c_config_t &port_config)
{
    _state.port[i2c_port].port_config = port_config;
}

void MultiI2C::setIOPins(gpio_num_t scl_io_num, gpio_num_t sda_io_num)
{
    setIOPins(I2C_NUM_0, scl_io_num, sda_io_num);
}

void MultiI2C::setIOPins(i2c_port_t i2c_port, gpio_num_t scl_io_num, gpio_num_t sda_io_num)
{
    _state.port[i2c_port].port_config.sda_io_num = sda_io_num;
    _state.port[i2c_port].port_config.scl_io_num = scl_io_num;
}

void MultiI2C::setPortFrequency(unsigned int port_freq)
{
    setPortFrequency(I2C_NUM_0, port_freq);
}

void MultiI2C::setPortFrequency(i2c_port_t i2c_port, unsigned int port_freq)
{
    _state.port[i2c_port].port_config.master.clk_speed = port_freq;
}

bool MultiI2C::init(void)
{
    return init(I2C_NUM_0);
}
bool MultiI2C::init(i2c_port_t i2c_port)
{
    _config_driver(i2c_port);
    _init_driver();
    #if I2C_SCAN_ON_STARTUP
    i2c_cmd_handle_t cmd;
    if(_state.port[i2c_port].port_activated)
        for( uint8_t addr_scan = 8; addr_scan<121; addr_scan++)
        {
            cmd = i2c_cmd_link_create();
            i2c_master_start(cmd);
            i2c_master_write_byte(cmd, (addr_scan << 1) | READ_BIT, ACK_CHECK_EN );
            i2c_master_stop(cmd);
            if(ESP_OK == i2c_master_cmd_begin(i2c_port, cmd, (i2c_port == I2C_NUM_0) ? I2C_PORT0_BUS_TIMEOUT : I2C_PORT1_BUS_TIMEOUT / portTICK_RATE_MS))
                Serial.printf("I2C Address 0x%2X found on I2C_NUM_%u\n", addr_scan, i2c_port);
            i2c_cmd_link_delete(cmd);    
        };
    #endif
    if(_state.port[i2c_port].port_activated) i2c_set_timeout(i2c_port, 0xFFFFF);
    return _state.port[i2c_port].port_activated;
    /*return ( 
        (_state.port[I2C_NUM_0].port_activated && _state.port[I2C_NUM_1].port_activated) ||
        ( _state.port[I2C_NUM_0].port_activated && !_state.port[I2C_NUM_1].port_enabled) ||
        ( _state.port[I2C_NUM_1].port_activated && !_state.port[I2C_NUM_0].port_enabled) ? ESP_OK : ESP_FAIL); */
}

void MultiI2C::begin(void)
{
    init(I2C_NUM_0);
}

esp_err_t MultiI2C::unload(i2c_port_t i2c_port)
{
    esp_err_t result = ESP_OK;
    if(_state.port[i2c_port].port_activated)
        result = i2c_driver_delete(i2c_port);
    _state.port[i2c_port].port_activated = (ESP_OK != result );
    return result;
}

esp_err_t MultiI2C::read(uint8_t addr_slave, uint8_t *data_rd, size_t size)
{
    if(_state.port[I2C_NUM_0].port_activated)
        return read(I2C_NUM_0, addr_slave, nullptr, 0, data_rd, size);
    else
        if(_state.port[I2C_NUM_1].port_activated)
            return read(I2C_NUM_1, addr_slave, nullptr, 0, data_rd, size);
    return ESP_FAIL;
}
esp_err_t MultiI2C::read(i2c_port_t i2c_port, uint8_t addr_slave, uint8_t *data_rd, size_t size)
{
    return read(i2c_port, addr_slave, nullptr, 0, data_rd, size);
}
esp_err_t MultiI2C::read(i2c_port_t i2c_port, uint8_t addr_slave, uint8_t *data_wr, size_t size_wr, uint8_t *data_rd, size_t size_rd)
{
    if (size_rd == 0) 
        {
            return ESP_OK;
        }
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        if(( data_wr != nullptr ) && (size_wr > 0) )
        {
            i2c_master_write_byte(cmd, (addr_slave << 1) | WRITE_BIT, ACK_CHECK_EN );
            i2c_master_write(cmd, data_wr, size_wr, (i2c_ack_type_t) ACK_CHECK_EN);
            i2c_master_start(cmd);
        }
        i2c_master_write_byte(cmd, (addr_slave << 1) | READ_BIT, ACK_CHECK_EN );
        if (size_rd > 1) 
            i2c_master_read(cmd, data_rd, size_rd - 1, (i2c_ack_type_t) ACK_VAL);
        i2c_master_read_byte(cmd, data_rd + size_rd - 1, (i2c_ack_type_t) NACK_VAL);
        i2c_master_stop(cmd);
        esp_err_t result = i2c_master_cmd_begin(i2c_port, cmd, (i2c_port == I2C_NUM_0) ? I2C_PORT0_BUS_TIMEOUT : I2C_PORT1_BUS_TIMEOUT / portTICK_RATE_MS);
        //if( ESP_OK != result )
        //    Serial.println("i2c_master::read() error");
        i2c_cmd_link_delete(cmd);
        return result;        
}

esp_err_t MultiI2C::read_reg(uint8_t addr_slave, uint8_t reg, uint8_t *data_rd, size_t size)
{
    if(_state.port[I2C_NUM_0].port_activated)
        return read(I2C_NUM_0, addr_slave, &reg, 1, data_rd, size);
    else
        if(_state.port[I2C_NUM_1].port_activated)
            return read(I2C_NUM_1, addr_slave, &reg, 1, data_rd, size);
    return ESP_FAIL;
}
esp_err_t MultiI2C::read_reg(i2c_port_t i2c_port, uint8_t addr_slave, uint8_t reg, uint8_t *data_rd, size_t size)
{
    return read(i2c_port, addr_slave, &reg, 1, data_rd, size);
}

esp_err_t MultiI2C::write(uint8_t addr_slave, uint8_t *data_wr, size_t size)
{
    if(_state.port[I2C_NUM_0].port_activated)
        return write(I2C_NUM_0, addr_slave, data_wr, size);
    else
        if(_state.port[I2C_NUM_1].port_activated)
            return write(I2C_NUM_1, addr_slave, data_wr, size);
    return ESP_FAIL;
}
esp_err_t MultiI2C::write(i2c_port_t i2c_port, uint8_t addr_slave, uint8_t *data_wr, size_t size)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (addr_slave << 1) | WRITE_BIT, ACK_CHECK_EN );
    i2c_master_write(cmd, data_wr, size, (i2c_ack_type_t) ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t result = i2c_master_cmd_begin(i2c_port, cmd, (i2c_port == I2C_NUM_0) ? I2C_PORT0_BUS_TIMEOUT : I2C_PORT1_BUS_TIMEOUT / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    //if( ESP_OK != result )
    //    Serial.println("MultiI2C::write() error");
    return result;
}

esp_err_t MultiI2C::write_reg(uint8_t addr_slave, uint8_t reg, uint8_t *data_wr, size_t size)
{
    if(_state.port[I2C_NUM_0].port_activated)
        return write_reg(I2C_NUM_0, addr_slave, reg, data_wr, size);
    else
        if(_state.port[I2C_NUM_1].port_activated)
            return write_reg(I2C_NUM_1, addr_slave, reg, data_wr, size);
    return ESP_FAIL;
}

esp_err_t MultiI2C::write_reg(i2c_port_t i2c_port, uint8_t addr_slave, uint8_t reg, uint8_t *data_wr, size_t size)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (addr_slave << 1) | WRITE_BIT, ACK_CHECK_EN );
    i2c_master_write_byte(cmd, reg, ACK_CHECK_EN);
    i2c_master_write(cmd, data_wr, size, (i2c_ack_type_t) ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t result = i2c_master_cmd_begin(i2c_port, cmd, (i2c_port == I2C_NUM_0) ? I2C_PORT0_BUS_TIMEOUT : I2C_PORT1_BUS_TIMEOUT / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    //if( ESP_OK != result )
    //    Serial.println("MultiI2C::write_reg() error");
    return result;        
}

bool MultiI2C::checkSlave(uint8_t addr_slave)
{
    if(_state.port[I2C_NUM_0].port_activated)
        return checkSlave(I2C_NUM_0, addr_slave);
    else
        if(_state.port[I2C_NUM_1].port_activated)
            return checkSlave(I2C_NUM_1, addr_slave);
    return false;        
}

bool MultiI2C::checkSlave(i2c_port_t i2c_port, uint8_t addr_slave)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (addr_slave << 1) | READ_BIT, ACK_CHECK_EN );
    i2c_master_stop(cmd);
    esp_err_t result = i2c_master_cmd_begin(i2c_port, cmd, (i2c_port == I2C_NUM_0) ? I2C_PORT0_BUS_TIMEOUT : I2C_PORT1_BUS_TIMEOUT / portTICK_RATE_MS);
    //Serial.printf("MultiI2C::checkSlave(Port%u, 0x%2X)=%s, ESP_ERR=%u\n", i2c_port, addr_slave, (ESP_OK == result)?"true":"false", result);
    i2c_cmd_link_delete(cmd);
    return (ESP_OK == result);    
}

bool MultiI2C::clearBus(void)
{
    if(_state.port[I2C_NUM_0].port_activated)
        return clearBus(I2C_NUM_0);
    else
        if(_state.port[I2C_NUM_1].port_activated)
            return clearBus(I2C_NUM_1);
    return true;
}

bool MultiI2C::clearBus(i2c_port_t i2c_port)
{
    return (ESP_OK == i2c_reset_rx_fifo(i2c_port)) & (ESP_OK == i2c_reset_tx_fifo(i2c_port));
}

bool MultiI2C::getState(i2c_port_t i2c_port)
{
    return _state.port[i2c_port].port_activated;
}

port_config_t MultiI2C::getConfig(i2c_port_t i2c_port)
{
    return _state.port[i2c_port];
}

/* Private */
void MultiI2C::_config_driver(i2c_port_t i2c_port)
{
    _state.port[i2c_port].port_enabled = (ESP_OK == i2c_param_config(i2c_port, &_state.port[i2c_port].port_config));
}

void MultiI2C::_init_driver(void)
{
    if(_state.port[I2C_NUM_0].port_enabled && !_state.port[I2C_NUM_0].port_activated)
        _state.port[I2C_NUM_0].port_activated = (ESP_OK == i2c_driver_install(I2C_NUM_0, _state.port[I2C_NUM_0].port_config.mode, I2C_RX_BUF_DISABLE, I2C_TX_BUF_DISABLE, 0));
    if(_state.port[I2C_NUM_1].port_enabled && !_state.port[I2C_NUM_1].port_activated)
        _state.port[I2C_NUM_1].port_activated = (ESP_OK == i2c_driver_install(I2C_NUM_1, _state.port[I2C_NUM_1].port_config.mode, I2C_RX_BUF_DISABLE, I2C_TX_BUF_DISABLE, 0));
    return;
}