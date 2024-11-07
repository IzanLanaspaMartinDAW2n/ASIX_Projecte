#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED width,  in pixels
#define SCREEN_HEIGHT 64 // OLED height, in pixels

// create an OLED display object connected to I2C
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
 Serial.begin(9600);

 // initialize OLED display with I2C address 0x3C
 if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
   Serial.println(F("failed to start SSD1306 OLED"));
   while (1);
 }

 delay(2000);         // wait two seconds for initializing
 oled.clearDisplay(); // clear display

 oled.setTextSize(1);         // set text size
 oled.setTextColor(WHITE);    // set text color
 oled.setCursor(0, 2);       // set position to display (x,y)
 oled.println("Cooktech"); // set text
 oled.display();              // display on OLED
}

void loop() {
}
