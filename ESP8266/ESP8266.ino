#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <UrlEncode.h>
#include <SoftwareSerial.h>
#define espSerial Serial

const char* ssid = "S10";
const char* password = "123456789";

String phoneNumber = "+40774425720";
String apiKey = "8443283";

const int maxAttempts = 3;
int wrongAttempts = 0;

void sendMessage(String message){

  // Data to send with HTTP POST
  String url = "http://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);
  WiFiClient client;    
  HTTPClient http;
  http.begin(client, url);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  // Send HTTP POST request
  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200){
    Serial.print("Message sent successfully");
  }
  else{
    Serial.println("Error sending the message");
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }

  // Free resources
  http.end();
}

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "THIEF") {
      sendMessage("Someone is trying to break into your house!!!");
    }
    if (command == "WATER") {
      sendMessage("You have water leaks!!!");
    }
    if (command == "GAS") {
      sendMessage("You have gas leaks!!!");
    }
    if (command == "FIRE") {
      sendMessage("You have fire in the house!!!");
    }
    if (command == "MOVE") {
      sendMessage("Someone's in your house!!!");
    }
  }
}