#include <Keypad.h>
#include <Servo.h>
#include <PIR.h>

const byte ROWS = 4; // Numărul de rânduri al tastaturii
const byte COLS = 3; // Numărul de coloane al tastaturii

char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {13, 12, 11, 10};     // Conectați pinii rândurilor la aceste pini
byte colPins[COLS] = {9, 8, 7};        // Conectați pinii coloanelor la aceste pini

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
const int servoPin = A0; // Pinul de semnal al servomotorului
const int PIR_Pin = A1; // Pinul senzorului PIR
const int waterSensorPin = A2; // Pinul senzorului de apă
const int buzzerPin = A3; // Pinul buzzer-ului
const int ledPin = A4;  // Pinul LED-ului verde

const String correctPIN = "0000"; // PIN-ul corect
String enteredPIN = ""; // PIN-ul introdus de utilizator
int incorrectAttempts = 0; // Numărul de încercări greșite

Servo servo; // Obiectul pentru controlul servomotorului
int pirState = LOW; // Starea senzorului PIR (LOW - nu a detectat mișcare, HIGH - a detectat mișcare)
int waterSensorState = LOW; // Starea senzorului de apă (LOW - nu a detectat apă, HIGH - a detectat apă)



void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(PIR_Pin, INPUT); // Setează pinul senzorului PIR ca pin de intrare
  pinMode(waterSensorPin, INPUT); // Setează pinul senzorului de apă ca pin de intrare
  servo.attach(servoPin); // Atașează servomotorul la pinul specificat
  servo.write(0); // Setează poziția inițială a servomotorului la 0 grade
  Serial.begin(9600);
}

void loop() {
  // Verifică starea senzorului PIR
  int currentPIRState = digitalRead(PIR_Pin);
    if (currentPIRState != pirState) {
      pirState = currentPIRState;
        if (pirState == HIGH) {
        // Senzorul PIR a detectat mișcare
        digitalWrite(buzzerPin, HIGH); // Activează buzzerul
        Serial.println("Mișcare detectată! Buzzer activat!");
        }
    }

  int currentWaterSensorState = digitalRead(waterSensorPin);
  if (currentWaterSensorState != waterSensorState) {
    waterSensorState = currentWaterSensorState;
    if (waterSensorState == HIGH) {
      // Senzorul de apă a detectat apă
      digitalWrite(buzzerPin, HIGH); // Activează buzzerul
      Serial.println("Apa detectată! Buzzer activat!");
    }
  }

  char key = keypad.getKey();
  if (key) {
    if (key == '#') {
      if (enteredPIN == correctPIN) {
        digitalWrite(ledPin, HIGH); // Aprinde LED-ul verde
        delay(2000);
        digitalWrite(ledPin, LOW); 
        digitalWrite(buzzerPin, LOW);// Setează buzzerul pe mute (fără sunet)
        incorrectAttempts = 0;
        Serial.println("PIN corect!");
        servo.write(90); // Rotirea servomotorului la 90 de grade
      } else {
        incorrectAttempts++;
        Serial.println("PIN greșit!");
        if (incorrectAttempts >= 3) {
          digitalWrite(buzzerPin, HIGH); // Activează buzzerul
          Serial.println("PIN greșit de 3 ori! Buzzer activat!");
          servo.write(0);
        }
      }
      enteredPIN = ""; // Resetează PIN-ul introdus de utilizator
    } else {
      enteredPIN += key; // Adaugă tasta apăsată la PIN-ul introdus
    }
  }
}
