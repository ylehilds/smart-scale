
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "keys.h"


const char* server = "192.168.1.101";                           // Raspberry Pi running mosquitto mqtt
String macAddr = WiFi.macAddress();                           // Store arduino MAC address as a string
String host = "arduinoSmartScale-" + macAddr.substring(15) ;  // Set a client ID for this device (should not match other MQTT devices)

WiFiClient wifiClient;                                        // Instantiate wifi
PubSubClient mqttClient(wifiClient);                          // Instantiate mqtt client

void setup()
{
  Serial.begin(9600);
  Serial.println("Smart Scale start!");
  WiFi.begin(ssid, password);                         // WiFi connection

  while (WiFi.status() != WL_CONNECTED) {                     // Wait for the WiFI connection completion
    delay(500);
    Serial.println("Waiting for connection");
  }
  Serial.println("");
  Serial.println("WiFi connected");                            // Wifi connection debug
  delay (100);                                                 // Let everything settle

  mqttClient.setServer(server, 1883);                          // Set up MQTT Connection info and server on port 1883
  mqttClient.setCallback(callback);            // Set callback action for receiving messages
}

void loop() {
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();                    // Loop to check for new messages
  char* message = "got a new weight scale reading!";
  mqttClient.publish("smart_scale_topic", message , sizeof(message));
  delay(1000);                           // This introduces a little pause in each cycle. Probably helps save some power.

}

void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      char* message = "smart scale initial message!";
      mqttClient.publish("smart_scale_topic", message , sizeof(message));
      // ... and resubscribe
  mqttClient.subscribe("smart_scale_topic");               // subscription 
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {     // Callback function to read messages for subscribed topics
  Serial.print("\n Message arrived [");    // Serial Debug
  Serial.print(topic);                  //    Print the topic name [in brackets]
  Serial.print("] ");

  if (strcmp(topic, "smart_scale_topic") == 0) {
    Serial.print("in smart scale topic subscription");
    String payloadStringified = (char*) payload;
    // Length (with one extra character for the null terminator)
    int str_len = length + 1; 
    // Prepare the character array (the buffer)
    char message_char_array[str_len];
    // Copy it over
    payloadStringified.toCharArray(message_char_array, str_len);
    Serial.print("message: ");          // Print the transformed payload to Char Array
    Serial.println(message_char_array);   
  }
}
