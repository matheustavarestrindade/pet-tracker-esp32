#include <DNSServer.h>

#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "WiFi.h"

class NetworkManager {
   private:
    // Wifi credentials
    String ssid;
    String password;
    String softAPSSID;
    DNSServer dnsServer;
    AsyncWebServer server;

    // AP Methods
    bool connect();
    // STA Methods
    void refreshNetworkList();
    void provide();
    void registerSTARoutes();

   public:
    NetworkManager(String ssid, String password, String softAPSSID);
    void doNetworkLoop();
};