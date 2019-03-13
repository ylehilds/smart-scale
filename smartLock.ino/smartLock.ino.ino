#include "keys.h"
/************************ Adafruit IO Configuration *******************************/

// visit io.adafruit.com if you need to create an account,
// or if you need your Adafruit IO key.
#define IO_USERNAME    adaFruitUserName
#define IO_KEY         adaFruitIoKey

/******************************* WIFI Configuration **************************************/

#define WIFI_SSID       ssid
#define WIFI_PASS       password

#include <AdafruitIO_WiFi.h>
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
/************************ Main Program Starts Here *******************************/

/******************************** Libraries **************************************/
#include <ESP8266WiFi.h> // ESP8266 library
#include <AdafruitIO.h>  // Adafruit IO library
#include <Adafruit_MQTT.h> // Adafruit MQTT library

int relay_pin = D5;

void setup() {
    Serial.begin(115200);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println(io.statusText());
  
  // put your setup code here, to run once:
   pinMode( relay_pin , OUTPUT);
  
}

void loop() {
  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
  
  // put your main code here, to run repeatedly:
  Serial.println("relay on");
   digitalWrite( relay_pin , HIGH);
   delay(5000);
   digitalWrite(relay_pin , LOW);
   Serial.println("power off");
   delay(5000);
}

void handleMessage(AdafruitIO_Data *data) {
Serial.printf("\nreceived <- %s", data->value());
  if (!strcmp(data->value(), "OPEN")) {
    Serial.printf("\nIt worked!");
  }}