#include <Arduino.h>

#include "Instances.h"
#include "network/NetworkManager.h"
#include "storage/StorageManager.h"

StorageManager storage;
NetworkManager* network;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);

    Serial.println("[INFO] Starting pet-tracker");
    // Debuging purposes
    // storage.setWifiCredentials("Rede trindade", "cpmn2018");

    String wifi_ssid = "";
    String wifi_password = "";
    String softAPSSID = "Pet Tracker Configuração";

    // Load wifi credentials
    storage.getWifiCredentials(wifi_ssid, wifi_password );

    network = new NetworkManager(wifi_ssid, wifi_password, softAPSSID);
}

void loop() {
    // put your main code here, to run repeatedly:
    network->doNetworkLoop();
}