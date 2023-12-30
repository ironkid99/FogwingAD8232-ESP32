// void setup() 
// {

// Serial.begin(9600);
// pinMode(14, INPUT); 
// pinMode(12, INPUT); 
 
// }
 
// void loop() {
 
// if((digitalRead(10) == 1)||(digitalRead(11) == 1)){
// Serial.println('!');
// }
// else{

// Serial.println(analogRead(A0));
// }

// delay(1);
// }

// #include <ESP8266WiFi.h>
// #include <PubSubClient.h>
 
// #define WIFISSID "motorola_edge_20_6963"                                // Put your WifiSSID here
// #define PASSWORD "soumilgupta"                                 // Put your wifi password here
// #define TOKEN "BBFF-YKxITsj1YPeTMxw7mq8lvYFBpXnCxD"         // Put your Ubidots' TOKEN
// #define MQTT_CLIENT_NAME "myecgsensor"                       // MQTT client Name, please enter your own 8-12 alphanumeric character ASCII string; 
 

// #define VARIABLE_LABEL "myecg" // Assing the variable label
// #define DEVICE_LABEL "esp8266" // Assig the device label
 
// #define SENSOR A0 // Set the A0 as SENSOR
 
// char mqttBroker[]  = "industrial.api.ubidots.com";
// char payload[100];
// char topic[150];
// // Space to store values to send
// char str_sensor[10];
 

// WiFiClient ubidots;
// PubSubClient client(ubidots);
 
// void callback(char* topic, byte* payload, unsigned int length) {
//   char p[length + 1];
//   memcpy(p, payload, length);
//   p[length] = NULL;
//   Serial.write(payload, length);
//   Serial.println(topic);
// }
 
// void reconnect() {
//   // Loop until we're reconnected
//   while (!client.connected()) {
//     Serial.println("Attempting MQTT connection...");
    
//     // Attemp to connect
//     if (client.connect(MQTT_CLIENT_NAME, TOKEN, "")) {
//       Serial.println("Connected");
//     } else {
//       Serial.print("Failed, rc=");
//       Serial.print(client.state());
//       Serial.println(" try again in 2 seconds");
//       // Wait 2 seconds before retrying
//       delay(2000);
//     }
//   }
// }
 

// void setup() {
//   Serial.begin(115200);
//   WiFi.begin(WIFISSID, PASSWORD);
//   // Assign the pin as INPUT 
//   pinMode(SENSOR, INPUT);
 
//   Serial.println();
//   Serial.print("Waiting for WiFi...");
  
//   while (WiFi.status() != WL_CONNECTED) {
//     Serial.print(".");
//     delay(500);
//   }
  
//   Serial.println("");
//   Serial.println("WiFi Connected");
//   Serial.println("IP address: ");
//   Serial.println(WiFi.localIP());
//   client.setServer(mqttBroker, 1883);
//   client.setCallback(callback);  
// }
 
// void loop() {
//   if (!client.connected()) {
//     reconnect();
//   }
 
//   sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
//   sprintf(payload, "%s", ""); // Cleans the payload
//   sprintf(payload, "{\"%s\":", VARIABLE_LABEL); // Adds the variable label
  
//   float myecg = analogRead(SENSOR); 
  
//   dtostrf(myecg, 4, 2, str_sensor);
  
//   sprintf(payload, "%s {\"value\": %s}}", payload, str_sensor); // Adds the value
//   Serial.println("Publishing data to Ubidots Cloud");
//   client.publish(topic, payload);
//   client.loop();
//   delay(10);
// }

//#include <WiFi.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
 //WiFi credentials
// #define WIFI_SSID "Your WiFi SSID"
// #define WIFI_PASS "Your WiFi Password"

// // MQTT broker
// #define MQTT_BROKER "iothub.fogwing.net"

// // Fogwing MQTT Access credentials
// #define CLIENT_ID "Client ID"
// #define USER_NAME "MQTT User Name"
// #define PASSWORD "MQTT Password"

// // Fogwing MQTT Access publish and subscribe topics
// #define PUB_EDGE "Publish Topic"
// #define SUB_EDGE "Subscribe Topic"

// // 1 min time frequency to send the data to Fogwing IoT Hub
// #define TIME_FREQ ((1000 * 60) * 1)


// // Client object for WiFi
// WiFiClient wifiClient;

// // Client object for MQTT
// PubSubClient client(wifiClient);
 
#define WIFISSID "motorola_edge_20_6963" // Put your Wifi SSID here
#define PASSWORD "soumilgupta" // Put your WIFI password here
#define TOKEN "BBFF-vJqvIz0KlCZTfIq46Z9aGFc1LNmnSS" // Put your Ubidots' TOKEN
#define MQTT_CLIENT_NAME "mymqttclient" // MQTT client Name, please enter your own 8-12 alphanumeric character ASCII string; 

 
#define VARIABLE_LABEL "myecg"
#define DEVICE_LABEL "smartecg" 
 
#define SENSOR A0 
 
char mqttBroker[]  = "industrial.api.ubidots.com";
char payload[100];
char topic[150];
// Space to store values to send
char str_sensor[10];
 

WiFiClient ubidots;
PubSubClient client(ubidots);
 
void callback(char* topic, byte* payload, unsigned int length) {
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = '\0';
  Serial.write(payload, length);
  Serial.println(topic);
}
 
void reconnect() {
 
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    
    
    if (client.connect(MQTT_CLIENT_NAME, TOKEN, "CONNECTED")) {
      Serial.println("Connected");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFISSID, PASSWORD);
  // Assign the pin as INPUT 
  pinMode(SENSOR, INPUT);
 
  Serial.println();
  Serial.print("Waiting for WiFi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqttBroker, 1883);
  client.setCallback(callback);  
}
 
void loop() {
  if (!client.connected()) {
    reconnect();
  }
 
  sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
  sprintf(payload, "%s", ""); // Cleans the payload
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL); // Adds the variable label

// Check and print the MQTT connection status
  if (client.connected()) {
    Serial.println("MQTT Connected");
  } 
  else 
  {
    Serial.println("MQTT Disconnected");
  }
  
  float sensor = analogRead(SENSOR); 
  

  dtostrf(sensor, 4, 2, str_sensor);
  
  sprintf(payload, "%s {\"value\": %s}}", payload, str_sensor); // Adds the value
  Serial.println("Publishing data to Ubidots Cloud");
  client.publish(topic, payload);
  client.loop();
  delay(500);
}


// #include <ArduinoJson.h>
// #include <PubSubClient.h>
// #include <ESP8266WiFi.h>
// #include "DHT.h"

// #define DHTPIN 4     // Digital pin connected to the DHT sensor

// #define DHTTYPE DHT11

// DHT dht(DHTPIN, DHTTYPE);

 // WiFi credentials
// #define WIFI_SSID "Your WiFi SSID"
// #define WIFI_PASS "Your WiFi Password"

// // MQTT broker
// #define MQTT_BROKER "iothub.fogwing.net"

// // Fogwing MQTT Access credentials
// #define CLIENT_ID "Client ID"
// #define USER_NAME "MQTT User Name"
// #define PASSWORD "MQTT Password"

// // Fogwing MQTT Access publish and subscribe topics
// #define PUB_EDGE "Publish Topic"
// #define SUB_EDGE "Subscribe Topic"

// // 1 min time frequency to send the data to Fogwing IoT Hub
// #define TIME_FREQ ((1000 * 60) * 1)


// // Client object for WiFi
// WiFiClient wifiClient;

// // Client object for MQTT
// PubSubClient client(wifiClient);/

