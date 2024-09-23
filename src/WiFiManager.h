#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>

class WiFiManager
{
public:
    WiFiManager(const char *ssid, const char *password); // Constructor
    void connect();                                      // Connect to WiFi
    void setupMDNS(const char *hostname);                // Setup mDNS
    void updateMDNS();                                   // Update mDNS responder
    IPAddress getLocalIP();                              // Get local IP address

private:
    ESP8266WiFiMulti wifiMulti; // WiFi multi instance
    const char *ssid;           // WiFi SSID
    const char *password;       // WiFi password
};

#endif
