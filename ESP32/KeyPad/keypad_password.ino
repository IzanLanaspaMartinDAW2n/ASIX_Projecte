#include <Keypad.h>
#include <EEPROM.h>
#include <ArduinoCrypto.h>

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
String input = "";

String generatePassword() {
 String pwd = "";
 for (int i = 0; i < 4; i++) {
   pwd += random(0, 10);
 }
 return pwd;
}

String encryptPassword(String password) {
 char key = 'K';
 String encrypted = password;
 for (int i = 0; i < password.length(); i++) {
   encrypted[i] = password[i] ^ key;
 }
 return encrypted;
}

void savePasswordToEEPROM(String password) {
 for (int i = 0; i < password.length(); i++) {
   EEPROM.write(i, password[i]);
 }
 EEPROM.commit();
}

String readPasswordFromEEPROM() {
 String password = "";
 for (int i = 0; i < 4; i++) {
   password += char(EEPROM.read(i));
 }
 return password;
}

void setup() {
 Serial.begin(9600);
 EEPROM.begin(512);

 String password = generatePassword();
 String encryptedPassword = encryptPassword(password);
 savePasswordToEEPROM(encryptedPassword);
 Serial.print("Contraseña generada: ");
 Serial.println(password);
 Serial.print("Contraseña cifrada: ");
 Serial.println(encryptedPassword);
}

void loop() {
 char key = keypad.getKey();

 if (key) {
   Serial.println(key);
   input += key;

   if (input.length() == 4) {
     String encryptedPassword = readPasswordFromEEPROM();
     String decryptedPassword = encryptPassword(encryptedPassword);

     if (input == decryptedPassword) {
       Serial.println("Contraseña correcta");
     } else {
       Serial.println("Contraseña incorrecta");
     }

     input = "";
   }
 }
}
