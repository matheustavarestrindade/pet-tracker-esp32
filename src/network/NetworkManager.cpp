#include "NetworkManager.h"

#include "Instances.h"

#define SERVER_PORT 80
#define REFRESH_NETWORKS_TIME 60000
int NETWORKS_IN_RANGE;
long LAST_NETWORK_SCAN_TIMESTAMP;

NetworkManager::NetworkManager(String ssid, String password, String softAPSSID) : server(SERVER_PORT) {
    this->ssid = ssid;
    this->password = password;
    this->softAPSSID = softAPSSID;
    if (this->ssid != nullptr && this->ssid.length() > 0) {
        this->connect();
        this->WIFI_MODE = WIFI_STA;
    } else {
        this->WIFI_MODE = WIFI_AP;
        this->provide();
        this->registerAPRoutes();
        NETWORKS_IN_RANGE = 0;
        LAST_NETWORK_SCAN_TIMESTAMP = millis() - REFRESH_NETWORKS_TIME;
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
    Serial.println("[INFO] Access Point mode starting...");

    Serial.println("[INFO] Initializing SPIFFS");
    SPIFFS.begin();
    Serial.println("[INFO] SPIFFS initialized");

    Serial.println("[INFO] Starting SoftAP on SSID: " + this->softAPSSID);

    // Set captative portal
    WiFi.mode(WIFI_AP_STA);
    const byte DNS_PORT = 53;
    IPAddress apIP(192, 168, 1, 1);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    this->dnsServer.start(DNS_PORT, "*", apIP);

    WiFi.softAP(this->softAPSSID.c_str(), NULL);
    WiFi.disconnect(true, true);
    Serial.println("[INFO] SoftAP started");
    Serial.print("[INFO] Network IP address: ");
    Serial.println(WiFi.softAPIP());
}

void NetworkManager::doNetworkLoop() {
    if (this->WIFI_MODE == WIFI_AP) {
        this->dnsServer.processNextRequest();
        long currentTime = millis();
        if (currentTime - LAST_NETWORK_SCAN_TIMESTAMP > REFRESH_NETWORKS_TIME) {
            this->refreshNetworkList();
        }
    }
}

void NetworkManager::registerAPRoutes() {
    this->server.onNotFound([](AsyncWebServerRequest *request) {
        Serial.println("[INFO] Not found, Sending HTML Page");
        request->send(SPIFFS, "/index.html", "text/html; charset=utf-8");
    });
    this->server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        Serial.println("[INFO] Requested /");
        request->send(SPIFFS, "/index.html", "text/html; charset=utf-8");
    });
    this->server.on("/global.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        Serial.println("[INFO] Requested /global.css");
        request->send(SPIFFS, "/global.css", "text/css");
    });
    this->server.on("/build/bundle.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        Serial.println("[INFO] Requested /build/bundle.js");
        request->send(SPIFFS, "/build/bundle.js", "application/javascript");
    });
    this->server.on("/build/bundle.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        Serial.println("[INFO] Requested /build/bundle.css");
        request->send(SPIFFS, "/build/bundle.css", "text/css");
    });
    this->server.on("/query_networks", HTTP_GET, [](AsyncWebServerRequest *request) {
        String networks = "{ \"networks\": [";
        for (int i = 0; i < NETWORKS_IN_RANGE; i++) {
            networks += "{ \"ssid\": \"" + WiFi.SSID(i) + "\", \"rssi\": \"" + WiFi.RSSI(i) + "\", \"has_password\": " + ((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "false" : "true") + " }" + (NETWORKS_IN_RANGE - 1 == i ? "" : ",");
        }
        networks += "] }";
        Serial.println("[INFO] Requested /query_networks");
        // Serial.println(networks);
        request->send(200, "application/json", networks);
    });
    this->server.on("/connect", HTTP_POST, [](AsyncWebServerRequest *request) {
        Serial.println("[INFO] Requested /connect");
        String ssid = request->getParam("ssid", true)->value();
        String password = request->getParam("password", true)->value();
        Serial.print("[INFO] Connecting to " + ssid + " ");
        WiFi.begin(ssid.c_str(), password.c_str());
        int tryes = 0;
        while (WiFi.status() != WL_CONNECTED && tryes < 30) {
            delay(500);
            Serial.print(".");
            tryes++;
        }
        bool connected = WiFi.status() == WL_CONNECTED;
        String status = (connected ? "Connected!" : "Not connected");
        Serial.println("[INFO] " + status);
        if (connected) storage.setWifiCredentials(ssid, password);
        request->send(200, "application/json", "{ \"connected\": " + String(connected) + " }");
    });
    this->server.begin();
}

void NetworkManager::refreshNetworkList() {
    Serial.println("[INFO] Scanning for networks...");
    NETWORKS_IN_RANGE = WiFi.scanNetworks();
    Serial.println("[INFO] Found " + String(NETWORKS_IN_RANGE) + " networks");
    LAST_NETWORK_SCAN_TIMESTAMP = millis() + 60000;
}

// Public Methods

bool NetworkManager::isConnected() {
    return WiFi.status() == WL_CONNECTED && this->WIFI_MODE == WIFI_STA;
}

void NetworkManager::sendLiveCheckPacket() {
}