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
#include <ArduinoJson.h>


AdafruitIO_Feed *myGoal = io.feed("my-goal"); // set up the 'iot scale' feed

#define relayPin D5

void setup() {
    Serial.begin(115200);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();
    
  // set up a message handler for the 'my weight' feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  myGoal->onMessage(handleMessage);

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println(io.statusText());
  
  // put your setup code here, to run once:
   pinMode( relayPin , OUTPUT);
   //digitalWrite(relayPin, LOW);
}

void loop() {
  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

//float f = 0.0;

//      myGoal->save(f);
      delay(400);
  
  // put your main code here, to run repeatedly:
//  Serial.println("relay on");
//   digitalWrite( relay_pin , HIGH);
//   delay(5000);
//   digitalWrite(relay_pin , LOW);
//   Serial.println("power off");
//   delay(5000);
}

void handleMessage(AdafruitIO_Data *data) {
  Serial.printf("\nreceived <- %s", data->value());
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(data->value());  // Parse to JSON format
  
//  const String dataWeight = root["weight"];
//  Serial.print("dataWeight: ");
//  Serial.println(dataWeight);
//
//  const String goalDate = root["goalDate"];
//  Serial.print("goalDate: ");
//  Serial.println(goalDate);

  const String goalAchieved = root["goal_achieved"];
  Serial.print("\ngoalAchieved: ");
  Serial.println(goalAchieved);

  if (strcmp(root["goal_achieved"], "true") == 0) {
      Serial.println("Unlock Reward Box!!!");
      digitalWrite(relayPin, HIGH); //Set the pin to HIGH (3.3V)
  }
  else {
      Serial.println("Lock Reward Box!!!");
      digitalWrite(relayPin, LOW); //Set the pin to HIGH (3.3V)
  }
}
