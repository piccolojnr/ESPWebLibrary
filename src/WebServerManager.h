#ifndef WEBSERVERMANAGER_H
#define WEBSERVERMANAGER_H

#include <ESP8266WebServer.h>
#include <LittleFS.h>
#include "RouteHandler.h"

class WebServerManager
{
public:
    WebServerManager(int port); // Constructor
    void begin();               // Start the web server
    void addRoute(const String &route, HTTPMethod method, std::function<void()> handler);
    void handleClient();       // Handle incoming client requests
    ESP8266WebServer server;   // Web server object
    RouteHandler routeHandler; // Route handler object

private:
};

#endif
