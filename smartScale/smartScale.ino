
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "keys.h"

#include <HX711.h>  // HX711 library for the scla
#include "DFRobot_HT1632C.h" // Library for DFRobot LED matrix display

#define calibration_factor -21700.0 //-7050.0 //This value is obtained using the SparkFun_HX711_Calibration sketch

#define DOUT 0  // Pin connected to HX711 data output pin
#define CLK  12  // Pin connected to HX711 clk pin

#define NUM_MEASUREMENTS 5 // Number of measurements
#define THRESHOLD 2      // Measures only if the weight is greater than 2 kg. Convert this value to pounds if you're not using SI units.
#define THRESHOLD1 0.5  // Restart averaging if the weight changes more than 0.5 kg.

#define DATA D6 // Pin for DFRobot LED matrix display
#define CS D2   // Pin for DFRobot LED matrix display
#define WR D7   // Pin for DFRobot LED matrix display

DFRobot_HT1632C ht1632c = DFRobot_HT1632C(DATA, WR,CS); // set up LED matrix display

HX711 scale(DOUT, CLK);

float weight = 0.0;
float prev_weight = 0.0;


const char* server = "192.168.1.101";                           // Raspberry Pi running mosquitto mqtt
String macAddr = WiFi.macAddress();                           // Store arduino MAC address as a string
String host = "arduinoSmartScale-" + macAddr.substring(15) ;  // Set a client ID for this device (should not match other MQTT devices)

WiFiClient wifiClient;                                        // Instantiate wifi
PubSubClient mqttClient(wifiClient);                          // Instantiate mqtt client

void setup()
{
// start the serial connection
  Serial.begin(115200);
  Serial.println("Smart Scale start!");

  
  WiFi.begin(ssid, password);                         // WiFi connection
  
  // wait for a connection
  while (WiFi.status() != WL_CONNECTED) {                     // Wait for the WiFI connection completion
    // setup LED matrix display
  ht1632c.begin();
  ht1632c.isLedOn(true);
  ht1632c.clearScreen();
  delay(500);


  ht1632c.print("connecting...",50);

    Serial.print(".");
    ht1632c.print("...",50);
    ESP.wdtFeed();
    delay(500);
    Serial.println("Waiting for connection");
  }
  Serial.println("");
  // we are connected
  Serial.println("WiFi connected");                            // Wifi connection debug
  delay (100);                                                 // Let everything settle


  mqttClient.setServer(server, 1883);                          // Set up MQTT Connection info and server on port 1883
  mqttClient.setCallback(callback);            // Set callback action for receiving messages
  
  ESP.wdtFeed();
  ht1632c.print("connected...",50);

  scale.begin(DOUT, CLK); //esp specific statment 

  // uncomment next 2 lines when ready to take weight measurements
 scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
 scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0
}

void loop() {
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();                    // Loop to check for new messages


  ht1632c.clearScreen();
  ESP.wdtFeed();
  weight = scale.get_units();
  // Serial.println(weight);
  
  float avgweight = 0;

  if (weight > THRESHOLD) { // Takes measures if the weight is greater than the threshold
    float weight0 = scale.get_units();
    for (int i = 0; i < NUM_MEASUREMENTS; i++) {  // Takes several measurements
      delay(100);
      weight = scale.get_units();
      avgweight = avgweight + weight;
      if (abs(weight - weight0) > THRESHOLD1) {
        avgweight = 0;
        i = 0;
      }
      weight0 = weight;
    }
    avgweight = avgweight / NUM_MEASUREMENTS; // Calculate average weight
    
    Serial.print("Measured weight: ");
    Serial.print(avgweight, 1);
    Serial.println(" lb");
    
    char result[8]; // Buffer big enough for 7-character float
    dtostrf(avgweight, 6, 1, result);
    ht1632c.print(result);  // Display on LED matrix

    String resultString = String(result);
    resultString.trim();
    
    String json = "{\"weight\":\""+resultString+"\"}";
    int str_len = json.length() + 1;
    char char_array[str_len];
    json.toCharArray(char_array, str_len);
    Serial.print("char_array: ");
    Serial.println(char_array);
    mqttClient.publish("smart_scale_topic", char_array , sizeof(char_array));

    // wait while there's someone on the scake
    while (scale.get_units() > THRESHOLD) {
      ESP.wdtFeed();
    }
    
    //keep LEDs on for two seconds then restart
    delay(2000); 
  }

  /*
    char message [100];
    sprintf(message,"got a new weight scale reading:  %i", 200);
    mqttClient.publish("smart_scale_topic", message , sizeof(message));
*/
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
