
#include "drivers/drv-spi_bus.hpp"
#include "drivers/sensors/Pt100Sensor.hpp"

#if RTD_SUPPORT && SPIBUS_SUPPORT

MAX31865Sensor::MAX31865Sensor(): BaseSensor()
{
    _sensor_id = SENSOR_BMX280_ID;
}

bool MAX31865Sensor::measure()
{
    clearFault();
    cfgRegisterCtrl(MAX31865_BIAS_BITS, true);   //Enable bias
    delay(MAX31865_BIAS_DELAY);
    cfgRegisterCtrl(MAX31865_BEG_1SHOT, true);   // Start 1-shot measurement
    delay(MAX31865_MAX_TIME);
    cfgRegisterCtrl(MAX31865_BIAS_BITS, false);   //Disable bias - reduce self heating
    rawRead();
    if(state.raw[MAX31865_LRTD_REG] & 0x01) return false;
    calculate();
    return true;
}

void MAX31865Sensor::calculate()
{
    #if PT100SENSOR_RPOLY_FN
    fnRPoly();
    #endif
    #if PT100SENSOR_CUBIC_FIT
    fnCubicFit();
    #endif
    #if PT100SENSOR_TABLE_LUP
    fnTableLookup();
    #endif
    #if PT100SENSOR_VAN_DUSEN
    fnVanDusen();
    #endif
}

void MAX31865Sensor::begin()
{
    if(!_dirty) return;
    init();
    _dirty = !_ready;
}

void MAX31865Sensor::pre()
{
    if(_run_init)
    {
        init();
    }
    _error = SENSOR_ERROR_OK;
    if(!measure())
    {
        _error = SENSOR_ERROR_OUT_OF_RANGE;
        _run_init = true;
        return;
    }
    calculate();
}

String MAX31865Sensor::description()
{
    return String("MAX31865 @ HSPI");
}

unsigned char MAX31865Sensor::type(unsigned char index)
{
    if( index < _count) return MAGNITUDE_TEMPERATURE;
    return MAGNITUDE_NONE;
}

signed char MAX31865Sensor::decimals(unsigned char type)
{
    if( type == MAGNITUDE_TEMPERATURE) return 2;
    return -1;
}

double MAX31865Sensor::value(unsigned char index)
{
    unsigned char ci = 0;
    #if PT100SENSOR_RPOLY_FN
    if( index == ci++ ) return state.TRPolyFn;
    #endif
    #if PT100SENSOR_CUBIC_FIT
    if( index == ci++ ) return state.TCubicFit;
    #endif
    #if PT100SENSOR_VAN_DUSEN
    if( index == ci++ ) return state.TVanDusen;
    #endif
    #if PT100SENSOR_TABLE_LUP
    if( index == ci++ ) return state.TableLU;
    #endif
    return 0;
}

String MAX31865Sensor::address(unsigned char index)
{
    unsigned char ci = 0;
    #if PT100SENSOR_RPOLY_FN
    if( index == ci++ ) return "MAX31865 @ SPI (Rational polynomial)";
    #endif
    #if PT100SENSOR_CUBIC_FIT
    if( index == ci++ ) return "MAX31865 @ SPI (Cubic fit)";
    #endif
    #if PT100SENSOR_VAN_DUSEN
    if( index == ci++ ) return "MAX31865 @ SPI (Inverse Van Dusen)";
    #endif
    #if PT100SENSOR_TABLE_LUP
    if( index == ci++ ) return "MAX31865 @ SPI (Table lookup)";
    #endif
    return "Unknown";
}

String MAX31865Sensor::slot(unsigned char index)
{
    return address(index);
}

/* For debug only */
unsigned char * MAX31865Sensor::getRaw()
{
    return state.raw;
}
rtdMAX31865_t MAX31865Sensor::getState()
{
    return state;
}
/* */

void MAX31865Sensor::cfgRegisterCtrl(unsigned char regBits, bool set)
{
    unsigned char cfgReg = spi::read_uint8(MAX31865_CFG_REG);
    spi::write_uint8(MAX31865_CFG_REG | MAX31865_WRITE_BIT, ( set ? (cfgReg | regBits) : (cfgReg & ~regBits) ));
}

