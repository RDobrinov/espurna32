#ifndef INC_DRV_SPIBUS_HPP
#define INC_DRV_SPIBUS_HPP

#include "config/hardware.h"

#if SPIBUS_SUPPORT

#include <Arduino.h>
#include "drivers/drv-gpio.h"

#define HSPI_MISO   12
#define HSPI_MOSI   13
#define HSPI_SCLK   14
#define HSPI_SS     15

#include <SPI.h>

#ifndef SPIDRV_SPI_BUS_SPEED
#define SPIDRV_SPI_BUS_SPEED    1000000
#endif

#ifndef SPIDRV_SPI_BIT_ORDER
#define SPIDRV_SPI_BIT_ORDER    SPI_MSBFIRST
#endif

#ifndef SPIDRV_SPI_DATA_MODE
#define SPIDRV_SPI_DATA_MODE    SPI_MODE1
#endif

typedef struct {
    unsigned long speed = SPIDRV_SPI_BUS_SPEED;
    unsigned char bitOrder = SPIDRV_SPI_BIT_ORDER;
    unsigned char dataMode = SPIDRV_SPI_DATA_MODE;
    unsigned char _csPin = HSPI_SS;
} spi_settings_t;

namespace spi {

    extern SPIClass * _hspi;
    extern spi_settings_t _drvSpiSettings;

    bool init(void);

    void setBusSpeed(unsigned long busSpeed);
    void setDataMode(unsigned char dataMode);
    void setBitOrder(unsigned char bitOrder);
    void setCSPin(unsigned char cs = HSPI_SS);

    unsigned char read_uint8(unsigned char reg);
    void read_uint8(unsigned char reg, unsigned char* value);
    void read_blob(unsigned char reg, unsigned char * value, size_t size);
    void write_uint8(unsigned char reg, unsigned char value);
    void write_blob(unsigned char reg, unsigned char * value, size_t size);
}
#endif /* SPIBUS_SUPPORT */
#endif /* INC_DRV_SPIBUS_HPP */