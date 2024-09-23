# ESPWebLibrary - Arduino Library for ESP8266 Web Server and WiFi Management

## Overview

ESPWebLibrary is a lightweight library designed to simplify web server creation and WiFi management on the ESP8266 microcontroller. It includes components to handle WiFi connections, DNS, and HTTP requests, making it easier to serve dynamic web pages and create IoT-based projects.

This library provides the following key features:
- **WiFi Management**: Easily connect to a WiFi network and set up mDNS.
- **Web Server**: Create HTTP routes and handle web requests.
- **File Serving**: Serve static and dynamic content from the ESP8266's filesystem.
- **Custom Routes**: Add routes to handle dynamic content, such as controlling hardware like LEDs.

## Features
- Simplified WiFi connection and mDNS setup.
- Easy-to-use API for defining HTTP routes and handling requests.
- Dynamic content handling with variable replacement for HTML files and inline strings.
- Lightweight and efficient, using built-in web server functionalities.

## Installation

1. Download the library and extract the contents.
2. Move the extracted folder to your Arduino libraries directory:
   - On Windows: `Documents/Arduino/libraries/`
   - On macOS: `~/Documents/Arduino/libraries/`
   - On Linux: `~/Arduino/libraries/`
3. Restart the Arduino IDE.
4. Include the library in your sketch by adding:
   ```cpp
   #include <WiFiManager.h>
   #include <WebServerManager.h>
   ```

## Dependencies
This library depends on the following Arduino libraries:
- **ESP8266WiFi.h**: For WiFi management.
- **ESP8266WebServer.h**: For creating an HTTP web server.
- **ESP8266mDNS.h**: For mDNS (Multicast DNS) to access the ESP8266 via a hostname.
- **LittleFS.h**: For serving files from the ESP8266's flash storage.

Make sure these libraries are installed through the Arduino Library Manager or manually if needed.

## Usage

### Example Code

Here’s an example of using the `ESPWebLibrary` to create a web server that can control an LED and serve dynamic content.

```cpp
#include <Arduino.h>
#include <WiFiManager.h>
#include <WebServerManager.h>

// Initialize WiFiManager with SSID and password
WiFiManager wifiManager("YourSSID", "YourPassword");

// Initialize WebServerManager on port 80
WebServerManager webServerManager(80);

bool ledStatus = false;
int ledPin = D4;

void setup() {
    Serial.begin(115200);
    
    // Configure LED pin
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    // Connect to WiFi and set up mDNS
    wifiManager.connect();
    wifiManager.setupMDNS("esp8266");

    // Define routes
    webServerManager.addRoute("/", HTTP_GET, []() {
        webServerManager.routeHandler.handleFileRead("/index.html", {{"{{LED_STATUS}}", ledStatus ? "ON" : "OFF"}});
    });

    webServerManager.addRoute("/LED=ON", HTTP_GET, []() {
        digitalWrite(ledPin, HIGH);
        ledStatus = true;
        webServerManager.routeHandler.redirectTo("/");
    });

    webServerManager.addRoute("/LED=OFF", HTTP_GET, []() {
        digitalWrite(ledPin, LOW);
        ledStatus = false;
        webServerManager.routeHandler.redirectTo("/");
    });

    // Start the server
    webServerManager.begin();
}

void loop() {
    wifiManager.updateMDNS();
    webServerManager.handleClient();
}
```

### How It Works

- **WiFiManager**: Handles WiFi connection and mDNS setup for easy access to the ESP8266.
- **WebServerManager**: Manages routes and serves both static and dynamic content.
- **Routes**: Routes are added using the `addRoute()` method. Each route can have custom logic to handle requests, such as turning an LED on/off.
- **Dynamic Content**: HTML content can contain placeholders like `{{LED_STATUS}}`, which are replaced with actual data before being served.

### Adding Routes

To add custom routes, use `webServerManager.addRoute()`. The first parameter is the route path (e.g., `/LED=ON`), the second is the HTTP method (e.g., `HTTP_GET`), and the third is the handler function.

Example:
```cpp
webServerManager.addRoute("/myroute", HTTP_GET, []() {
    webServerManager.routeHandler.handleString("text/html", "<h1>Hello World</h1>");
});
```

### Serving Files with Dynamic Content

You can serve files from the filesystem with placeholders that will be replaced with actual values at runtime. Use `webServerManager.routeHandler.handleFileRead()` to serve such files.

Example:
```cpp
webServerManager.routeHandler.handleFileRead("/index.html", {{"{{PLACEHOLDER}}", "ReplacementText"}});
```

### Redirecting

To redirect from one route to another, use `webServerManager.routeHandler.redirectTo()`.

Example:
```cpp
webServerManager.routeHandler.redirectTo("/newPage");
```

## File Structure

The library consists of two main components:

1. **WiFiManager**: Manages WiFi connection and mDNS.
2. **WebServerManager**: Manages HTTP requests and response handling.

### WiFiManager

Responsible for:
- Connecting to WiFi using credentials provided during initialization.
- Setting up mDNS for easy access to the device.

### WebServerManager

Responsible for:
- Creating and handling routes.
- Serving static and dynamic files from the filesystem.
- Managing HTTP requests and sending responses.

## API Reference

### WiFiManager

```cpp
WiFiManager(String ssid, String password)
```
- **ssid**: Your WiFi network's SSID.
- **password**: Your WiFi network's password.

```cpp
void connect()
```
- Connects to the WiFi network.

```cpp
void setupMDNS(String hostname)
```
- Sets up mDNS with the specified hostname.

```cpp
void updateMDNS()
```
- Updates mDNS services.

### WebServerManager

```cpp
WebServerManager(int port)
```
- **port**: The port to run the web server on (typically `80`).

```cpp
void begin()
```
- Starts the web server.

```cpp
void handleClient()
```
- Handles incoming web requests.

```cpp
void addRoute(String route, HTTPMethod method, std::function<void()> handler)
```
- **route**: The URL path to handle.
- **method**: HTTP method (e.g., `HTTP_GET`, `HTTP_POST`).
- **handler**: A lambda function or callback that handles requests to this route.

### RouteHandler

```cpp
bool handleFileRead(String path, std::map<String, String> variables)
```
- **path**: The path to the file to be served.
- **variables**: A map of placeholder variables and their replacement values for dynamic content.

```cpp
void handleString(String contentType, String content, std::map<String, String> variables)
```
- **contentType**: The content type (e.g., `text/html`).
- **content**: The HTML or other content to be served.
- **variables**: A map of placeholder variables and their replacement values.

```cpp
void redirectTo(String uri)
```
- **uri**: The URL to redirect the client to.

## License

This project is licensed under the MIT License. See the `LICENSE` file for more details.

---