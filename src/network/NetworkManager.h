#include <DNSServer.h>

#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "WiFi.h"

#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H
class NetworkManager {
   private:
    // Wifi credentials
    String ssid;
    String password;
    String softAPSSID;
    DNSServer dnsServer;
    AsyncWebServer server;
    int WIFI_MODE;
    // AP Methods
    bool connect();
    // STA Methods
    void refreshNetworkList();
    void provide();
    void registerAPRoutes();

   public:
    NetworkManager(String ssid, String password, String softAPSSID);
    void doNetworkLoop();
    bool isConnected();
    void sendLiveCheckPacket();
};
#endif