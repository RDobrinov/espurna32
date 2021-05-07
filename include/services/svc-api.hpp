// -----------------------------------------------------------------------------
// API
// -----------------------------------------------------------------------------
#ifdef WEB_SUPPORT
    #ifndef API_SUPPORT
    #define API_SUPPORT                 1           // API (REST & RPC) support built in
    #endif
#else
    #ifdef API_SUPPORT
    #undef API_SUPPORT
    #define API_SUPPORT                 0           // API (REST & RPC) support built in
    #endif
#endif

// This will only be enabled if WEB_SUPPORT is 1 (this is the default value)
#ifndef API_ENABLED
#define API_ENABLED                 0           // Do not enable API by default
#endif

#ifndef API_KEY
#define API_KEY                     ""          // Do not enable API by default. WebUI will automatically generate the key
#endif

#ifndef API_RESTFUL
#define API_RESTFUL                 1           // A restful API requires changes to be issued as PUT requests
                                                // Setting this to 0 will allow using GET to change relays, for instance
#endif

#ifndef API_BUFFER_SIZE
#define API_BUFFER_SIZE             64          // Size of the buffer for HTTP GET API responses
#endif

#ifndef API_REAL_TIME_VALUES
#define API_REAL_TIME_VALUES        0           // Show filtered/median values by default (0 => median, 1 => real time)
#endif
// -----------------------------------------------------------------------------
// API
// -----------------------------------------------------------------------------
#include <functional>
using api_get_callback_f = std::function<void(char * buffer, size_t size)>;
using api_put_callback_f = std::function<void(const char * payload)> ;

void apiRegister(const char * key, api_get_callback_f getFn, api_put_callback_f putFn = nullptr);
void apiSetup();