// -----------------------------------------------------------------------------
// Helper functions
// -----------------------------------------------------------------------------
#include <Arduino.h>

char * ltrim(char * s);
double roundTo(double num, unsigned char positions);
void nice_delay(unsigned long ms);
// This method is called by the SDK to know where to connect the ADC
int __get_adc_mode();
bool isNumber(const char * s) ;
// ref: lwip2 lwip_strnstr with strnlen
char* strnstr(const char* buffer, const char* token, size_t n);
// TODO: force getSetting return type to handle settings
uint32_t u32fromString(const String& string, int base);
uint32_t u32fromString(const String& string);
String u32toString(uint32_t value, int base);
