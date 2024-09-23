#include <Arduino.h>
#include <WiFiManager.h>
#include <WebServerManager.h>

// Create WiFiManager object with WiFi credentials
WiFiManager wifiManager("piccolo", "getters64@");

// Create WebServerManager object to handle HTTP requests (port 80)
WebServerManager webServerManager(80);

// Global variables for managing LED state and pin
bool ledStatus = false;
int ledPin = D4;

/**
 * @brief Initialization function for setting up WiFi, mDNS, and web server routes.
 *
 * This function:
 *  - Initializes the serial communication.
 *  - Configures the LED pin for output.
 *  - Connects to WiFi using the WiFiManager class.
 *  - Sets up mDNS with the hostname "esp8266".
 *  - Adds various routes for controlling the LED and serving dynamic content.
 *  - Starts the web server.
 */
void setup()
{
    // Initialize serial communication for debugging
    Serial.begin(115200);

    // Set the LED pin as an output pin
    pinMode(ledPin, OUTPUT);

    // Turn off the LED initially
    digitalWrite(ledPin, LOW);

    // Connect to WiFi and setup mDNS (Multicast DNS)
    wifiManager.connect();
    wifiManager.setupMDNS("esp8266");

    // Add route for root URL ("/") to serve index.html with dynamic content
    webServerManager.addRoute("/",
                              HTTP_GET,
                              []()
                              {
                                  // Serve index.html and replace {{LED_STATUS}} with actual status
                                  webServerManager.routeHandler.handleFileRead("/index.html",
                                                                               {{"{{LED_STATUS}}", ledStatus ? "ON" : "OFF"}});
                              });

    // Add route "/2" to serve inline HTML string with dynamic LED status
    webServerManager.addRoute("/2",
                              HTTP_GET,
                              []()
                              {
                                  // Serve an inline HTML page with dynamic LED status
                                  webServerManager.routeHandler.handleString("text/html",
                                                                             "<!DOCTYPE html><html><head><title>ESP8266 Web Server</title></head><body><h1>ESP8266 Web Server</h1><p>LED Status: {{LED_STATUS}}</p><a href=\"/LED=ON\">Turn LED ON</a><br><a href=\"/LED=OFF\">Turn LED OFF</a><br><a href=\"/LED=TOGGLE\">Toggle LED</a></body></html>",
                                                                             {{"{{LED_STATUS}}", ledStatus ? "ON" : "OFF"}});
                              });

    // Add route "/LED=ON" to turn the LED ON
    webServerManager.addRoute("/LED=ON",
                              HTTP_GET,
                              []()
                              {
                                  // Turn on LED and update status
                                  digitalWrite(ledPin, HIGH);
                                  ledStatus = true;
                                  // Redirect back to the home page
                                  webServerManager.routeHandler.redirectTo("/");
                              });

    // Add route "/LED=OFF" to turn the LED OFF
    webServerManager.addRoute("/LED=OFF",
                              HTTP_GET,
                              []()
                              {
                                  // Turn off LED and update status
                                  digitalWrite(ledPin, LOW);
                                  ledStatus = false;
                                  // Redirect back to the home page
                                  webServerManager.routeHandler.redirectTo("/");
                              });

    // Add route "/LED=TOGGLE" to toggle the LED state
    webServerManager.addRoute("/LED=TOGGLE",
                              HTTP_GET,
                              []()
                              {
                                  // Toggle the LED status and update pin output
                                  ledStatus = !ledStatus;
                                  digitalWrite(ledPin, ledStatus);
                                  // Redirect back to the home page
                                  webServerManager.routeHandler.redirectTo("/");
                              });

    // Start the web server
    webServerManager.begin();
}

/**
 * @brief Main loop for handling web requests and updating mDNS.
 *
 * This function runs continuously, ensuring that:
 *  - mDNS services are updated.
 *  - Web server handles any incoming HTTP requests.
 */
void loop()
{
    // Update mDNS (to ensure the hostname is discoverable)
    wifiManager.updateMDNS();

    // Handle incoming web requests
    webServerManager.handleClient();
}
