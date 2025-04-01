#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <HTTPClient.h>


const char* ssid = "AP_asix";
const char* password = "AP_asix2023";


IPAddress local_IP(10, 0, 110, 244);
IPAddress gateway(10, 0, 110, 1);
IPAddress subnet(255, 255, 255, 0);


AsyncWebServer server(80);
String cameraIP = "";


void setup() {
    Serial.begin(9600);


    if (!WiFi.config(local_IP, gateway, subnet)) {
        Serial.println("Error al configurar la IP");
    }
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Conectando...");
    }
    Serial.println("Conectado a WiFi: " + WiFi.localIP().toString());


    server.on("/thermostat", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request->hasParam("temp")) {
            float targetTemp = request->getParam("temp")->value().toFloat();
            Serial.println("Modo Termostato: " + String(targetTemp) + "C");
        }
        request->send(200, "text/plain", "OK");
    });


    server.on("/cook", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request->hasParam("time")) {
            int cookTime = request->getParam("time")->value().toInt();
            Serial.println("Modo Cook: " + String(cookTime) + " min");
        }
        request->send(200, "text/plain", "OK");
    });


    server.on("/set_camera", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request->hasParam("ip")) {
            cameraIP = request->getParam("ip")->value();
            Serial.println("IP Cámara configurada: " + cameraIP);
        }
        request->send(200, "text/plain", "IP configurada");
    });


    server.on("/camera", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (cameraIP == "") {
            request->send(400, "text/plain", "IP de la cámara no configurada");
            return;
        }


        HTTPClient http;
        http.begin("http://" + cameraIP + "/capture");
        int httpCode = http.GET();
        if (httpCode == 200) {
            WiFiClient *client = http.getStreamPtr();
            request->send(*client, "image/jpeg", http.getSize());
        } else {
            request->send(500, "text/plain", "Error en captura");
        }
        http.end();
    });


    server.begin();
}


void loop() {}
