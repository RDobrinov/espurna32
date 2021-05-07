
unsigned char getPkgVerison();
unsigned char getChipRevision();
String chipDescription();
String getFeatures();
String getSerialNumber();

unsigned long getUptime();
uint32_t getChipId();
String getIdentifier();
String getBoardName();
String getAdminPass();
const String& getCoreVersion();
const String& getCoreRevision();
unsigned char getHeartbeatMode();
unsigned char getHeartbeatInterval();
String getEspurnaModules();
String getEspurnaOTAModules();
#if SENSOR_SUPPORT
    String getEspurnaSensors();
#endif
String getEspurnaWebUI();
String buildTime();
String getResetReason();


//
void setDefaultHostname();
void setBoardName();

void info();