#include "nvkeyvalue.hpp"

NVKeyValue* NVKeyValue::_instance = NULL;

NVKeyValue::NVKeyValue()
{
    _ready = _nvStore.begin(NVK_DEFAULT_STORAGE_NAME, false);
};

NVKeyValue *NVKeyValue::getInstance()
{
    if(!_instance)
    {
        _instance = new NVKeyValue();
        if(!_instance->_ready)
        {
            delete _instance;
            return nullptr;
        }
    }
    return _instance;    
}

bool NVKeyValue::set(String key, String value)
{
    if(!_instance) return false;

    return ( 0 != _instance->_nvStore.putString(key.c_str(), value) );
}

String NVKeyValue::get(String key, String defaultValue)
{
    if(!_instance) return defaultValue;

    if(_instance->_nvStore.isKey(key.c_str()))
        return _instance->_nvStore.getString(key.c_str());
    else
        return defaultValue;    
}

bool NVKeyValue::remove(String key)
{
    if(!_instance) return false;

    if(_instance->_nvStore.isKey(key.c_str())) 
    {
        return _instance->_nvStore.remove(key.c_str());
    }
    return false;
    //return _instance->_nvStore.remove(key.c_str());
}

bool NVKeyValue::isKey(String key)
{
    if(!_instance) return false;

    return _instance->_nvStore.isKey(key.c_str());
}

void NVKeyValue::clear()
{
    if(_instance) _instance->_nvStore.clear();
}

bool hasSetting(String key)
{
    return NVKeyValue::isKey(key);
}
bool hasSetting(String key, unsigned int index)
{
    return NVKeyValue::isKey(key + "|" + String(index));
}

bool delSetting(String key)
{
    return NVKeyValue::remove(key);
}
bool delSetting(String key, unsigned int index)
{
    return NVKeyValue::remove(key + "|" + String(index));
}

String getSetting(const String& key)
{
    return NVKeyValue::get(key, "");
}