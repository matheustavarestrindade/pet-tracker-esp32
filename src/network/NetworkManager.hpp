#include <DNSServer.h>

#include "SPIFFS.h"
#include "WiFi.h"
#define SERVER_PORT 80

class NetworkManager {
   private:
    // Wifi credentials
    String ssid;
    String password;
    String softAPSSID;
    DNSServer dnsServer;
    AsyncWebServer server(SERVER_PORT);

    // AP Methods
    bool connect();
    // STA Methods
    void refreshNetworkList();
    void provide();
    void registerSTARoutes();

   public:
    NetworkManager(String ssid, String password, String softAPSSID);
    void doNetworkLoop();
}