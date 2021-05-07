

#include "drivers/sensors/filters.hpp"
#include "drivers/sensors/BaseSensor.hpp"

struct sensor_magnitude_t {
    BaseSensor * sensor;        // Sensor object
    BaseFilter * filter;        // Filter object
    unsigned char local;        // Local index in its provider
    unsigned char type;         // Type of measurement
    unsigned char decimals;     // Number of decimals in textual representation
    unsigned char global;       // Global index in its type
    double last;                // Last raw value from sensor (unfiltered)
    double reported;            // Last reported value
    double min_change;          // Minimum value change to report
    double max_change;          // Maximum value change to report
};

void _sensorResetTS();
void _sensorReport(unsigned char index, double value);

unsigned char sensorCount();
unsigned char magnitudeCount();
String magnitudeName(unsigned char index);
unsigned char magnitudeType(unsigned char index);
double magnitudeValue(unsigned char index);
unsigned char magnitudeIndex(unsigned char index);
String magnitudeTopic(unsigned char type);
String magnitudeTopicIndex(unsigned char index);
String magnitudeUnits(unsigned char type);

void sensorSetup();
void sensorLoop();