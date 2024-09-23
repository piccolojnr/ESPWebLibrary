#ifndef ROUTEHANDLER_H
#define ROUTEHANDLER_H

#include <ESP8266WebServer.h>
#include <functional>
#include <map>

class RouteHandler
{
public:
    RouteHandler(ESP8266WebServer &server); // Constructor
    void addRoute(String uri, HTTPMethod method, std::function<void()> handler);
    void setupRoutes(); // Initialize all routes

    bool handleFileRead(String path, std::map<String, String> variables);

    void handleString(String contentType, String content, std::map<String, String> variables); // Handle string content for web pages

    void redirectTo(String uri); // Redirect to another URI
private:
    ESP8266WebServer &server;                              // Reference to the web server
    std::map<String, std::function<void()>> routeHandlers; // Map of URI to route handlers
    String getContentType(String filename);                // Get MIME type of file
};

#endif
