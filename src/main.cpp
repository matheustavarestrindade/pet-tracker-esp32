#include <Arduino.h>

#include "storage/StorageManager.h"

StorageManager storage;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);

    // Debuging purposes
    storage.setWifiCredentials("Rede trindade", "cpmn2018");

    String wifi_ssid = "";
    String wifi_password = "";

    // Load wifi credentials
    storage.getWifiCredentials(&wifi_ssid, &wifi_password);
}

void loop() {
    // put your main code here, to run repeatedly:
}