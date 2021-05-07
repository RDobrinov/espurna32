#include <Arduino.h>
#include <core_version.h>
#include <nvs.h>
#include <soc/efuse_reg.h>

#include "espurna32.hpp"

#include "services/svc-ntp.hpp"
#include "config/buildtime.h"
#include "config/version.h"
#include "config/hardware.h"
#include "config/progmem.h"
#include "config/webui.h"
#include "config/general.h"

unsigned char getPkgVerison()
{
  unsigned int reg_data3 = REG_READ(EFUSE_BLK0_RDATA3_REG);
  return ( ( reg_data3 >> 9 ) & 0x07 ) | (((reg_data3 >> 20) & 0x01 ) << 3 );
}

unsigned char getChipRevision()
{
	unsigned char rev_bits = ((( REG_READ(0x3ff6607c) >> 31 ) & 0x01 ) << 2 ) | (((REG_READ(EFUSE_BLK0_RDATA5_REG) >> 20) & 0x01 ) << 1 ) | ((REG_READ(EFUSE_BLK0_RDATA3_REG) >> 15) & 0x01);
	return (rev_bits == 0x07) ? 3 : (rev_bits ==3 ) ? 2 : (rev_bits == 1) ? 1 : 0;
}


String chipDescription()
{
	String chipName;
	unsigned char pkg_version = getPkgVerison();
	unsigned char chip_rev = getChipRevision();
	bool single_core = REG_READ(EFUSE_BLK0_RDATA3_REG) & 0x01;
	switch (pkg_version)
	{
		case 0:
			if(single_core) chipName = "ESP32-S0WDQ6";
			else
			{
				chipName = (chip_rev == 3) ? "ESP32-D0WDQ6-V3" : "ESP32-D0WDQ6";
			}
			break;
		case 1:
			if(single_core) chipName = "ESP32-S0WD";
			else
			{
				chipName = (chip_rev == 3) ? "ESP32-D0WD-V3" : "ESP32-D0WD";
			}
			break;
		case 2:
			chipName = "ESP32-D2WD";
			break;
		case 4:
			chipName = "ESP32-U4WDH";
			break;
		case 5:
			chipName = (chip_rev == 3) ? "ESP32-PICO-V3" : "ESP32-PICO-D4";
			break;
		case 6:
			chipName = "ESP32-PICO-V3-02";
			break;
		default:
			chipName = "unknown ESP32";
	}
	return chipName + " (rev." + String(chip_rev)+")";
}

String getFeatures()
{
	String features = "WiFi";
	unsigned int reg_data3 = REG_READ(EFUSE_BLK0_RDATA3_REG);
	features += ( reg_data3 & 0x01 ) ? ", Single Core" : " ,Dual Core";
	features += ( reg_data3 & 0x02 ) ? "" : " ,BT";
	if(reg_data3 & 0x0200) features += ( reg_data3 & 0x0100 ) ? ", 160MHz" : " ,240MHz";
	switch(getPkgVerison())
	{
		case 2:
		case 4:
		case 5:
			features += ", Embedded flash";
			break;
		case 6:
			features += ", Embedded Flash, Embedded PSRAM";
			break;
		default:
			break;
	}
	return features;
}

String getSerialNumber()
{
	uint8_t mac[6];
	if(esp_efuse_mac_get_default(mac) == ESP_OK)
	{
		char buff[10];
		snprintf(buff, sizeof(buff), "%02X%02X%02X%02X", mac[5]^mac[1], mac[4]^mac[0], mac[3]^mac[1],mac[2]^mac[0]);
		return String(buff);
	}
	return String("BADSERCC");
}

unsigned long getUptime() {

    static unsigned long last_uptime = 0;
    static unsigned char uptime_overflows = 0;

    if (millis() < last_uptime) ++uptime_overflows;
    last_uptime = millis();
    unsigned long uptime_seconds = uptime_overflows * (UPTIME_OVERFLOW / 1000) + (last_uptime / 1000);

    return uptime_seconds;

}

