#include <Keypad.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

const byte ROWS = 4; // Numărul de rânduri al tastaturii
const byte COLS = 3; // Numărul de coloane al tastaturii

char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {13, 12, 11, 10}; // Conectați pinii rândurilor la aceste pini
byte colPins[COLS] = {9, 8, 7}; // Conectați pinii coloanelor la aceste pini

const int waterSensorPin = A0;
const int gasPin = A1;
const int micPin = A2;
int isFlamePin = 6;
int sensor = 5;              // the pin that the sensor is atteched to
int state = LOW;             // by default, no motion detected
int val = 0;                 // variable to store the sensor status (value)

const String correctPIN = "0000"; // PIN-ul corect
String enteredPIN = ""; // PIN-ul introdus de utilizator
int wrongAttempts = 0; // Numărul de încercări greșite
const int ValMaxMic = 500;
int isFlame = HIGH; // HIGH MEANS NO FLAME

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adresa I2C a LCD-ului și dimensiunea afișajului

void setup() {
  Serial.begin(9600); // Inițializarea comunicării seriale
  pinMode(waterSensorPin, INPUT); // Inițializarea pinului senzorului de apă
  pinMode(micPin, INPUT); // Setează pinul de ieșire al modulului de microfon ca intrare
  pinMode(isFlamePin, INPUT); //flame sensor should be input as it is giving data
  pinMode(sensor, INPUT);    // initialize sensor as an input

  // Inițializarea LCD-ului
  lcd.init(); // initialize the lcd
  lcd.backlight();        // print message at (0, 0)
  lcd.setCursor(0, 0);         // move cursor to   (0, 0)
  lcd.print("Welcome to Edi's");// print message at (0, 0)
  lcd.setCursor(0, 1);         // move cursor to   (0, 0)
  lcd.print("  Smart  House  ");// print message at (0, 0)
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    lcd.clear();
    lcd.setCursor(0, 0);         // move cursor to   (0, 0)
    lcd.print("Enter password: ");// print message at (0, 0)
    lcd.setCursor(0, 1);         // move cursor to   (0, 0)
    lcd.print(enteredPIN);
    if (key == '#') {
      if (enteredPIN == correctPIN) {
        wrongAttempts = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("  Correct  PIN  ");
      } else {
        wrongAttempts++;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("  PIN   gresit!  ");
        delay(1000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("   Try again:  ");
        if (wrongAttempts >= 3) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("     ALARM!     ");
          Serial.println("THIEF");
          // Trimite mesajul de avertizare către ESP8266
        }
      }
      enteredPIN = ""; // Resetează PIN-ul introdus de utilizator
    } else {
      enteredPIN += key; // Adaugă tasta apăsată la PIN-ul introdus
      lcd.print(key);    // Afiseaza tasta apasata pe LCD
    }
  }
  
  int waterLevel = digitalRead(waterSensorPin); // Citirea stării senzorului de apă
  if (waterLevel == HIGH) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("     WARNING    ");
    lcd.setCursor(0, 1);
    lcd.print("Water detected! ");
    Serial.println("WATER");
    // Trimite un mesaj către ESP8266 pentru a iniția trimiterea mesajului "Water" pe WhatsApp
    delay(1000);
  }

  int gasValue = analogRead(gasPin); // Citim valoarea analogică de la senzor
  if(gasValue>400){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("     WARNING    ");
    lcd.setCursor(0, 1);
    lcd.print(" Gas  detected! ");
    Serial.println("GAS");
    // Trimite un mesaj către ESP8266 pentru a iniția trimiterea mesajului "Water" pe WhatsApp
    delay(1000); // Pauza de 1 secundă între citiri
  }

  isFlame = digitalRead(isFlamePin);//Readd the data gien by the flame sensor
  if (isFlame== LOW){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("     WARNING    ");
    lcd.setCursor(0, 1);
    lcd.print(" Fire detected! ");
    Serial.println("FIRE");
    delay(1000); // Pauza de 1 secundă între citiri
  }

  val = digitalRead(sensor);   // read sensor value
  if (val == HIGH) {           // check if the sensor is HIGH
    delay(100);                // delay 100 milliseconds 
    if (state == LOW) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("     WARNING    ");
      lcd.setCursor(0, 1);
      lcd.print("Motion detected!");
      Serial.println("MOVE");
      state = HIGH;       // update variable state to HIGH
    }
  } 
  else {
      delay(200);             // delay 200 milliseconds 
      if (state == HIGH){
        state = LOW;       // update variable state to LOW
    }
  }
}
