#include <string>

#include "Preferences.h"

#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H
class StorageManager {
   private:
    // Wifi credentials
    Preferences wifi_preferences;
    String wifi_ssid;
    String wifi_password;

   public:
    StorageManager();
    // Wifi Methods
    void setWifiCredentials(String ssid, String password);
    void getWifiCredentials(String ssid, String password);
    void resetWifiCredentials();

    void teste();
};

#endif  // STORAGE_MANAGER_H