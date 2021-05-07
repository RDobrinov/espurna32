// -----------------------------------------------------------------------------
// Helper functions
// -----------------------------------------------------------------------------
#include "helperfn.hpp"

char * ltrim(char * s) {
    char *p = s;
    while ((unsigned char) *p == ' ') ++p;
    return p;
}

double roundTo(double num, unsigned char positions) {
    double multiplier = 1;
    while (positions-- > 0) multiplier *= 10;
    return round(num * multiplier) / multiplier;
}

void nice_delay(unsigned long ms) {
    unsigned long start = millis();
    while (millis() - start < ms) delay(1);
}

// This method is called by the SDK to know where to connect the ADC
int __get_adc_mode() {
    return 0; //(int) (ADC_MODE_VALUE);
}

bool isNumber(const char * s) {
    unsigned char len = strlen(s);
    if (0 == len) return false;
    bool decimal = false;
    bool digit = false;
    for (unsigned char i=0; i<len; i++) {
        if (('-' == s[i]) || ('+' == s[i])) {
            if (i>0) return false;
        } else if (s[i] == '.') {
            if (!digit) return false;
            if (decimal) return false;
            decimal = true;
        } else if (!isdigit(s[i])) {
            return false;
        } else {
            digit = true;
        }
    }
    return digit;
}

// ref: lwip2 lwip_strnstr with strnlen
char* strnstr(const char* buffer, const char* token, size_t n) {
  size_t token_len = strnlen(token, n);
  if (token_len == 0) {
    return const_cast<char*>(buffer);
  }

  for (const char* p = buffer; *p && (p + token_len <= buffer + n); p++) {
    if ((*p == *token) && (strncmp(p, token, token_len) == 0)) {
      return const_cast<char*>(p);
    }
  }

  return nullptr;
}

// TODO: force getSetting return type to handle settings
uint32_t u32fromString(const String& string, int base) {

    const char *ptr = string.c_str();
    char *value_endptr = nullptr;

    // invalidate the whole string when invalid chars are detected
    const auto value = strtoul(ptr, &value_endptr, base);
    if (value_endptr == ptr || value_endptr[0] != '\0') {
        return 0;
    }

    return value;

}

uint32_t u32fromString(const String& string) {
    if (!string.length()) {
        return 0;
    }

    int base = 10;
    if (string.length() > 2) {
        if (string.startsWith("0b")) {
            base = 2;
        } else if (string.startsWith("0o")) {
            base = 8;
        } else if (string.startsWith("0x")) {
            base = 16;
        }
    }

    return u32fromString((base == 10) ? string : string.substring(2), base);
}

String u32toString(uint32_t value, int base) {
    String result;
    result.reserve(32 + 2);

    if (base == 2) {
        result += "0b";
    } else if (base == 8) {
        result += "0o";
    } else if (base == 16) {
        result += "0x";
    }

    char buffer[33] = {0};
    ultoa(value, buffer, base);
    result += buffer;

    return result;
}
