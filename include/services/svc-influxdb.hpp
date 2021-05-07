// -----------------------------------------------------------------------------
// INFLUXDB
// -----------------------------------------------------------------------------

#ifndef INFLUXDB_SUPPORT
#define INFLUXDB_SUPPORT        0               // Disable InfluxDB support by default (4.38Kb)
#endif

#ifndef INFLUXDB_ENABLED
#define INFLUXDB_ENABLED        0               // InfluxDB disabled by default
#endif

#ifndef INFLUXDB_HOST
#define INFLUXDB_HOST           ""              // Default server
#endif

#ifndef INFLUXDB_PORT
#define INFLUXDB_PORT           8086            // Default InfluxDB port
#endif

#ifndef INFLUXDB_DATABASE
#define INFLUXDB_DATABASE       ""              // Default database
#endif

#ifndef INFLUXDB_USERNAME
#define INFLUXDB_USERNAME       ""              // Default username
#endif

#ifndef INFLUXDB_PASSWORD
#define INFLUXDB_PASSWORD       ""              // Default password
#endif

// Influx V2 engine
#ifndef INFLUXDB_USE_V2
#define INFLUXDB_USE_V2         0               // InfluxDB ver.2 disabled by default
#endif

#ifndef INFLUXDB_ORG
#define INFLUXDB_ORG            ""              // Default organization
#endif

#ifndef INFLUXDB_BUCKET
#define INFLUXDB_BUCKET         ""              // Default bucket
#endif

#ifndef INFLUXDB_TOKEN
#define INFLUXDB_TOKEN          ""              // Default token
#endif


bool idbSend(const char * topic, const char * payload);
bool idbSend(const char * topic, unsigned char id, const char * payload);
bool idbEnabled();
void idbSetup();