uint32_t getChipId()
{
    return (ESP.getEfuseMac() & 0x00FFFFFF);
}

String getIdentifier() {
    char buffer[20];
    snprintf_P(buffer, sizeof(buffer), PSTR("%s-%06X"), APP_NAME, getChipId());
    return String(buffer);
}

void setDefaultHostname() {
    if (strlen(HOSTNAME) > 0) {
        setSetting("hostname", HOSTNAME);
    } else {
        setSetting("hostname", getIdentifier());
    }
}

void setBoardName() {
    #ifndef ESPURNA_CORE
        setSetting("boardName", DEVICE_NAME);
    #endif
}

String getBoardName() {
    return getSetting("boardName", DEVICE_NAME);
}

String getAdminPass() {
    return getSetting("adminPass", ADMIN_PASS);
}
#ifdef ARDUINO_ESP32_RELEASE
#endif
const String& getCoreVersion() {
    static String version;
    if (!version.length()) {
        #ifdef ARDUINO_ESP32_RELEASE //ARDUINO_ESP8266_RELEASE
            version = String(ARDUINO_ESP32_RELEASE);
            /*version = ESP.getCoreVersion();
            if (version.equals("00000000")) {
                version = String(ARDUINO_ESP32_RELEASE);
            }*/
            version.replace("_", ".");
        #else
            #define _GET_COREVERSION_STR(X) #X
            #define GET_COREVERSION_STR(X) _GET_COREVERSION_STR(X)
            version = GET_COREVERSION_STR(ARDUINO_ESP32_GIT_DESC);
            #undef _GET_COREVERSION_STR
            #undef GET_COREVERSION_STR
        #endif
    }
    return version;
}

const String& getCoreRevision() {
    static String revision;
    if (!revision.length()) {
        #ifdef ARDUINO_ESP32_GIT_VER
            revision = String(ARDUINO_ESP32_GIT_VER, 16);
        #else
            revision = "(unspecified)";
        #endif
    }
    return revision;
}

unsigned char getHeartbeatMode() {
    return getSetting("hbMode", HEARTBEAT_MODE).toInt();
}

unsigned char getHeartbeatInterval() {
    return getSetting("hbInterval", HEARTBEAT_INTERVAL).toInt();
}

String getEspurnaModules() {
    return FPSTR(espurna_modules);
}

String getEspurnaOTAModules() {
    return FPSTR(espurna_ota_modules);
}

#if SENSOR_SUPPORT
String getEspurnaSensors() {
    return FPSTR(espurna_sensors);
}
#endif

String getEspurnaWebUI() {
    return FPSTR(espurna_webui);
}

String buildTime() {
    #if NTP_SUPPORT
        return ntpDateTime(__UNIX_TIMESTAMP__);
    #else
        char buffer[20];
        snprintf_P(
            buffer, sizeof(buffer), PSTR("%04d-%02d-%02d %02d:%02d:%02d"),
            __TIME_YEAR__, __TIME_MONTH__, __TIME_DAY__,
            __TIME_HOUR__, __TIME_MINUTE__, __TIME_SECOND__
        );
        return String(buffer);
    #endif
}

String getResetReason()
{
    switch (esp_reset_reason()) 
    {
        case ESP_RST_SW:         
            return "Software reset via esp_restart";
            break;
        case ESP_RST_DEEPSLEEP:
            return "Reset after exiting deep sleep mode";
            break;
        case ESP_RST_UNKNOWN:
            return "Reset reason can not be determined";
            break;
        case ESP_RST_POWERON:
            return "Reset due to power-on event";
            break;
        //case ESP_RST_EXT:        //!< Reset by external pin (not applicable for ESP32)
        case ESP_RST_PANIC:
            return "Software reset due to exception/panic";
            break;
        case ESP_RST_INT_WDT:
            return "Reset (software or hardware) due to interrupt watchdog";
            break;
        case ESP_RST_TASK_WDT:
            return "Reset due to task watchdog";
            break;
        case ESP_RST_WDT:
            return "Reset due to other watchdogs";
            break;
        case ESP_RST_BROWNOUT:
            return "Brownout reset (software or hardware)";
            break;
        case ESP_RST_SDIO:
            return "Reset over SDIO";
            break;
        default:
            break;
    }
    return "Unknown reset reason";
}

