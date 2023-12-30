//#include <LoRa.h>

//#include <AFMotor.h>

//#include <LoRa.h>

// #include <ArduinoJson.h>
// #include <PubSubClient.h>
// #include <ESP8266WiFi.h>



// #define AD8232PIN A0  // Analog pin connected to the AD8232 sensor



// // WiFi credentials
// #define WIFI_SSID "motorola_edge_20_6963"
// #define WIFI_PASS ""

// // MQTT broker
// #define MQTT_BROKER "iothub.fogwing.net"

// // Fogwing MQTT Access credentials
// #define CLIENT_ID ""
// #define USER_NAME ""
// #define PASSWORD ""

// // Fogwing MQTT Access publish and subscribe topics
// #define PUB_EDGE "fwent/edge/557815c887b4c04a/inbound"
// #define SUB_EDGE "fwent/edge/557815c887b4c04a/outbound"

// // 1 min time frequency to send the data to Fogwing IoT Hub
// #define TIME_FREQ ((1000 * 60) * 1)

// // Reference voltage for AD8232 (if needed)
// #define REFERENCE_VOLTAGE 3.3

// // Client object for WiFi
// WiFiClient wifiClient;

// // Client object for MQTT
// PubSubClient client(wifiClient);

// void setup() {
//   // Initialize ESP8266 serial interface
//   Serial.begin(115200);

 

//   // Initialize WiFi
//   initWiFi();

//   // Set server and port here
//   client.setServer(MQTT_BROKER, 1883);

//   // Set function to receive the subscribed message
//   client.setCallback(onMsgReception);
// }

// void loop() {
//   static uint32_t tNow, tLast = 0;

//   // Check for MQTT Server connection
//   if (!client.connected())
//     reconnect();

//   // Store the current time ticks
//   tNow = millis();

//   // Send the payload at every TIME_FREQ
//   if ((tNow - tLast) > TIME_FREQ) {


//     // Read data from the AD8232 sensor
//     int ad8232Value = analogRead(A0);

//     // Convert the AD8232 data to voltage
//     float ecgValue = (ad8232Value / 1023.0) * REFERENCE_VOLTAGE;

//     // Create a JSON object
//     StaticJsonDocument<500> jsonData;

//     // Variable to store JSON payload
//     char payload[500];

//     // JSON payload
//     jsonData["temperature"] = t;
//     jsonData["humidity"] = h;
//     jsonData["ecg"] = ecgValue;

//     // Serialize JSON to send over the network
//     serializeJson(jsonData, payload);

//     // Publish your data
//     if (client.publish(PUB_EDGE, payload)) {
//       Serial.print(F("Published >> "));
//       Serial.println(payload);
//     }
//     else
//       Serial.println(F("Error publishing payload"));

//     // Subscribe to topic
//     if (client.subscribe(SUB_EDGE)) {
//       Serial.print(F("Subscribed to topic"));
//       Serial.println(SUB_EDGE);
//     }
//     else
//       Serial.println(F("Error subscribing to topic"));

//     tLast = tNow;
//   }

//   // Check continuously for incoming messages from subscribed topics
//   client.loop();
//   delay(100);
// }

// void initWiFi() {
//   Serial.print(F("Connecting to AP"));

//   // Set ESP8266 in station mode
//   WiFi.mode(WIFI_STA);

//   // Attempt to connect to WiFi network
//   WiFi.begin(WIFI_SSID, WIFI_PASS);

//   // Wait until connection
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }

//   if (WiFi.status() == WL_CONNECTED)
//     Serial.println(F("\nConnected to AP\n"));
//   else
//     Serial.println(F("Error connecting to WiFi"));
// }

// void reconnect() {
//   // Loop until reconnect
//   while (!client.connected()) {
//     if (WiFi.status() != WL_CONNECTED)
//       initWiFi();

//     Serial.println(F("Connecting to MQTT Broker"));
//     if (client.connect(CLIENT_ID, USER_NAME, PASSWORD))
//       Serial.println(F("Connected to MQTT"));
//     else
//       Serial.println(F("Failed to connect to MQTT"));
//   }
// }

