#include "WebServerManager.h"
#include <Arduino.h>

WebServerManager::WebServerManager(int port) : server(port), routeHandler(server)
{
}

void WebServerManager::begin()
{
    if (!LittleFS.begin())
    {
        Serial.println("An Error has occurred while mounting LittleFS");
        return;
    }
    Serial.println("LittleFS mounted successfully");

    routeHandler.setupRoutes(); // Initialize all routes

    server.begin();
    Serial.println("HTTP server started");
}

void WebServerManager::handleClient()
{
    server.handleClient();
}

void WebServerManager::addRoute(const String &route, HTTPMethod method, std::function<void()> handler)
{
    routeHandler.addRoute(route, method, handler);
}
