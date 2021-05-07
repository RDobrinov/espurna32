#ifndef _NVKEYVALUE_H_
#define _NVKEYVALUE_H_

#define NVK_DEFAULT_STORAGE_NAME    "NVKeyValue"
#define NVK_DECIMAL_PLACES          5
#define SETTINGS_MAX_LIST_COUNT     10          // Maximum index for settings lists

#include <Arduino.h>
#include <Preferences.h>

class NVKeyValue {
    static NVKeyValue *_instance;
    Preferences _nvStore;
    bool _ready;
    /*
    NVKeyValue(const NVKeyValue&);
    NVKeyValue& operator=(const NVKeyValue&);
    */
    NVKeyValue();

    public:
        static NVKeyValue *getInstance();
        static bool set(String key, String value);
        static String get(String key, String value);
        static bool remove(String key);
        static bool isKey(String key);
        static void clear();
};

String getSetting(const String& key);
bool hasSetting(String key);
bool hasSetting(String key, unsigned int index);
bool delSetting(String key);
bool delSetting(String key, unsigned int index);

template<typename T> bool setSetting(const String& key, T value) {
    return NVKeyValue::set(key, String(value));
}

template<> inline bool setSetting(const String& key, double value) {
    return NVKeyValue::set(key, String(value, NVK_DECIMAL_PLACES));
}

template<typename T> bool setSetting(const String& key, unsigned int index, T value) {
    return NVKeyValue::set(key + "|" + String(index), String(value));
}

template<> inline bool setSetting(const String& key, unsigned int index, double value) {
    return NVKeyValue::set(key + "|" + String(index), String(value, NVK_DECIMAL_PLACES));
}

template<typename T> String getSetting(const String& key, T defaultValue) {
    return NVKeyValue::get(key, String(defaultValue));
}

template<typename T> String getSetting(const String& key, unsigned int index, T defaultValue) {
    return NVKeyValue::get(key + "|" + String(index), String(defaultValue));
}

#define resetSettings() NVKeyValue::clear()

#endif