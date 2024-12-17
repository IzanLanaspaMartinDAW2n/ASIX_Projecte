#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "index.h"
#include "notasMusicales.h"

#define LED_PIN 18   // ESP32 pin GPIO18 connected to LED
#define LED_PIN2 19  // ESP32 pin GPIO19 connected to second LED (for police mode)
#define BUZZER_PIN 16  

#define SCREEN_WIDTH 128 // OLED width,  in pixels
#define SCREEN_HEIGHT 64 // OLED height, in pixels

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const char *ssid = "AP_asix";     // CHANG
const char *password = "AP_asix2023";  // CHANGE IT

AsyncWebServer server(80);

// Variable to track the current mode
String mode = "OFF";

void updateOLED() {
  oled.clearDisplay();          // Clear the screen
  oled.setTextSize(1);          // Set text size
  oled.setTextColor(WHITE);     // Set text color
  oled.setCursor(0, 2);         // Set cursor to start position
  oled.println("Current Mode:"); // Print a label
  oled.setCursor(0, 20);        // Move cursor down
  oled.println(mode);           // Print the current mode
  oled.display();               // Update the display
}

String getHTML() {
  String html = webpage;                                  
  html.replace("%MODE%", mode);  // Update the mode status
  return html;
}

// Function to handle police mode
void policeMode() {
  mode = "POLICE";
  updateOLED();
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
  mode = "OFF";
  updateOLED();
}

void activateBuzzer() {
  mode = "BUZZER";
  updateOLED();
  unsigned long startTime = millis(); 
  while (millis() - startTime < 3000) { 
    digitalWrite(BUZZER_PIN, HIGH);  
    delay(500);                      
    digitalWrite(BUZZER_PIN, LOW);   
    delay(500);                      
  }
  mode = "OFF";
  updateOLED();
}
int melody[] = {
  NOTE_E7, NOTE_G7, NOTE_A7, NOTE_F7, NOTE_G7, NOTE_E7,
  NOTE_C7, NOTE_D7, NOTE_B7, NOTE_G7, NOTE_F7, NOTE_E7, NOTE_A7, NOTE_G7,
};

// Duración de las notas (4 = negra, 8 = corchea, etc.)
int noteDurations[] = {
  4, 8, 8, 4, 4, 4,
  4, 8, 8, 4, 8, 8, 4, 4
};

void playHarryPotter() {
  mode = "Harry";
   updateOLED();
  for (int thisNote = 0; thisNote < sizeof(melody) / sizeof(melody[0]); thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZER_PIN, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(BUZZER_PIN);
  }
  mode = "OFF";
  updateOLED();
}
void initSetup(){
  Serial.begin(9600);
  // Set pins as outputs
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
 
  digitalWrite(LED_PIN, LOW);
  digitalWrite(LED_PIN2, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("failed to start SSD1306 OLED"));
    while (1);
  }
  // Initialize the display with the default mode
  updateOLED();

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
}

void serverGetters(){
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("ESP32 Web Server: New request received:");
    Serial.println("GET /");
    request->send(200, "text/html", getHTML());
  });

  // Route to turn LED on
  server.on("/led1/on", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("ESP32 Web Server: New request received:");
    Serial.println("GET /led1/on");
    mode = "ON";
    digitalWrite(LED_PIN, HIGH);
    updateOLED();
    request->send(200, "text/html", getHTML());
  });

  // Route to turn LED off
  server.on("/led1/off", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("ESP32 Web Server: New request received:");
    Serial.println("GET /led1/off");
    mode = "OFF";
    digitalWrite(LED_PIN, LOW);
    updateOLED();
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
    playHarryPotter();  
    request->send(200, "text/html", getHTML());
  });
}

void setup() {
  initSetup();
  serverGetters();
  server.begin(); 
}

void loop() {
}
