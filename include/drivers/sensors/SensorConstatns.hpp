#if SENSOR_SUPPORT

PROGMEM const char espurna_sensors[] =
    #if AM2320_SUPPORT
        "AM2320_I2C "
    #endif
    #if ANALOG_SUPPORT
        "ANALOG "
    #endif
    #if BH1750_SUPPORT
        "BH1750 "
    #endif
    #if BMP180_SUPPORT
        "BMP180 "
    #endif
    #if BMX280_SUPPORT
        "BMX280 "
    #endif
    #if CSE7766_SUPPORT
        "CSE7766 "
    #endif
    #if DALLAS_SUPPORT
        "DALLAS "
    #endif
    #if DHT_SUPPORT
        "DHTXX "
    #endif
    #if DIGITAL_SUPPORT
        "DIGITAL "
    #endif
    #if ECH1560_SUPPORT
        "ECH1560 "
    #endif
    #if EMON_ADC121_SUPPORT
        "EMON_ADC121 "
    #endif
    #if EMON_ADS1X15_SUPPORT
        "EMON_ADX1X15 "
    #endif
    #if EMON_ANALOG_SUPPORT
        "EMON_ANALOG "
    #endif
    #if EVENTS_SUPPORT
        "EVENTS "
    #endif
    #if GEIGER_SUPPORT
        "GEIGER "
    #endif
    #if GUVAS12SD_SUPPORT
        "GUVAS12SD "
    #endif
    #if HLW8012_SUPPORT
        "HLW8012 "
    #endif
    #if LDR_SUPPORT
        "LDR "
    #endif
    #if MHZ19_SUPPORT
        "MHZ19 "
    #endif
    #if MICS2710_SUPPORT
        "MICS2710 "
    #endif
    #if MICS5525_SUPPORT
        "MICS5525 "
    #endif
    #if NTC_SUPPORT
        "NTC "
    #endif
    #if PMSX003_SUPPORT
        "PMSX003 "
    #endif
    #if PULSEMETER_SUPPORT
        "PULSEMETER "
    #endif
    #if PZEM004T_SUPPORT
        "PZEM004T "
    #endif
    #if SDS011_SUPPORT
        "SDS011 "
    #endif
    #if SENSEAIR_SUPPORT
        "SENSEAIR "
    #endif
    #if SHT3X_I2C_SUPPORT
        "SHT3X_I2C "
    #endif
    #if SI7021_SUPPORT
        "SI7021 "
    #endif
    #if SONAR_SUPPORT
        "SONAR "
    #endif
    #if T6613_SUPPORT
        "T6613 "
    #endif
    #if TMP3X_SUPPORT
        "TMP3X "
    #endif
    #if V9261F_SUPPORT
        "V9261F "
    #endif
    #if VEML6075_SUPPORT
        "VEML6075 "
    #endif
    #if VL53L1X_SUPPORT
        "VL53L1X "
    #endif
    #if EZOPH_SUPPORT
        "EZOPH "
    #endif
    #if ADE7953_SUPPORT
        "ADE7953 "
    #endif
    #if RTD_SUPPORT
        "RTD "
    #endif
    "";


PROGMEM const unsigned char magnitude_decimals[] = {
    0,
    1, 0, 2, // THP
    3, 0, 0, 0, 0, 0, 0, 0, // Power decimals
    0, 0, 0, // analog, digital, event
    0, 0, 0, // PM
    0, 0,
    0, 0, 3, // UVA, UVB, UVI
    3, 0,
    4, 4, // Geiger Counter decimals
    0,
    0, 0, 0, 3    // NO2, CO, Ohms, pH
};