// // This function is responsible for receiving messages from MQTT Broker
// void onMsgReception(char* topic, byte* payload, unsigned int length) {
//   payload[length] = '\0';

//   Serial.println(F("\n<------------- [Message Arrived] ------------->"));
//   Serial.print("Topic: ");
//   Serial.println(topic);
//   Serial.println((char*)payload);
//   Serial.println(F("<-------------- [End of Message] ------------->\n"));
// }



#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

#define AD8232PIN A0  // Analog pin connected to the AD8232 sensor

// WiFi credentials
#define WIFI_SSID "your wifi ssid"
#define WIFI_PASS "ypur password"

// MQTT broker
#define MQTT_BROKER "iothub.fogwing.net"

// Fogwing MQTT Access credentials
#define CLIENT_ID "2"
#define USER_NAME "a"
#define PASSWORD "&"

// Fogwing MQTT Access publish and subscribe topics
#define PUB_EDGE "/inbound"
#define SUB_EDGE "/outbound"

// 1 min time frequency to send the data to Fogwing IoT Hub
#define TIME_FREQ ((1000 * 60) * 1)

// Reference voltage for AD8232 (if needed)
#define REFERENCE_VOLTAGE 3.3

// Client object for WiFi
WiFiClient wifiClient;

// Client object for MQTT
PubSubClient client(wifiClient);

SoftwareSerial BTSerial(2,3); //RX TX

void setup() {
  // Initialize ESP8266 serial interface
  Serial.begin(115200);
  BTSerial.begin(9600);
  // Initialize WiFi
  initWiFi();

  // Set server and port here
  client.setServer(MQTT_BROKER, 1883);

  // Set function to receive the subscribed message
  client.setCallback(onMsgReception);
}

void loop() {
  static uint32_t tNow, tLast = 0;

  // Check for MQTT Server connection
  if (!client.connected())
    reconnect();

  // Store the current time ticks
  tNow = millis();

  // Send the payload at every TIME_FREQ
  if ((tNow - tLast) > TIME_FREQ) {
    // Read data from the AD8232 sensor
    int ad8232Value = analogRead(AD8232PIN);

    // Convert the AD8232 data to voltage
    float ecgValue = (ad8232Value / 1023.0) * REFERENCE_VOLTAGE;

    // Create a JSON object
    StaticJsonDocument<500> jsonData;

    // Variable to store JSON payload
    char payload[500];

    // JSON payload
    jsonData["ecg"] = ecgValue;

    // Serialize JSON to send over the network
    serializeJson(jsonData, payload);
    
    BTSerial.println(payload);

    delay(1000); // i can remove this

    // Publish your data
    if (client.publish(PUB_EDGE, payload)) {
      Serial.print(F("Published >> "));
      Serial.println(payload);
    }
    else
      Serial.println(F("Error publishing payload"));

    // Subscribe to topic
    if (client.subscribe(SUB_EDGE)) {
      Serial.print(F("Subscribed to topic "));
      Serial.println(SUB_EDGE);
    }
    else
      Serial.println(F("Error subscribing to topic"));

    tLast = tNow;
  }

  // Check continuously for incoming messages from subscribed topics
  client.loop();
  delay(100);
}

void initWiFi() {
  Serial.print(F("Connecting to AP"));

  // Set ESP8266 in station mode
  WiFi.mode(WIFI_STA);

  // Attempt to connect to WiFi network
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Wait until connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED)
    Serial.println(F("\nConnected to AP\n"));
  else
    Serial.println(F("Error connecting to WiFi"));
}

void reconnect() {
  // Loop until reconnect
  while (!client.connected()) {
    if (WiFi.status() != WL_CONNECTED)
      initWiFi();

    Serial.println(F("Connecting to MQTT Broker"));
    if (client.connect(CLIENT_ID, USER_NAME, PASSWORD))
      Serial.println(F("Connected to MQTT"));
    else
      Serial.println(F("Failed to connect to MQTT"));
  }
}

// This function is responsible for receiving messages from MQTT Broker
void onMsgReception(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0';

  Serial.println(F("\n<------------- [Message Arrived] ------------->"));
  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.println((char*)payload);
  Serial.println(F("<-------------- [End of Message] ------------->\n"));
}
