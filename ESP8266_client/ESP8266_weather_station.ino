/*
This program uses a Sparkfun ESP8266 Thing Dev board and DHT22 temperature + humidity
sensor. DHT22 sensor is connected to the ESP8266, which reads the sensor data and sends
it to a MQTT broker via WiFi connection, hosted in a Raspberry Pi. ESP8266 goes to deep sleep mode for 5 minutes
between sensor value readings to lower the power consumption significantly.

 * Connections:
 *   Thing Dev |  DHT22
 *  -----------|---------
 *      3V3    | 1 (VDD) 
 *        4    | 2 (DATA)
 *      GND    | 4 (GND)
 *
 */
 
#include <ESP8266WiFi.h> // Enables the ESP8266 to connect to the local network (via WiFi)
#include <PubSubClient.h> // Allows us to connect to, and publish to the MQTT broker
#include "DHT.h"

#define DHTPIN 4     // DHT22 digital pin
#define DHTTYPE DHT22   // DHT sensor type

DHT dht(DHTPIN, DHTTYPE);

// WiFi
// Update with your wifi ssid and password
const char* ssid = "";
const char* wifi_password = "";

// MQTT
// Update with your broker information
const char* mqtt_server = "";
const char* mqtt_topic1 = "balcony/temperature";
const char* mqtt_topic2 = "balcony/humidity";
const char* mqtt_username = "";
const char* mqtt_password = "";
const char* clientID = "ESP8266_THING_DEV";

// Initialise the WiFi and MQTT Client objects
WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient); // 1883 is the listener port for the Broker

void setup() {

  // Begin Serial on 115200
  // just for debugging purposes
  Serial.begin(115200);
  
  dht.begin();
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Connect to the WiFi
  WiFi.begin(ssid, wifi_password);

  // Wait until the connection has been confirmed before continuing
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Debugging - Output the IP Address of the ESP8266
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Connect to MQTT Broker
  // client.connect returns a boolean value to let us know if the connection was successful.
  if (client.connect(clientID, mqtt_username, mqtt_password)) {
    Serial.println("Connected to MQTT Broker!");
  }
  else {
    Serial.println("Connection to MQTT Broker failed...");
  }

  // Read the sensors in readSensors() function and send the data to MQTT Broker
  readSensors();

  // Deep sleep for 5 minutes
  ESP.deepSleep(300e6);
}

void loop() {
  
}

void readSensors() {
  // Reading temperature or humidity takes about 250 milliseconds.
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Buffer for the data
  char buffer[10];
    
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  dtostrf(h,4, 1, buffer);
  
  // Send humidity data
  client.publish(mqtt_topic2, buffer);
  
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  dtostrf(t,4, 1, buffer);

  // Send temperature data
  client.publish(mqtt_topic1, buffer);
    
  Serial.println();
  
  delay(100); 
}