PROGMEM const char magnitude_unknown_topic[] = "unknown";
PROGMEM const char magnitude_temperature_topic[] =  "temperature";
PROGMEM const char magnitude_humidity_topic[] = "humidity";
PROGMEM const char magnitude_pressure_topic[] = "pressure";
PROGMEM const char magnitude_current_topic[] = "current";
PROGMEM const char magnitude_voltage_topic[] = "voltage";
PROGMEM const char magnitude_active_power_topic[] = "power";
PROGMEM const char magnitude_apparent_power_topic[] = "apparent";
PROGMEM const char magnitude_reactive_power_topic[] = "reactive";
PROGMEM const char magnitude_power_factor_topic[] = "factor";
PROGMEM const char magnitude_energy_topic[] = "energy";
PROGMEM const char magnitude_energy_delta_topic[] = "energy_delta";
PROGMEM const char magnitude_analog_topic[] = "analog";
PROGMEM const char magnitude_digital_topic[] = "digital";
PROGMEM const char magnitude_event_topic[] = "event";
PROGMEM const char magnitude_pm1dot0_topic[] = "pm1dot0";
PROGMEM const char magnitude_pm2dot5_topic[] = "pm2dot5";
PROGMEM const char magnitude_pm10_topic[] = "pm10";
PROGMEM const char magnitude_co2_topic[] = "co2";
PROGMEM const char magnitude_lux_topic[] = "lux";
PROGMEM const char magnitude_uva_topic[] = "uva";
PROGMEM const char magnitude_uvb_topic[] = "uvb";
PROGMEM const char magnitude_uvi_topic[] = "uvi";
PROGMEM const char magnitude_distance_topic[] = "distance";
PROGMEM const char magnitude_hcho_topic[] = "hcho";
PROGMEM const char magnitude_geiger_cpm_topic[] = "ldr_cpm";  // local dose rate [Counts per minute]
PROGMEM const char magnitude_geiger_sv_topic[] = "ldr_uSvh";  // local dose rate [µSievert per hour]
PROGMEM const char magnitude_count_topic[] = "count";
PROGMEM const char magnitude_no2_topic[] = "no2";
PROGMEM const char magnitude_co_topic[] = "co";
PROGMEM const char magnitude_resistance_topic[] = "resistance";
PROGMEM const char magnitude_ph_topic[] = "ph";

PROGMEM const char* const magnitude_topics[] = {
    magnitude_unknown_topic, magnitude_temperature_topic, magnitude_humidity_topic,
    magnitude_pressure_topic, magnitude_current_topic, magnitude_voltage_topic,
    magnitude_active_power_topic, magnitude_apparent_power_topic, magnitude_reactive_power_topic,
    magnitude_power_factor_topic, magnitude_energy_topic, magnitude_energy_delta_topic,
    magnitude_analog_topic, magnitude_digital_topic, magnitude_event_topic,
    magnitude_pm1dot0_topic, magnitude_pm2dot5_topic, magnitude_pm10_topic,
    magnitude_co2_topic, magnitude_lux_topic,
    magnitude_uva_topic, magnitude_uvb_topic, magnitude_uvi_topic,
    magnitude_distance_topic, magnitude_hcho_topic,
    magnitude_geiger_cpm_topic, magnitude_geiger_sv_topic,
    magnitude_count_topic,
    magnitude_no2_topic, magnitude_co_topic, magnitude_resistance_topic, magnitude_ph_topic
};

PROGMEM const char magnitude_empty[] = "";
PROGMEM const char magnitude_celsius[] =  "°C";
PROGMEM const char magnitude_fahrenheit[] =  "°F";
PROGMEM const char magnitude_percentage[] = "%";
PROGMEM const char magnitude_hectopascals[] = "hPa";
PROGMEM const char magnitude_amperes[] = "A";
PROGMEM const char magnitude_volts[] = "V";
PROGMEM const char magnitude_watts[] = "W";
PROGMEM const char magnitude_kw[] = "kW";
PROGMEM const char magnitude_joules[] = "J";
PROGMEM const char magnitude_kwh[] = "kWh";
PROGMEM const char magnitude_ugm3[] = "µg/m³";
PROGMEM const char magnitude_ppm[] = "ppm";
PROGMEM const char magnitude_lux[] = "lux";
PROGMEM const char magnitude_distance[] = "m";
PROGMEM const char magnitude_mgm3[] = "mg/m³";
PROGMEM const char magnitude_geiger_cpm[] = "cpm";    // Counts per Minute: Unit of local dose rate (Geiger counting)
PROGMEM const char magnitude_geiger_sv[] = "µSv/h";   // µSievert per hour: 2nd unit of local dose rate (Geiger counting)
PROGMEM const char magnitude_resistance[] = "ohm";


PROGMEM const char* const magnitude_units[] = {
    magnitude_empty, magnitude_celsius, magnitude_percentage,
    magnitude_hectopascals, magnitude_amperes, magnitude_volts,
    magnitude_watts, magnitude_watts, magnitude_watts,
    magnitude_percentage, magnitude_joules, magnitude_joules,
    magnitude_empty, magnitude_empty, magnitude_empty,
    magnitude_ugm3, magnitude_ugm3, magnitude_ugm3,
    magnitude_ppm, magnitude_lux,
    magnitude_empty, magnitude_empty, magnitude_empty,
    magnitude_distance, magnitude_mgm3,
    magnitude_geiger_cpm, magnitude_geiger_sv,                  // Geiger counter units
    magnitude_empty,                                            //
    magnitude_ppm, magnitude_ppm,                               // NO2 & CO2
    magnitude_resistance,
    magnitude_empty                                             // pH
};

#endif