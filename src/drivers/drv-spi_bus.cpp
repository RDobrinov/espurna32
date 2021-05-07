#include "drivers/drv-spi_bus.hpp"

#if SPIBUS_SUPPORT

namespace spi {

    SPIClass * _hspi = nullptr;
    spi_settings_t _drvSpiSettings;

    bool init(void)
    {
        if(_hspi) return true;

        if( gpioGetLock( HSPI_MISO, "HSPI_MISO") && gpioGetLock( HSPI_MOSI, "HSPI_MOSI") 
            && gpioGetLock( HSPI_SCLK, "HSPI_SCLK") && gpioGetLock( HSPI_SS, "HSPI_SS") )
            {
                _hspi = new SPIClass(HSPI);
                pinMode(HSPI_SS, OUTPUT);
                _hspi->begin();
                return true;
            }
        gpioReleaseLock(HSPI_MISO);
        gpioReleaseLock(HSPI_MOSI);
        gpioReleaseLock(HSPI_SCLK);
        gpioReleaseLock(HSPI_SS);
        return false;
    }

    void setBusSpeed(unsigned long busSpeed)
    {
        _drvSpiSettings.speed = busSpeed;  
        if(_hspi) _hspi->setFrequency(busSpeed); 
    }

    void setDataMode(unsigned char dataMode)
    {
        _drvSpiSettings.dataMode = dataMode;
        if(_hspi) _hspi->setDataMode(dataMode);
    }

    void setBitOrder(unsigned char bitOrder)
    {
        _drvSpiSettings.bitOrder = bitOrder;
        if(_hspi) _hspi->setBitOrder(bitOrder);
    }

    void setCSPin(unsigned char cs)
    {
        _drvSpiSettings._csPin = cs;
    }

    unsigned char read_uint8(unsigned char reg)
    {
        unsigned char result;
        read_uint8(reg, &result);
        return result;
    }

    void read_uint8(unsigned char reg, unsigned char *value)
    {
        if(!_hspi) return;
        _hspi->beginTransaction(SPISettings(_drvSpiSettings.speed, _drvSpiSettings.bitOrder, _drvSpiSettings.dataMode));
        digitalWrite(_drvSpiSettings._csPin, LOW);
        _hspi->transfer(reg);
        *value = _hspi->transfer(0xFF);
        _hspi->endTransaction();
        digitalWrite(_drvSpiSettings._csPin, HIGH);
        return;
    }

    void read_blob(unsigned char reg, unsigned char * value, size_t size)
    {
        if(!_hspi) return;
        _hspi->beginTransaction(SPISettings(_drvSpiSettings.speed, _drvSpiSettings.bitOrder, _drvSpiSettings.dataMode));
        digitalWrite(_drvSpiSettings._csPin, LOW);
        _hspi->transfer(reg);
        _hspi->transferBytes(NULL, value, size);
        _hspi->endTransaction();
        digitalWrite(_drvSpiSettings._csPin, HIGH);
    }

    void write_uint8(unsigned char reg, unsigned char value)
    {
        if(!_hspi) return;
        _hspi->beginTransaction(SPISettings(_drvSpiSettings.speed, _drvSpiSettings.bitOrder, _drvSpiSettings.dataMode));
        digitalWrite(_drvSpiSettings._csPin, LOW);
        _hspi->write(reg);
        _hspi->write(value);
        _hspi->endTransaction();
        digitalWrite(_drvSpiSettings._csPin, HIGH);
    }

    void write_blob(unsigned char reg, unsigned char * value, size_t size)
    {
        if(!_hspi) return;
        _hspi->beginTransaction(SPISettings(_drvSpiSettings.speed, _drvSpiSettings.bitOrder, _drvSpiSettings.dataMode));
        digitalWrite(_drvSpiSettings._csPin, LOW);
        _hspi->write(reg);
        _hspi->writeBytes(value, size);
        _hspi->endTransaction();
        digitalWrite(_drvSpiSettings._csPin, HIGH);
    }
}

#endif /* SPIBUS_SUPPORT */