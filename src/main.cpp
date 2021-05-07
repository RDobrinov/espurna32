#include <Arduino.h>
#include <vector>
#include "freertos/task.h"
#include "ArduinoJson.h"

/* extern "C" {
#include <esp_spiram.h>
#include <esp_himem.h>
} */
#include "config/hardware.h"
#include "config/version.h"
#include "espurna32.hpp"
#include "main.hpp"

std::vector<void_callback_f> _loop_callbacks;
std::vector<void_callback_f> _reload_callbacks;

bool _reload_config = false;
unsigned long _loop_delay = 0;

unsigned int *pt100 = nullptr;

typedef struct
{
    void_callback_f task;
} taskFn_st;

// -----------------------------------------------------------------------------
// GENERAL CALLBACKS
// -----------------------------------------------------------------------------

void espurna32RegisterLoop(void_callback_f callback) {
    _loop_callbacks.push_back(callback);
}

void espurna32RegisterReload(void_callback_f callback) {
    _reload_callbacks.push_back(callback);
}

void espurna32Reload() {
    _reload_config = true;
}

void _espurna32Reload() {
    for (const auto& callback : _reload_callbacks) {
        callback();
    }
}

/* 
//Simple sheduler - WTF
// Used in homeassistant integration but WHY???? ( Original code is commented )
//Execute a user defined function and kill RTOS Task
void Shedule(void * params)
{
    ((void_callback_f)params)();
    vTaskDelete(NULL);
}
//Accept pointer to function and create FreeRTOS task to execute
void schedule_function(void_callback_f xTaskFn)
{
    xTaskCreate(Shedule, "xTaskFn", 2048, (void *)xTaskFn, 1, NULL );
}
*/
/*
// For tests only 
void sheduled_fn()
{
    DEBUG_MSG_P(PSTR("sheduled_fn() Main CPU %s (%s)\n"), chipDescription().c_str(), getFeatures().c_str() );   
    DEBUG_MSG_P(PSTR("sheduled_fn() end\n")); 
}
*/
unsigned long espurna32LoopDelay() {
    return _loop_delay;
}

