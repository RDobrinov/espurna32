#if SENSOR_SUPPORT

//------------------------------------------------------------------------------
// UNITS
//------------------------------------------------------------------------------

#define POWER_WATTS                 0
#define POWER_KILOWATTS             1

#define ENERGY_JOULES               0
#define ENERGY_KWH                  1

#define TMP_CELSIUS                 0
#define TMP_FAHRENHEIT              1
#define TMP_KELVIN                  2

//--------------------------------------------------------------------------------
// Sensor ID
// These should remain over time, do not modify them, only add new ones at the end
//--------------------------------------------------------------------------------

#define SENSOR_DHTXX_ID             1
#define SENSOR_DALLAS_ID            2
#define SENSOR_EMON_ANALOG_ID       3
#define SENSOR_EMON_ADC121_ID       4
#define SENSOR_EMON_ADS1X15_ID      5
#define SENSOR_HLW8012_ID           6
#define SENSOR_V9261F_ID            7
#define SENSOR_ECH1560_ID           8
#define SENSOR_ANALOG_ID            9
#define SENSOR_DIGITAL_ID           10
#define SENSOR_EVENTS_ID            11
#define SENSOR_PMSX003_ID           12
#define SENSOR_BMX280_ID            13
#define SENSOR_MHZ19_ID             14
#define SENSOR_SI7021_ID            15
#define SENSOR_SHT3X_I2C_ID         16
#define SENSOR_BH1750_ID            17
#define SENSOR_PZEM004T_ID          18
#define SENSOR_AM2320_ID            19
#define SENSOR_GUVAS12SD_ID         20
#define SENSOR_CSE7766_ID           21
#define SENSOR_TMP3X_ID             22
#define SENSOR_SONAR_ID             23
#define SENSOR_SENSEAIR_ID          24
#define SENSOR_GEIGER_ID            25
#define SENSOR_NTC_ID               26
#define SENSOR_SDS011_ID            27
#define SENSOR_MICS2710_ID          28
#define SENSOR_MICS5525_ID          29
#define SENSOR_PULSEMETER_ID        30
#define SENSOR_VEML6075_ID          31
#define SENSOR_VL53L1X_ID           32
#define SENSOR_EZOPH_ID             33
#define SENSOR_BMP180_ID            34
#define SENSOR_MAX6675_ID           35
#define SENSOR_LDR_ID               36
#define SENSOR_ADE7953_ID           37
#define SENSOR_T6613_ID             38
#define SENSOR_PT100_ID             39

//--------------------------------------------------------------------------------
// Magnitudes
// These should remain over time, do not modify their values, only add new ones at the end
//--------------------------------------------------------------------------------

#define MAGNITUDE_NONE              0
#define MAGNITUDE_TEMPERATURE       1
#define MAGNITUDE_HUMIDITY          2
#define MAGNITUDE_PRESSURE          3
#define MAGNITUDE_CURRENT           4
#define MAGNITUDE_VOLTAGE           5
#define MAGNITUDE_POWER_ACTIVE      6
#define MAGNITUDE_POWER_APPARENT    7
#define MAGNITUDE_POWER_REACTIVE    8
#define MAGNITUDE_POWER_FACTOR      9
#define MAGNITUDE_ENERGY            10
#define MAGNITUDE_ENERGY_DELTA      11
#define MAGNITUDE_ANALOG            12
#define MAGNITUDE_DIGITAL           13
#define MAGNITUDE_EVENT             14
#define MAGNITUDE_PM1dot0           15
#define MAGNITUDE_PM2dot5           16
#define MAGNITUDE_PM10              17
#define MAGNITUDE_CO2               18
#define MAGNITUDE_LUX               19
#define MAGNITUDE_UVA               20
#define MAGNITUDE_UVB               21
#define MAGNITUDE_UVI               22
#define MAGNITUDE_DISTANCE          23
#define MAGNITUDE_HCHO              24
#define MAGNITUDE_GEIGER_CPM        25
#define MAGNITUDE_GEIGER_SIEVERT    26
#define MAGNITUDE_COUNT             27
#define MAGNITUDE_NO2               28
#define MAGNITUDE_CO                29
#define MAGNITUDE_RESISTANCE        30
#define MAGNITUDE_PH                31

