/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-controls-led-via-web
 */

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "index.h"  // Include the index.h file

#define LED_PIN 18   // ESP32 pin GPIO18 connected to LED
#define LED_PIN2 19  // ESP32 pin GPIO19 connected to second LED (for police mode)
#define BUZZER_PIN 21  


const char *ssid = "AP_asix";     // CHANG
const char *password = "AP_asix2023";  // CHANGE IT

AsyncWebServer server(80);

int LED_state = LOW;

String getHTML() {
  String html = webpage;                                  // Use the HTML content from the index.h file
  html.replace("%LED_STATE%", LED_state ? "ON" : "OFF");  // update the led state

  return html;
}

// Function to handle police mode
void policeMode() {
  for (int i = 0; i < 10; i++) {  // Loop for 10 seconds
    digitalWrite(LED_PIN, HIGH);   // Turn on LED 1
    digitalWrite(LED_PIN2, LOW);   // Turn off LED 2
    delay(500);                    // Wait 500 ms
    digitalWrite(LED_PIN, LOW);    // Turn off LED 1
    digitalWrite(LED_PIN2, HIGH);  // Turn on LED 2
    delay(500);                    // Wait 500 ms
  }
  // Turn off both LEDs after the mode ends
  digitalWrite(LED_PIN, LOW);
  digitalWrite(LED_PIN2, LOW);
}

void activateBuzzer() {
  unsigned long startTime = millis(); // Tiempo de inicio
  while (millis() - startTime < 3000) { // Ejecutar por 3 segundos
    digitalWrite(BUZZER_PIN, HIGH);  // Encender zumbador
    delay(500);                      // Esperar 500 ms
    digitalWrite(BUZZER_PIN, LOW);   // Apagar zumbador
    delay(500);                      // Esperar 500 ms
  }
}

void setup() {
  Serial.begin(9600);

  // Set pins as outputs
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
 
  digitalWrite(LED_PIN, LED_state);
  digitalWrite(LED_PIN2, LOW);
  digitalWrite(BUZZER_PIN, LOW);

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
    Serial.println("ESP32 Web Server: New request received:");
    Serial.println("GET /");
    request->send(200, "text/html", getHTML());
  });

  // Route to turn LED on
  server.on("/led1/on", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("ESP32 Web Server: New request received:");
    Serial.println("GET /led1/on");
    LED_state = HIGH;
    digitalWrite(LED_PIN, LED_state);
    request->send(200, "text/html", getHTML());
  });

  // Route to turn LED off
  server.on("/led1/off", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("ESP32 Web Server: New request received:");
    Serial.println("GET /led1/off");
    LED_state = LOW;
    digitalWrite(LED_PIN, LED_state);
    request->send(200, "text/html", getHTML());
  });

  // Route to activate police mode
  server.on("/led/police", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("ESP32 Web Server: New request received:");
    Serial.println("GET /led/police");
    request->send(200, "text/html", getHTML());  // Send the current page first
    policeMode();  // Execute police mode
  });

  server.on("/buzzer", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("GET /buzzer");
    activateBuzzer();  // Llamar a la función para activar el zumbador
    request->send(200, "text/html", getHTML());
  });

  // Start the server
  server.begin();
}

void loop() {
  // Empty loop as the server handles requests asynchronously
}