void MAX31865Sensor::writeRegister(unsigned char reg, unsigned char value)
{
    spi::write_uint8((reg | MAX31865_WRITE_BIT), value);
}

void MAX31865Sensor::rawRead()
{
    spi::read_blob(MAX31865_CFG_REG, state.raw, 8);
    state.adcValue = ((unsigned int)state.raw[MAX31865_MRTD_REG]<<8 | state.raw[MAX31865_LRTD_REG]) >> 1;
    state.rtdOhms = ((double)state.adcValue * MEASURED_R_REF )/32768.0; 
}

void MAX31865Sensor::clearFault()
{
    /* clear fault status -  write 0 to D5, D3 and D2 and rise Fault clear bit */
    writeRegister(MAX31865_CFG_REG, (spi::read_uint8(MAX31865_CFG_REG) & ~0b00101100) | MAX31865_FAULT_CLR);
}

void MAX31865Sensor::init()
{
    _count = 0;
    #if PT100SENSOR_RPOLY_FN
    _count++;
    #endif
    #if PT100SENSOR_CUBIC_FIT
    _count++;
    #endif
    #if PT100SENSOR_VAN_DUSEN
    _count++;
    #endif
    #if PT100SENSOR_TABLE_LUP
    _count++;
    #endif

    spi::init();
    cfgRegisterCtrl(MAX31865_3WIRE_RTD, (PT100SENSOR_WIRES == 3));  // Set number ot wires
    cfgRegisterCtrl(MAX31865_BIAS_BITS, false);                     // Bias disabled
    cfgRegisterCtrl(MAX31865_AUTO_BITS, false);                     // Autoconvert disabled
    cfgRegisterCtrl(MAX31865_50HZ_NOTCH, true);                     // MAX31865_50HZ_NOTCH 
    clearFault();
    _run_init = false;
    _ready = true;
}

#if PT100SENSOR_RPOLY_FN
/*
* Rational polynomial function from Mosaic Industries site Section RTD Calibration
* http://www.mosaic-industries.com/embedded-systems/microcontroller-projects/temperature-measurement/platinum-rtd-sensors/resistance-calibration-table
* Average absolute error is only 0.015°C over the full temperature range
*
* c0 	    c1 	    c2 	        c3 	        c4 	        c5 	        c6 	        c7
* -245.19 	2.5293 	-0.066046 	4.0422E-3 	-2.0697E-6 	-0.025422 	1.6883E-3 	-1.3601E-6 
*/
#define C0   -245.19
#define C1   2.5293
#define C2   -0.066046
#define C3   4.0422e-3
#define C4   -2.0697e-6
#define C5   -0.025422
#define C6   1.6883e-3
#define C7   -1.3601e-6

void MAX31865Sensor::fnRPoly()
{
    state.TRPolyFn = C0 + ( ( state.rtdOhms * (C1 + state.rtdOhms * (C2 + state.rtdOhms * (C3 + C4 * state.rtdOhms))) )
                         / ( 1 + (state.rtdOhms * (C5 + state.rtdOhms * (C6 + C7 * state.rtdOhms))) ) );
}
#endif

#if PT100SENSOR_CUBIC_FIT
/*
* Rational polynomial function from Mosaic Industries site Section RTD Calibration
* http://www.mosaic-industries.com/embedded-systems/microcontroller-projects/temperature-measurement/platinum-rtd-sensors/resistance-calibration-table
* Average absolute error is 0.03°C over the -100 -> +600 temperature range
*/
void MAX31865Sensor::fnCubicFit()
{
    state.TCubicFit = -247.29 + state.rtdOhms * ( 2.3992 + state.rtdOhms * (0.00063962 + 1.0241e-6 * state.rtdOhms) );
}
#endif

