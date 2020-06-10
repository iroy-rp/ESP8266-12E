#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "fauxmoESP.h"

#define WIFI_SSID "Casa-chetos"
#define WIFI_PASS "Amalia987_"
#define SERIAL_BAUDRATE 74880//74880,115200

// DEVICES
#define LED_BUILTIN 2//LED AZUL
int LedPin = D5;
#define ID_1 "lampara 2"

fauxmoESP fauxmo;

// configuracion wifi
void wifiSetup() {

  // Set WIFI module to STA mode
  WiFi.mode(WIFI_STA);

  // Conectando

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // espera
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }

  Serial.println("\nListo esta conectado a internet..\n");
  Serial.printf("[WIFI] conectado a %s\n ", WIFI_SSID);


  // Connected!
  //Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());

}

void setup() {

  // Init serial port and clean garbage
  Serial.begin(SERIAL_BAUDRATE);

  // LEDs
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(LedPin, OUTPUT);
  digitalWrite(LedPin, LOW);

  // Wifi
  wifiSetup();

  fauxmo.createServer(true); // not needed, this is the default value
  fauxmo.setPort(80); // This is required for gen3 devices
  fauxmo.enable(true);
  // Add virtual devices
  fauxmo.addDevice(ID_1);

  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value)
  {
    Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
    if (strcmp(device_name, ID_1) == 0) {
      digitalWrite(LedPin, state ? HIGH : LOW);
    }
  });

}

void loop() {
  fauxmo.handle();
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
}
