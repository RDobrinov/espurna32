// -----------------------------------------------------------------------------
// WEB
// -----------------------------------------------------------------------------

#ifndef WEB_SUPPORT
#define WEB_SUPPORT                 1           // Enable web support (http, api, 121.65Kb)
#endif

#ifndef WEB_EMBEDDED
#define WEB_EMBEDDED                1           // Build the firmware with the web interface embedded in
#endif

// Requires ESPAsyncTCP to be built with ASYNC_TCP_SSL_ENABLED=1 and Arduino Core version >= 2.4.0
// XXX: This is not working at the moment!! Pending https://github.com/me-no-dev/ESPAsyncTCP/issues/95
#ifndef WEB_SSL_ENABLED
#define WEB_SSL_ENABLED             0           // Use HTTPS web interface
#endif

#ifndef WEB_USERNAME
#define WEB_USERNAME                "root"     // HTTP username
#endif

#ifndef WEB_FORCE_PASS_CHANGE
#define WEB_FORCE_PASS_CHANGE       0           // Force the user to change the password if default one
#endif

#ifndef WEB_PORT
#define WEB_PORT                    80          // HTTP port
#endif

// Defining a WEB_REMOTE_DOMAIN will enable Cross-Origin Resource Sharing (CORS)
// so you will be able to login to this device from another domain. This will allow
// you to manage all ESPurna devices in your local network from a unique installation
// of the web UI. This installation could be in a local server (a Raspberry Pi, for instance)
// or in the Internet. Since the WebUI is just one compressed file with HTML, CSS and JS
// there are no special requirements. Any static web server will do (NGinx, Apache, Lighttpd,...).
// The only requirement is that the resource must be available under this domain.
#ifndef WEB_REMOTE_DOMAIN
#define WEB_REMOTE_DOMAIN           "http://espurna.io"
#endif

#include <FreeRTOS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

using web_body_callback_f = std::function<bool(AsyncWebServerRequest*, uint8_t* data, size_t len, size_t index, size_t total)>;
using web_request_callback_f = std::function<bool(AsyncWebServerRequest*)>;

void webBodyRegister(web_body_callback_f callback);
void webRequestRegister(web_request_callback_f callback);

bool webAuthenticate(AsyncWebServerRequest *request);
void webLog(AsyncWebServerRequest *request);
AsyncWebServer * webServer();
void webSetup();