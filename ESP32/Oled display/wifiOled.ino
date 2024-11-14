#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Configuración de la pantalla OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1  // GPIO para el reset, -1 si no se utiliza
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Configuración de la red WiFi
const char* ssid = "AP_asix";
const char* password = "AP_asix2023";

void setup() {
  // Iniciar la conexión serial
  Serial.begin(115200);

  // Iniciar la pantalla OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // La dirección I2C típica de la pantalla OLED es 0x3C
    Serial.println(F("Fallo en la inicialización del OLED"));
    for(;;); // Si la pantalla falla, detener el programa
  }
  display.clearDisplay();
  display.setTextSize(1); // Tamaño de texto
  display.setTextColor(SSD1306_WHITE); // Color de texto
  display.setCursor(0,0);
  display.println("Conectando a WiFi...");
  display.display();

  // Conexión a la red WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Limpia y muestra la IP obtenida
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Conectado a WiFi");
  display.print("IP: ");
  display.println(WiFi.localIP()); // Obtiene la IP
  display.display();

  // También muestra la IP en el monitor serial
  Serial.println();
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // No hace falta nada en el loop para este caso
}
