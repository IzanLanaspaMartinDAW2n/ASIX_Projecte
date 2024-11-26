#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "index.h"  // Include the index.h file

#define RED_LED_PIN 18    // ESP32 pin GPIO18 connected to Red LED
#define BLUE_LED_PIN 19   // ESP32 pin GPIO19 connected to Blue LED

const char *ssid = "AP_asix";     // CHANGE IT
const char *password = "AP_asix2023";  // CHANGE IT

AsyncWebServer server(80);

int currentTemperature = 18;  // Initial temperature

String getHTML() {
  String html = webpage;  // Use the HTML content from the index.h file
  html.replace("%TEMPERATURE%", String(currentTemperature));  // update the temperature

  return html;
}

void controlLEDs() {
  if (currentTemperature >= 16) {
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(BLUE_LED_PIN, LOW);
  } else {
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(BLUE_LED_PIN, HIGH);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  controlLEDs();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Print the ESP32's IP address
  Serial.print("ESP32 Web Server's IP address: ");
  Serial.println(WiFi.localIP());

  // Home page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", getHTML());
  });

  // Route to increase temperature
  server.on("/increase", HTTP_GET, [](AsyncWebServerRequest *request) {
    currentTemperature++;
    controlLEDs();
    request->send(200, "text/html", getHTML());
  });

  // Route to decrease temperature
  server.on("/decrease", HTTP_GET, [](AsyncWebServerRequest *request) {
    currentTemperature--;
    controlLEDs();
    request->send(200, "text/html", getHTML());
  });

  // Start the server
  server.begin();
}

void loop() {
  // Your code here
}
