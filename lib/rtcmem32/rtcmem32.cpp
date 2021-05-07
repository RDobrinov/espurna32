#include <Arduino.h>
#include <rtcmem32.h>

static RTC_NOINIT_ATTR RtcmemData RtcNoInitData;
RtcmemData *Rtcmem = &RtcNoInitData;

bool _rtcmem_status = false;

void _rtcmemErase() {
}

void _rtcmemInit() {
    _rtcmemErase();
    Rtcmem->magic = RTCMEM_MAGIC;
}

// Treat memory as dirty on cold boot, hardware wdt reset and rst pin
bool _rtcmemStatus() {
    bool readable;
    switch (esp_reset_reason()) {
        case ESP_RST_SW:         //!< Software reset via esp_restart
        case ESP_RST_DEEPSLEEP:  //!< Reset after exiting deep sleep mode
        //case ESP_RST_UNKNOWN:    //!< Reset reason can not be determined
        //case ESP_RST_POWERON:    //!< Reset due to power-on event
        //case ESP_RST_EXT:        //!< Reset by external pin (not applicable for ESP32)
        //case ESP_RST_PANIC:      //!< Software reset due to exception/panic
        //case ESP_RST_INT_WDT:    //!< Reset (software or hardware) due to interrupt watchdog
        //case ESP_RST_TASK_WDT:   //!< Reset due to task watchdog
        //case ESP_RST_WDT:        //!< Reset due to other watchdogs
        //case ESP_RST_BROWNOUT:   //!< Brownout reset (software or hardware)
        //case ESP_RST_SDIO:       //!< Reset over SDIO
            readable = true;
            break;
        default:
            readable = false;
    }

    readable = readable and (RTCMEM_MAGIC == Rtcmem->magic);

    return readable;
}

bool rtcmemStatus() 
{
    return _rtcmem_status;
}

void rtcmemSetup() {
    _rtcmem_status = _rtcmemStatus();
    if (!_rtcmem_status) {
        _rtcmemInit();
    }

    /*#if TERMINAL_SUPPORT
        _rtcmemInitCommands();
    #endif*/
}