#define MAGNITUDE_MAX               32

/**/
// =============================================================================
// SENSORS - General data
// =============================================================================
/**/

#define SENSOR_DEBUG                        0               // Debug sensors

#define SENSOR_READ_INTERVAL                6               // Read data from sensors every 6 seconds
#define SENSOR_READ_MIN_INTERVAL            1               // Minimum read interval
#define SENSOR_READ_MAX_INTERVAL            3600            // Maximum read interval
#define SENSOR_INIT_INTERVAL                10000           // Try to re-init non-ready sensors every 10s

#define SENSOR_REPORT_EVERY                 10              // Report every this many readings
#define SENSOR_REPORT_MIN_EVERY             1               // Minimum every value
#define SENSOR_REPORT_MAX_EVERY             60              // Maximum

#define SENSOR_USE_INDEX                    0               // Use the index in topic (i.e. temperature/0)
                                                            // even if just one sensor (0 for backwards compatibility)

#ifndef SENSOR_POWER_CHECK_STATUS
#define SENSOR_POWER_CHECK_STATUS           1               // If set to 1 the reported power/current/energy will be 0 if the relay[0] is OFF
#endif

#ifndef SENSOR_TEMPERATURE_CORRECTION
#define SENSOR_TEMPERATURE_CORRECTION       0.0             // Offset correction
#endif

#ifndef TEMPERATURE_MIN_CHANGE
#define TEMPERATURE_MIN_CHANGE              0.0             // Minimum temperature change to report
#endif

#ifndef SENSOR_HUMIDITY_CORRECTION
#define SENSOR_HUMIDITY_CORRECTION          0.0             // Offset correction
#endif

#ifndef HUMIDITY_MIN_CHANGE
#define HUMIDITY_MIN_CHANGE                 0               // Minimum humidity change to report
#endif

#ifndef ENERGY_MAX_CHANGE
#define ENERGY_MAX_CHANGE                   0               // Maximum energy change to report (if >0 it will allways report when delta(E) is greater than this)
#endif

#ifndef SENSOR_SAVE_EVERY
#define SENSOR_SAVE_EVERY                   0               // Save accumulating values to EEPROM (atm only energy)
                                                            // A 0 means do not save and it's the default value
                                                            // A number different from 0 means it should store the value in EEPROM
                                                            // after these many reports
                                                            // Warning: this might wear out flash fast!
#endif

#define SENSOR_PUBLISH_ADDRESSES            0               // Publish sensor addresses
#define SENSOR_ADDRESS_TOPIC                "address"       // Topic to publish sensor addresses


#ifndef SENSOR_TEMPERATURE_UNITS
#define SENSOR_TEMPERATURE_UNITS            TMP_CELSIUS     // Temperature units (TMP_CELSIUS | TMP_FAHRENHEIT)
#endif

#ifndef SENSOR_ENERGY_UNITS
#define SENSOR_ENERGY_UNITS                 ENERGY_JOULES   // Energy units (ENERGY_JOULES | ENERGY_KWH)
#endif

#ifndef SENSOR_POWER_UNITS
#define SENSOR_POWER_UNITS                  POWER_WATTS     // Power units (POWER_WATTS | POWER_KILOWATTS)
#endif


// =============================================================================
// Specific data for each sensor
// =============================================================================

//------------------------------------------------------------------------------
// BME280/BMP280
// Enable support by passing BMX280_SUPPORT=1 build flag
//------------------------------------------------------------------------------

#ifndef BMX280_SUPPORT
#define BMX280_SUPPORT                  0
#endif

#ifndef BMX280_I2CPORT_A
#define BMX280_I2CPORT_A                I2C_NUM_0
#endif

#ifndef BMX280_I2CPORT_B
#define BMX280_I2CPORT_B                I2C_NUM_1
#endif

#ifndef BMX280_NUMBER
#define BMX280_NUMBER                   1       // Number of sensors present. Either 1 or 2 allowed
#endif
#ifndef BMX280_ADDRESS_1
#define BMX280_ADDRESS_1                0x76
#endif
#ifndef BMX280_ADDRESS_2
#define BMX280_ADDRESS_2                0x77
#endif                                      

