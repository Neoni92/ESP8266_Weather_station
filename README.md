# MQTT Weather station using ESP8266 and a Raspberry Pi.
This weather station project uses ESP8266, Raspberry Pi and MQTT to communicate and send temperature + humidity data between these devices.
DHT22 temperature and humidity sensor is connected to the ESP8266. Data is shown on a 16x2 LCD display connected to the Raspberry Pi.

## Project devices used
- Sparkfun ESP8266 Thing Dev
- Raspberry Pi Model 4B
- 16x2 LCD
- DHT22 temperature and humidity sensor

## Weather station
This small project started from the idea of seeing basic weather info from a small lcd screen indoors. The data comes from a basic temp+hum
sensor located on a balcony. Because the esp8266, which the sensor is connected, is located well under my own wifi network, I chose to use 
MQTT as the communication protocol. My Raspberry Pi is the MQTT broker which the esp8266 client connects to and there is also another client
program running on the Pi, which updates the LCD with the latest data.

ESP8266 Thing Dev has some low power options built in that can be used to reduce its power. I chose to use its deep sleep mode to take the
power saving to the maximum so my weather station can run longer on battery life.

## Details and wiring
- to be done

## TODO
- choose a battery to start testing the deep sleep mode in use
- think about the casing for the esp8266

