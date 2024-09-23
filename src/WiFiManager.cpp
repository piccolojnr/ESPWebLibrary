#include "WiFiManager.h"
#include <Arduino.h>

WiFiManager::WiFiManager(const char *ssid, const char *password)
{
    this->ssid = ssid;
    this->password = password;
    wifiMulti.addAP(ssid, password); // Add network credentials
}

void WiFiManager::connect()
{
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println(" ...");

    int i = 0;
    while (wifiMulti.run() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(++i);
        Serial.print(' ');
    }

    Serial.println('\n');
    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());
}

void WiFiManager::setupMDNS(const char *hostname)
{
    if (MDNS.begin(hostname))
    {
        Serial.println("mDNS responder started");
    }
    else
    {
        Serial.println("Error setting up MDNS responder!");
    }
}

void WiFiManager::updateMDNS()
{
    MDNS.update();
}

IPAddress WiFiManager::getLocalIP()
{
    return WiFi.localIP();
}
