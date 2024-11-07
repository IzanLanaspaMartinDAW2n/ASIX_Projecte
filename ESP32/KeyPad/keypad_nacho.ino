// Importa la librería Keypad
#include <Keypad.h>

// Define número de filas y columnas
const uint8_t ROWS = 4;
const uint8_t COLS = 4;

// Distribución de teclas
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

// Pines correspondientes a las filas y columnas
uint8_t colPins[COLS] = { 16, 4, 2, 15 };  // Pines de columnas
uint8_t rowPins[ROWS] = { 19, 18, 5, 17 };  // Pines de filas

// Crea objeto Keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Pines para el LED y buzzer
int ledPin = 13;      // Pin del LED
int buzzerPin = 12;   // Pin del buzzer

// Variables para almacenar la clave ingresada
String inputCode = "";

// Configuración inicial
void setup() {
  Serial.begin(9600);

  // Configura los pines del LED y buzzer como salida
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
 
  // Asegúrate de que el LED y buzzer estén apagados inicialmente
  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);
}

// Bucle principal
void loop() {
  // Obtiene la tecla presionada
  char key = keypad.getKey();

  // Si se presiona una tecla
  if (key) {
    Serial.println(key);  // Muestra la tecla presionada en el monitor serie
   
    // Si se presiona un número (0-9), se agrega al código de entrada
    if (key >= '0' && key <= '9') {
      inputCode += key;  // Agrega el número presionado al código
    }

    // Si se presiona '#' se resetea el código ingresado
    if (key == '#') {
      inputCode = "";  // Resetea el código de entrada
      Serial.println("Código reseteado.");
    }

    // Si se ha ingresado "69"
    if (inputCode == "69") {
      activarAlarma();  // Llama a la función para activar el LED y el buzzer
      inputCode = "";   // Resetea el código después de la activación
    }
  }
}

// Función para activar el LED y el buzzer
void activarAlarma() {
  Serial.println("¡Código 69 ingresado! Activando alarma...");

  // Enciende el LED
  digitalWrite(ledPin, HIGH);
 
  // Enciende el buzzer por 2 segundos
  for (int i = 0; i < 200; i++) {
    digitalWrite(buzzerPin, HIGH);
    delay(5);
    digitalWrite(buzzerPin, LOW);
    delay(5);
  }

  // Apaga el LED y el buzzer
  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);
}
