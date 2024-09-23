#include "RouteHandler.h"
#include <LittleFS.h>
#include <Arduino.h>

RouteHandler::RouteHandler(ESP8266WebServer &server) : server(server)
{
}

void RouteHandler::addRoute(String uri, HTTPMethod method, std::function<void()> handler)
{
    if (method == HTTP_GET)
    {
        server.on(uri.c_str(), HTTP_GET, handler);
    }
    else if (method == HTTP_POST)
    {
        server.on(uri.c_str(), HTTP_POST, handler);
    }
    // Add other methods if needed
}

void RouteHandler::setupRoutes()
{
    for (auto const &route : routeHandlers)
    {
        server.on(route.first, route.second); // Set each route's handler
    }

    server.onNotFound([this]() { // Handle undefined routes
        if (!handleFileRead("/404.html", {}))
            server.send(404, "text/plain", "404: Not Found");
    });
}

String RouteHandler::getContentType(String filename)
{
    if (filename.endsWith(".html"))
        return "text/html";
    else if (filename.endsWith(".css"))
        return "text/css";
    else if (filename.endsWith(".js"))
        return "application/javascript";
    else if (filename.endsWith(".ico"))
        return "image/x-icon";
    else if (filename.endsWith(".gz"))
        return "application/x-gzip";
    return "text/plain";
}

std::map<String, String> fileCache; // Cache for files' content

bool RouteHandler::handleFileRead(String path, std::map<String, String> variables)
{
    if (path.endsWith("/"))
        path += "index.html";

    if (fileCache.find(path) != fileCache.end())
    {
        handleString(getContentType(path), fileCache[path], variables);
        return true;
    }

    if (LittleFS.exists(path))
    {
        File file = LittleFS.open(path, "r");
        String content = file.readString();
        fileCache[path] = content;
        handleString(getContentType(path), content, variables);
        file.close();
        return true;
    }
    return false;
}

void RouteHandler::handleString(String contentType, String content, std::map<String, String> variables)
{
    for (auto const &variable : variables)
    {
        content.replace(variable.first, variable.second);
    }
    server.send(200, contentType, content);
}

void RouteHandler::redirectTo(String uri)
{
    server.sendHeader("Location", uri, true);
    server.send(302, "text/plain", "");
}
