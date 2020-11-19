import paho.mqtt.client as mqtt
from time import sleep
import board
import digitalio
import adafruit_character_lcd.character_lcd as characterlcd
# This MQTT client program connects to a MQTT broker specified
# by broker address and port.
# This MQTT broker also uses a username and password.
# Then it subscribes to two topics, in this example case balcony/temperature and
# balcony/humidity to read those two datas sent by an ESP8266 to these topics.
# Lastly it displays these datas on a 16x2 LCD display.
# Broker is running on the same Raspberry Pi 4 as this script
# so the LCD display is also connected to this same Raspberry.

# The address and port of our broker.
# Update with your address and port.
broker_address = ""
broker_port = 1883

# LCD config
lcd_columns = 16
lcd_rows = 2

lcd_rs = digitalio.DigitalInOut(board.D22)
lcd_en = digitalio.DigitalInOut(board.D17)
lcd_d4 = digitalio.DigitalInOut(board.D25)
lcd_d5 = digitalio.DigitalInOut(board.D24)
lcd_d6 = digitalio.DigitalInOut(board.D23)
lcd_d7 = digitalio.DigitalInOut(board.D18)

# Initialise the lcd class
lcd = characterlcd.Character_LCD_Mono(lcd_rs, lcd_en, lcd_d4, lcd_d5, lcd_d6,
                                      lcd_d7, lcd_columns, lcd_rows)
# Clear the lcd
lcd.clear()

# Globals for lcd
lcd_line_1 = ""
lcd_line_2 = ""

# Callback for listening to balcony/temperature topic.
def temperature_callback(client, userdata, msg):
    # Decode the message from binary
    temp = msg.payload.decode("utf-8")
    print(msg.topic + " " + str(temp))
    # Update the global variable
    global lcd_line_1
    lcd_line_1 = "Temp: " + temp + " C\n"
    # Update the lcd
    update_lcd()
    
# Callback for listening to balcony/humidity topic.
def humidity_callback(client, userdata, msg):
    hum = msg.payload.decode("utf-8")
    print(msg.topic + " " + str(hum))
    global lcd_line_2
    lcd_line_2 = "Hum: " + hum + " %"
    update_lcd()

# Callback when client receives a CONNACK response from server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result: " +str(rc))

    # Subscribe to our two topics.
    client.subscribe([("balcony/temperature", 0), ("balcony/humidity", 2)])

def update_lcd():
    lcd.message = lcd_line_1 + lcd_line_2
    

# New client object.
client = mqtt.Client()
# Username and password to our broker.
# Update with your username and password.
client.username_pw_set("", "")
client.on_connect = on_connect
# Different callbacks to handle the temperature and humidity data.
client.message_callback_add("balcony/temperature", temperature_callback)
client.message_callback_add("balcony/humidity", humidity_callback)

sleep(5)
client.connect(broker_address, broker_port, 60)
# loop_forever handles automatic reconnects.
client.loop_forever()
