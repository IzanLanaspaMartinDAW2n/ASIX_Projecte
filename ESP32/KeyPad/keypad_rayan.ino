#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
 {'1','2','3','A'},
 {'4','5','6','B'},
 {'7','8','9','C'},
 {'*','0','#','D'}
};

byte rowPins[ROWS] = {19, 18, 5, 17};
byte colPins[COLS] = {16, 4, 0, 2};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

const int ledPin = 13;

void setup() {
 pinMode(ledPin, OUTPUT);
 Serial.begin(115200);
}

void loop() {
 char key = keypad.getKey();
 if (key) {
   Serial.println(key);
   if (key == 'A') {
     digitalWrite(ledPin, HIGH);
   } else {
     digitalWrite(ledPin, LOW);
   }
 }
}
#include <Keypad.h>

// Configuració del KeyPad
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
 {'1','2','3','A'},
 {'4','5','6','B'},
 {'7','8','9','C'},
 {'*','0','#','D'}
};

byte rowPins[ROWS] = {19, 18, 5, 17};
byte colPins[COLS] = {16, 4, 0, 2};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

const int ledPin = 13;

void setup() {
 pinMode(ledPin, OUTPUT);
 Serial.begin(115200);
}

void loop() {
 char key = keypad.getKey();
 if (key) {
   Serial.println(key);
   if (key == 'A') {
     digitalWrite(ledPin, HIGH);
   } else {
     digitalWrite(ledPin, LOW);
   }
 }
}