void info() {

    DEBUG_MSG_P(PSTR("\n\n"));

    // -------------------------------------------------------------------------

    #if defined(APP_REVISION)
        DEBUG_MSG_P(PSTR("[MAIN] " APP_NAME " " APP_VERSION " (" APP_REVISION ")\n"));
    #else
        DEBUG_MSG_P(PSTR("[MAIN] " APP_NAME " " APP_VERSION "\n"));
    #endif
    DEBUG_MSG_P(PSTR("[APPS] " APP_AUTHOR "\n"));
    DEBUG_MSG_P(PSTR("[APPS] " APP_WEBSITE "\n\n"));
    DEBUG_MSG_P(PSTR("[HDWR] CPU %s (%s)\n"), chipDescription().c_str(), getFeatures().c_str() );
    DEBUG_MSG_P(PSTR("[HDWR] CPU chip ID: 0x%06X\n"), getChipId());
    DEBUG_MSG_P(PSTR("[HDWR] CPU frequency: %u MHz\n"), ESP.getCpuFreqMHz());
    DEBUG_MSG_P(PSTR("[APPS] SDK version: %s\n"), ESP.getSdkVersion());
    DEBUG_MSG_P(PSTR("[APPS] Core version: %s\n"), getCoreVersion().c_str());
    DEBUG_MSG_P(PSTR("[APPS] Core revision: %s\n"), getCoreRevision().c_str());
    DEBUG_MSG_P(PSTR("[APPS] Build time: %lu\n"), __UNIX_TIMESTAMP__);
    DEBUG_MSG_P(PSTR("\n"));

    // -------------------------------------------------------------------------

    FlashMode_t mode = ESP.getFlashChipMode();
    UNUSED(mode);
    DEBUG_MSG_P(PSTR("[FLSH] Flash chip ID: 0x%06X\n"), ESP.getFlashChipSize());
    DEBUG_MSG_P(PSTR("[FLSH] Flash speed: %u Hz\n"), ESP.getFlashChipSpeed());
    DEBUG_MSG_P(PSTR("[FLSH] Flash mode: %s\n"), mode == FM_QIO ? "QIO" : mode == FM_QOUT ? "QOUT" : mode == FM_DIO ? "DIO" : mode == FM_DOUT ? "DOUT" : "UNKNOWN");
    DEBUG_MSG_P(PSTR("\n"));

    // -------------------------------------------------------------------------

    /*_info_print_memory_layout_line("Flash size (CHIP)", ESP.getFlashChipRealSize(), true);
    _info_print_memory_layout_line("Flash size (SDK)", ESP.getFlashChipSize(), true);
    _info_print_memory_layout_line("Reserved", 1 * SPI_FLASH_SEC_SIZE, true);
    _info_print_memory_layout_line("Firmware size", ESP.getSketchSize());
    _info_print_memory_layout_line("Max OTA size", info_ota_space());
    _info_print_memory_layout_line("SPIFFS size", info_filesystem_space());
    _info_print_memory_layout_line("EEPROM size", info_eeprom_space());
    _info_print_memory_layout_line("Reserved", 4 * SPI_FLASH_SEC_SIZE);
    DEBUG_MSG_P(PSTR("\n"));*/

    // -------------------------------------------------------------------------

    #if SPIFFS_SUPPORT
        FSInfo fs_info;
        bool fs = SPIFFS.info(fs_info);
        if (fs) {
            DEBUG_MSG_P(PSTR("[MAIN] SPIFFS total size   : %8u bytes / %4d sectors\n"), fs_info.totalBytes, info_bytes2sectors(fs_info.totalBytes));
            DEBUG_MSG_P(PSTR("[MAIN]        used size    : %8u bytes\n"), fs_info.usedBytes);
            DEBUG_MSG_P(PSTR("[MAIN]        block size   : %8u bytes\n"), fs_info.blockSize);
            DEBUG_MSG_P(PSTR("[MAIN]        page size    : %8u bytes\n"), fs_info.pageSize);
            DEBUG_MSG_P(PSTR("[MAIN]        max files    : %8u\n"), fs_info.maxOpenFiles);
            DEBUG_MSG_P(PSTR("[MAIN]        max length   : %8u\n"), fs_info.maxPathLength);
        } else {
            DEBUG_MSG_P(PSTR("[MAIN] No SPIFFS partition\n"));
        }
        DEBUG_MSG_P(PSTR("\n"));
    #endif


    //DEBUG_MSG_P(PSTR("\n"));

    //static bool show_frag_stats = false;

    nvs_stats_t nvs_stats;
    nvs_get_stats(NULL, &nvs_stats);

    DEBUG_MSG_P(PSTR("[MAIN] Total NVS Entries %u ( %u Used / %u Free )\n"), nvs_stats.total_entries,  nvs_stats.used_entries, nvs_stats.free_entries );
    //infoMemory("EEPROM", SPI_FLASH_SEC_SIZE, SPI_FLASH_SEC_SIZE - settingsSize());
    //infoHeapStats(show_frag_stats);
    //infoMemory("Stack", CONT_STACKSIZE, getFreeStack());
    DEBUG_MSG_P(PSTR("\n"));

    //show_frag_stats = true;

    unsigned char reason = customResetReason();
    if (reason > 0) {
        char buffer[32];
        strcpy_P(buffer, custom_reset_string[reason-1]);
        DEBUG_MSG_P(PSTR("[MAIN] Last reset reason: %s\n"), buffer);
    } else {
        DEBUG_MSG_P(PSTR("[MAIN] Last reset reason: %s\n"), (char *) getResetReason().c_str());
    }
    DEBUG_MSG_P(PSTR("\n"));

    // -------------------------------------------------------------------------

    DEBUG_MSG_P(PSTR("[MAIN] Board: %s\n"), getBoardName().c_str());
    DEBUG_MSG_P(PSTR("[MAIN] Support: %s\n"), getEspurnaModules().c_str());
    DEBUG_MSG_P(PSTR("[MAIN] OTA: %s\n"), getEspurnaOTAModules().c_str());
    #if SENSOR_SUPPORT
        DEBUG_MSG_P(PSTR("[MAIN] Sensors: %s\n"), getEspurnaSensors().c_str());
    #endif // SENSOR_SUPPORT
    DEBUG_MSG_P(PSTR("[MAIN] WebUI image: %s\n"), getEspurnaWebUI().c_str());
    //DEBUG_MSG_P(PSTR("\n"));

    // -------------------------------------------------------------------------

    DEBUG_MSG_P(PSTR("[MAIN] Firmware MD5: %s\n"), (char *) ESP.getSketchMD5().c_str());
    #if ADC_MODE_VALUE == ADC_VCC
        //DEBUG_MSG_P(PSTR("[MAIN] Power: %u mV\n"), ESP.getVcc());
    #endif
    if (espurna32LoopDelay()) {
        DEBUG_MSG_P(PSTR("[MAIN] Power saving delay value: %lu ms\n"), espurna32LoopDelay());
    }

    //const WiFiSleepType_t sleep_mode = WiFi.getSleepMode();
    //if (sleep_mode != WIFI_NONE_SLEEP) {
    //    DEBUG_MSG_P(PSTR("[MAIN] WiFi Sleep Mode: %s\n"), _info_wifi_sleep_mode(sleep_mode));
    //}

    // -------------------------------------------------------------------------

    #if SYSTEM_CHECK_ENABLED
        if (!systemCheck()) {
            DEBUG_MSG_P(PSTR("\n"));
            DEBUG_MSG_P(PSTR("[MAIN] Device is in SAFE MODE\n"));
        }
    #endif

    // -------------------------------------------------------------------------

    DEBUG_MSG_P(PSTR("\n\n"));
}