#if PT100SENSOR_VAN_DUSEN
/*
* From Standard EN 60751:1995 
* A = 3.9083 ∙ 10-3 °C-1
* B = -5.775 ∙ 10-7 °C-2
* C = -4.183 ∙ 10-12 °C-4
* 
* Calculations from Logic Beach Inc. App note AN-401 
* https://logicbeach.com/wp-content/uploads/AN401-RTD-Interface-to-the-IL_2017_10-25-a.pdf
*   S1 = -iCVD_A;
*   S2 = iCVD_A * iCVD_A - (4 * iCVD_B);
*   S3 = (4 * iCVD_B) / PT100_NOMINAL;
*   S4 = 2 * iCVD_B;
*   temp = Z2 + (Z3 * R_ohms);
*   temp = (sqrt(temp) + Z1) / Z4;
*/

#define CVD_A   3.9083e-3
#define CVD_B   -5.775e-7
#define CVD_C   -4.183e-12
#define S1      -3.9083e-3      /* -CVD_A                       */
#define S2      1.75848e-5      /* CVD_A^2 - 4*CVD_B            */
#define S3      -2.31e-8        /* 4*CVD_B / 100 (RT nominal)   */
#define S4      -1.155e-6       /* 2*CVD_B                      */

void MAX31865Sensor::fnVanDusen()
{
    state.TVanDusen = S2 + ( S3 * state.rtdOhms);
    state.TVanDusen = (sqrt(state.TVanDusen) + S1) / S4;
}
#endif

#if PT100SENSOR_TABLE_LUP
/*
* Lookup adc value lookup in table and return temperature directly
* Based on https://github.com/drhaney/pt100rtd but instead ot resistance performs adc readout lookup.
* Values in table is calculated for Rref of Adaruit MAX31865 Arduino module ( See header file )
*/
#ifdef REF_TABLE_LOW_LIMIT
    const unsigned int REF_TABLE_LOW_ID = REF_TABLE_LOW_LIMIT;
#else
    const unsigned int REF_TABLE_LOW_ID = 0;
#endif
#ifdef REF_TABLE_HIGH_LIMIT
    const unsigned int REF_TABLE_HIGH_ID = REF_TABLE_HIGH_LIMIT;
#else
    const unsigned int REF_TABLE_HIGH_ID = ((sizeof(adc_table) / sizeof(unsigned int)) - 1);    // Проверка за out of limits
#endif

/* 
* Search for nearest index in ADC table values
* and calculate temperature based on that index.
*/
void MAX31865Sensor::fnTableLookup()
{
    unsigned int adcLow, adcUp;
    int iIntTemp = 0;
    int iFracTemp = 0;

    unsigned int low = REF_TABLE_LOW_ID;
    unsigned int up = REF_TABLE_HIGH_ID;
    unsigned int chk = ( low + up ) / 2;
    unsigned int adcValue;
    do
    {
        adcValue = pgm_read_word_near(&adc_table[chk]);
        if(adcValue == state.adcValue)
        {
            break;
        }
        else 
        {
            if (adcValue < state.adcValue)
            {
                low = chk + 1;
            }
            else
            {
                up = chk;
            }                   
        }
        chk = ( low + up ) / 2;
    } while ( low < up );
    
    if(chk == REF_TABLE_LOW_ID)
    {
        state.TableLU = (double)(TBL_MIN_CELSIUS+REF_TABLE_LOW_ID);
        return;
    }
    if(chk == REF_TABLE_HIGH_ID)
    {
        state.TableLU = (double)(TBL_MIN_CELSIUS+REF_TABLE_HIGH_ID);
        return;
    }
    iIntTemp = chk-1 + TBL_MIN_CELSIUS;
    adcLow = pgm_read_word_near(&adc_table[chk - 1]);
    adcUp = pgm_read_word_near(&adc_table[chk]);
    if( state.adcValue == adcUp )
    {
        iIntTemp++;
        iFracTemp = 0;
    }
    else
    {
        if( state.adcValue < adcUp )
        {
            iFracTemp = ((100 * ( state.adcValue - adcLow )) / ( adcUp - adcLow ));
        }
        else
        {
            state.TableLU = (double)(TBL_MIN_CELSIUS+REF_TABLE_HIGH_ID);  //Funny :-)
        }               
    }
    state.TableLU = (double)iIntTemp + double(iFracTemp)/100.0;
}
#endif

#endif /* RTD_SUPPORT && SPIBUS_SUPPORT */
