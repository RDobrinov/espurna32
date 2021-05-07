#ifndef OTA_ARDUINOOTA_SUPPORT
#define OTA_ARDUINOOTA_SUPPORT      1           // Support ArduinoOTA by default (4.2Kb)
                                                // Implicitly depends on ESP8266mDNS library, thus increasing firmware size
#endif

#ifndef OTA_PORT
#define OTA_PORT                    3232        // Port for ArduinoOTA
#endif

void arduinoOtaSetup();