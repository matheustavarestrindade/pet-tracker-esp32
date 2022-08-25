#include "StorageManager.h"

StorageManager::StorageManager(){};

/*
    Wifi credentials are stored in the following format:
*/

// Set wifi credentials
void StorageManager::setWifiCredentials(String ssid, String password) {
    this->wifi_preferences.begin("wifi", false);
    this->wifi_ssid = ssid;
    this->wifi_password = password;
    this->wifi_preferences.putString("wifi_ssid", this->wifi_ssid);
    this->wifi_preferences.putString("wifi_password", this->wifi_password);
    this->wifi_preferences.end();
}

// Get wifi credentials
void StorageManager::getWifiCredentials(String ssid, String password) {
    this->wifi_preferences.begin("wifi", false);
    this->wifi_ssid = this->wifi_preferences.getString("wifi_ssid", "");
    this->wifi_password = this->wifi_preferences.getString("wifi_password", "");
    ssid = this->wifi_ssid;
    password = this->wifi_password;
    this->wifi_preferences.end();
}

// Reset wifi credentials
void StorageManager::resetWifiCredentials() {
    this->wifi_preferences.begin("wifi", false);
    this->wifi_preferences.clear();
    this->wifi_preferences.end();
}