#ifndef BMX280_MODE
#define BMX280_MODE                     1       // 0 for sleep mode, 1 or 2 for forced mode, 3 for normal mode
#endif
#ifndef BMX280_STANDBY
#define BMX280_STANDBY                  0       // 0 for 0.5ms, 1 for 62.5ms, 2 for 125ms
                                                // 3 for 250ms, 4 for 500ms, 5 for 1000ms
                                                // 6 for 10ms, 7 for 20ms
#endif
#ifndef BMX280_FILTER
#define BMX280_FILTER                   0       // 0 for OFF, 1 for 2 values, 2 for 4 values, 3 for 8 values and 4 for 16 values
#endif
#ifndef BMX280_TEMPERATURE
#define BMX280_TEMPERATURE              1       // Oversampling for temperature (set to 0 to disable magnitude)
                                                // 0b000 = 0 = Skip measurement
                                                // 0b001 = 1 = 1x 16bit/0.0050C resolution
                                                // 0b010 = 2 = 2x 17bit/0.0025C
                                                // 0b011 = 3 = 4x 18bit/0.0012C
                                                // 0b100 = 4 = 8x 19bit/0.0006C
                                                // 0b101 = 5 = 16x 20bit/0.0003C
#endif
#ifndef BMX280_HUMIDITY
#define BMX280_HUMIDITY                 1       // Oversampling for humidity (set to 0 to disable magnitude, only for BME280)
                                                // 0b000 = 0 = Skip measurement
                                                // 0b001 = 1 = 1x 0.07% resolution
                                                // 0b010 = 2 = 2x 0.05%
                                                // 0b011 = 3 = 4x 0.04%
                                                // 0b100 = 4 = 8x 0.03%
                                                // 0b101 = 5 = 16x 0.02%
#endif
#ifndef BMX280_PRESSURE
#define BMX280_PRESSURE                 1       // Oversampling for pressure (set to 0 to disable magnitude)
                                                // 0b000 = 0 = Skipped
                                                // 0b001 = 1 = 1x 16bit/2.62 Pa resolution
                                                // 0b010 = 2 = 2x 17bit/1.31 Pa
                                                // 0b011 = 3 = 4x 18bit/0.66 Pa
                                                // 0b100 = 4 = 8x 19bit/0.33 Pa
                                                // 0b101 = 5 = 16x 20bit/0.16 Pa
#endif

//------------------------------------------------------------------------------
// Dallas OneWire temperature sensors
// Enable support by passing DALLAS_SUPPORT=1 build flag
//------------------------------------------------------------------------------

#ifndef DALLAS_SUPPORT
#define DALLAS_SUPPORT                  0
#endif

#ifndef DALLAS_PIN
#define DALLAS_PIN                      32
#endif

#define DALLAS_RESOLUTION               9           // Not used atm
#define DALLAS_READ_INTERVAL            2000        // Force sensor read & cache every 2 seconds

//------------------------------------------------------------------------------
// Maxim RTD-to-Digital Converter
// Enable support by passing RTD_SUPPORT=1 build flag
//------------------------------------------------------------------------------

#define PT100SENSOR_WIRES   3
#define MEASURED_R_REF      433.5

//
#ifndef SENSOR_LUX_CORRECTION
#define SENSOR_LUX_CORRECTION           0.0     // Offset correction
#endif

#ifndef ANALOG_DECIMALS
#define ANALOG_DECIMALS                  2
#endif

/* EVENTS Sensors */

#ifndef EVENTS1_TRIGGER
#define EVENTS1_TRIGGER                  1       // 1 to trigger callback on events,
                                                // 0 to only count them and report periodically
#endif

#ifndef EVENTS1_PIN
#define EVENTS1_PIN                      0       // GPIO to monitor
#endif

#ifndef EVENTS1_PIN_MODE
#define EVENTS1_PIN_MODE                 INPUT_PULLUP   // INPUT, INPUT_PULLUP
#endif

