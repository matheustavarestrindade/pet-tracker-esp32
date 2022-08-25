#include "NetworkManager.h"
#define SERVER_PORT 80

NetworkManager::NetworkManager(String ssid, String password, String softAPSSID) : server(SERVER_PORT) {
    this->ssid = ssid;
    this->password = password;
    this->softAPSSID = softAPSSID;
    if (this->ssid != nullptr && this->ssid.length() > 0) {
        this->connect();
    } else {
        this->provide();
        this->registerSTARoutes();
    }
};

// AP Methods

bool NetworkManager::connect() {
    WiFi.begin(this->ssid.c_str(), this->password.c_str());
    int tryes = 0;
    while (WiFi.status() != WL_CONNECTED && tryes < 30) {
        delay(500);
        Serial.print(".");
        tryes++;
    }
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        return true;
    } else {
        Serial.println("");
        Serial.println("WiFi connection failed");
        return false;
    }
}

// STA Methodss

void NetworkManager::provide() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(this->ssid.c_str(), this->password.c_str());

    Serial.println("Access Point mode started");
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());

    // Set captative portal
    const byte DNS_PORT = 53;
    IPAddress apIP(192, 168, 1, 1);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(this->softAPSSID.c_str(), NULL);
    WiFi.disconnect(true, true);
    this->dnsServer.start(DNS_PORT, "*", apIP);
}

int networksInRange;
long lastNetworkScan = millis();

void NetworkManager::registerSTARoutes() {
    this->server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", "text/plain");
    });
    this->server.on("/global.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/global.css", "text/plain");
    });
    this->server.on("/build/bundle.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/build/bundle.js", "text/plain");
    });
    this->server.on("/build/bundle.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/build/bundle.css", "text/plain");
    });
    this->server.begin();
}

void NetworkManager::refreshNetworkList() {
    Serial.println("[INFO] Scanning for networks...");
    networksInRange = WiFi.scanNetworks();
    Serial.println("[INFO] Found " + String(networksInRange) + " networks");
    lastNetworkScan = millis() + 30000;
}