void setup() {
    //Basic modules
    //setInitialFreeHeap();
    // Serial debug
    #if DEBUG_SUPPORT
        debugSetup();
        DEBUG_MSG_P(PSTR("FreeRTOS " tskKERNEL_VERSION_NUMBER " running %s on CPU Core %u\n"), pcTaskGetTaskName(NULL), xPortGetCoreID());
        DEBUG_MSG_P(PSTR("FreeRTOS " tskKERNEL_VERSION_NUMBER " task %s booting " APP_NAME " " APP_VERSION " (build %s)\n"), pcTaskGetTaskName(NULL), buildTime().c_str());
        DEBUG_MSG_P(PSTR("[KRNL] Main CPU %s (%s)"), chipDescription().c_str(), getFeatures().c_str() );
        if(psramFound())
        {
            DEBUG_MSG_P(PSTR(" with %uMB PSRAM\n"), (ESP.getPsramSize()+52)/1048576 ); //esp_spiram_get_size()
        }
        else 
            DEBUG_MSG_P(PSTR("\n"));
    #endif

    // Init GPIO functions
    gpioSetup();
    #if DEBUG_SERIAL_SUPPORT
        DEBUG_MSG_P(PSTR("[KRNL] GPIO Init done.\n"));   
    #endif

    // Init RTCMEM
    rtcmemSetup();
    #if DEBUG_SERIAL_SUPPORT
        DEBUG_MSG_P(PSTR("[KRNL] RTC NOINIT Init done.\n"));   
    #endif

    //Init Settings
    if(!NVKeyValue::getInstance()) 
    {
        #if DEBUG_SERIAL_SUPPORT
            DEBUG_MSG_P(PSTR("[NVS] [FATAL ERROR] Partititon not opened"));
        #endif
        return;
    }
    else
    {
        #if DEBUG_SERIAL_SUPPORT
            DEBUG_MSG_P(PSTR("[KRNL] Got NVS Partition handler.\n"));   
        #endif    
    }

    systemSetup();

     // Hostname & board name initialization
    if (getSetting("hostname").length() == 0) {
        setDefaultHostname();
    }
    setBoardName();

    info();
    wifiSetup();

    #if OTA_ARDUINOOTA_SUPPORT
        arduinoOtaSetup();
    #endif

    //#if TELNET_SUPPORT
    //    telnetSetup();
    //#endif

    #if SYSTEM_CHECK_ENABLED
        if (!systemCheck()) 
        {
            #if DEBUG_SERIAL_SUPPORT
                DEBUG_MSG_P(PSTR("[KRNL] System marked as unstable.\n"));   
            #endif
            return;
        }
    #endif
    #if DEBUG_SERIAL_SUPPORT
        DEBUG_MSG_P(PSTR("[KRNL] " APP_NAME " init complete.\n"));   
    #endif
    
    #if WEB_SUPPORT
        webSetup();
        wsSetup();
        #if DEBUG_WEB_SUPPORT
            debugWebSetup();
        #endif
        #if DEBUG_SERIAL_SUPPORT
            DEBUG_MSG_P(PSTR("[KRNL] Webserver is waiting for WiFi\n"));   
        #endif
    #endif

    #if NTP_SUPPORT
        ntpSetup();
    #endif

    #if API_SUPPORT
        apiSetup();
    #endif

    relaySetup();

    #if BUTTON_SUPPORT
        buttonSetup();
    #endif
    /*#if ENCODER_SUPPORT && (LIGHT_PROVIDER != LIGHT_PROVIDER_NONE)
        encoderSetup();
    #endif*/
    #if LED_SUPPORT
        ledSetup();
    #endif

    #if MQTT_SUPPORT
        mqttSetup();
    #endif

    #if MDNS_SERVER_SUPPORT
        mdnsServerSetup();
    #endif

    #if I2C_SUPPORT
        /* For custom I2C Driver initialization*/
        if( I2C_PORT_A != I2C_NUM_MAX) {
            if( ESP_OK != i2c::master::init(I2C_PORT_A))
                DEBUG_MSG_P(PSTR("[KRNL] Failed to initialize RTOS I2C Driver for PORT0\n"));
            else
                DEBUG_MSG_P(PSTR("[I2C0] RTOS I2C Driver loaded\n"));
        }
        if( I2C_PORT_B != I2C_NUM_MAX) {
            if( ESP_OK != i2c::master::init(I2C_PORT_B))
                DEBUG_MSG_P(PSTR("[KRNL] Failed to initialize RTOS I2C Driver for PORT1\n"));
            else
                DEBUG_MSG_P(PSTR("[I2C1] RTOS I2C Driver loaded\n"));
        }
            //DEBUG_MSG_P(PSTR("[I2C%u] RTOS I2C Driver loaded\n"), I2C_NUM_0);
        //i2c::master::device.init();
        //DEBUG_MSG_P(PSTR("[I2C_] Test only ChipID: %02X\n"), i2c::master::read_uint8(0x76, 0xD0));
    #endif

    #if SPIBUS_SUPPORT
        spi::init();
    #endif

    #if SENSOR_SUPPORT
        sensorSetup();
    #endif
    #if INFLUXDB_SUPPORT
        idbSetup();
    #endif

    #if SCHEDULER_SUPPORT
        schSetup();
    #endif

    #if RPN_RULES_SUPPORT
        rpnSetup();
    #endif

    //schedule_function(sheduled_fn);   //Tests
    #if HOMEASSISTANT_SUPPORT
        haSetup();
    #endif
    /*
    if(ESP.getFreePsram()>(800*sizeof(unsigned int)))
    {
        DEBUG_MSG_P(PSTR("PSRAM Availale\n"));
        pt100 = (unsigned int*)ps_malloc(800*sizeof(unsigned int));
        if(pt100)
        {
            pt100[111] = 0xff0f;
            DEBUG_MSG_P(PSTR("[PSRAM] Test only: %0#4x\n"), pt100[111]);
        }
    }*/
}

void loop() 
{
    // Reload config before running any callbacks
    if (_reload_config) {
        _espurna32Reload();
        _reload_config = false;
    }

    // Call registered loop callbacks
    for (unsigned char i = 0; i < _loop_callbacks.size(); i++) {
        (_loop_callbacks[i])();
    }

    // Power saving delay
    //if (_loop_delay) delay(_loop_delay);
}