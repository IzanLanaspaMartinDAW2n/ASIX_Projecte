#include <Keypad.h>

const uint8_t ROWS = 4;
const uint8_t COLS = 4;

char keys[ROWS][COLS] = {
 { '1', '2', '3', 'A' },
 { '4', '5', '6', 'B' },
 { '7', '8', '9', 'C' },
 { '*', '0', '#', 'D' }
};

uint8_t colPins[COLS] = { 16, 4, 2, 15 };
uint8_t rowPins[ROWS] = { 19, 18, 5, 17 };

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String password = "1234";
String input = "";
void setup() {
 Serial.begin(9600);
}

void loop() {
 char key = keypad.getKey();
 if (key) {
   Serial.println(key);
   input += key;
  
   if (input.length() == 4) {
     if (input == password) {
       Serial.println("Contraseña correcta");
     } else {
       Serial.println("Contraseña incorrecta");
     }
     input = "";
   }
 }
}