#ifndef EVENTS1_INTERRUPT_MODE
#define EVENTS1_INTERRUPT_MODE           CHANGE  // RISING, FALLING, CHANGE
#endif

#ifndef EVENTS1_DEBOUNCE
#define EVENTS1_DEBOUNCE                 50      // Do not register events within less than 50 millis
#endif

#ifndef EVENTS2_TRIGGER
#define EVENTS2_TRIGGER                  1       // 1 to trigger callback on events,
                                                // 0 to only count them and report periodically
#endif

#ifndef EVENTS2_PIN
#define EVENTS2_PIN                      GPIO_NONE       // GPIO to monitor
#endif

#ifndef EVENTS2_PIN_MODE
#define EVENTS2_PIN_MODE                 INPUT_PULLUP   // INPUT, INPUT_PULLUP
#endif

#ifndef EVENTS2_INTERRUPT_MODE
#define EVENTS2_INTERRUPT_MODE           RISING  // RISING, FALLING, CHANGE
#endif

#ifndef EVENTS2_DEBOUNCE
#define EVENTS2_DEBOUNCE                 50      // Do not register events within less than 50 millis
#endif

#ifndef EVENTS3_TRIGGER
#define EVENTS3_TRIGGER                  1       // 1 to trigger callback on events,
                                                // 0 to only count them and report periodically
#endif

#ifndef EVENTS3_PIN
#define EVENTS3_PIN                      GPIO_NONE       // GPIO to monitor
#endif

#ifndef EVENTS3_PIN_MODE
#define EVENTS3_PIN_MODE                 INPUT_PULLUP   // INPUT, INPUT_PULLUP
#endif

#ifndef EVENTS3_INTERRUPT_MODE
#define EVENTS3_INTERRUPT_MODE           RISING  // RISING, FALLING, CHANGE
#endif

#ifndef EVENTS3_DEBOUNCE
#define EVENTS3_DEBOUNCE                 50      // Do not register events within less than 50 millis
#endif

#ifndef EVENTS4_TRIGGER
#define EVENTS4_TRIGGER                  1       // 1 to trigger callback on events,
                                                // 0 to only count them and report periodically
#endif

#ifndef EVENTS4_PIN
#define EVENTS4_PIN                      GPIO_NONE       // GPIO to monitor
#endif

#ifndef EVENTS4_PIN_MODE
#define EVENTS4_PIN_MODE                 INPUT   // INPUT, INPUT_PULLUP
#endif

#ifndef EVENTS4_INTERRUPT_MODE
#define EVENTS4_INTERRUPT_MODE           RISING  // RISING, FALLING, CHANGE
#endif

#ifndef EVENTS4_DEBOUNCE
#define EVENTS4_DEBOUNCE                 50      // Do not register events within less than 50 millis
#endif

#ifndef EVENTS5_TRIGGER
#define EVENTS5_TRIGGER                  1       // 1 to trigger callback on events,
                                                // 0 to only count them and report periodically
#endif

#ifndef EVENTS5_PIN
#define EVENTS5_PIN                      GPIO_NONE       // GPIO to monitor
#endif

#ifndef EVENTS5_PIN_MODE
#define EVENTS5_PIN_MODE                 INPUT   // INPUT, INPUT_PULLUP
#endif

#ifndef EVENTS5_INTERRUPT_MODE
#define EVENTS5_INTERRUPT_MODE           RISING  // RISING, FALLING, CHANGE
#endif

#ifndef EVENTS5_DEBOUNCE
#define EVENTS5_DEBOUNCE                 50      // Do not register events within less than 50 millis
#endif


/* Class Loading */ // Not in right place
#if BMX280_SUPPORT
    #include "drivers/sensors/BMX280Sensor.hpp"
#endif

#ifdef DALLAS_SUPPORT
    #include "drivers/sensors/DallasSensor.hpp"
#endif

#if RTD_SUPPORT && SPIBUS_SUPPORT
    #include "drivers/sensors/Pt100Sensor.hpp"
#endif

#if EVENTS_SUPPORT
    #include "drivers/sensors/EventSensor.hpp"
#endif

#endif /* SENSOR_SUPPORT */