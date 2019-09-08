


#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#define WIFI_SSID "OnePlus 6"
#define WIFI_PASS "asdfghjkl1234"

#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "adish"
#define MQTT_PASS "a5ce42ec9f8b4f1db76f60f8275070fe"
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);

Adafruit_MQTT_Subscribe ad1 = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/ad1");
Adafruit_MQTT_Subscribe ad2 = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/ad2");

int ledPin = 14; // choose pin for the LED
int inputPin = 13; // choose input pin (for Infrared sensor) 
int val = 0; // variable for reading the pin status

void setup()
{
   pinMode(D0, OUTPUT); 
  // pinMode(4, OUTPUT);
  // pinMode(0, OUTPUT);
  // pinMode(2, OUTPUT);
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT); // declare LED as output 
  pinMode(inputPin, INPUT); // declare Infrared sensor as input

  //Connect to WiFi
  Serial.print("\n\nConnecting Wifi... ");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
      Serial.print(".");
              digitalWrite(D0,HIGH);
              delay(100);   
              digitalWrite(D0,LOW);
    delay(100);
  }

            //On Successful connection to WiFi
            Serial.println("");
            Serial.println("WiFi successfully connected!");
            delay(500);
            

  //Subscribe to the onoff topic
  mqtt.subscribe(&ad1);
  mqtt.subscribe(&ad2);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  //Connect/Reconnect to MQTT
  MQTT_connect();

  //Read from our subscription queue until we run out, or
  //wait up to 5 seconds for subscription to update
  Adafruit_MQTT_Subscribe * subscription;
  while ((subscription = mqtt.readSubscription(5000)))
  {
    //If we're in here, a subscription updated...
    if (subscription == &ad1)
    {
      //Print the new value to the serial monitor
      Serial.print("ad1: ");
      Serial.println((char*) ad1.lastread);

      //If the new value is  "ON", turn the light on.
      //Otherwise, turn it off.
      if (!strcmp((char*) ad1.lastread, "ON"))
      {
        //active low logic
        digitalWrite(LED_BUILTIN, HIGH);
      }
      else
      {
        digitalWrite(LED_BUILTIN, LOW);
      }
    }
    if (subscription == &ad2)
    {
      //Print the new value to the serial monitor
      Serial.print("ad2: ");
      Serial.println((char*) ad2.lastread);

      //If the new value is  "ON", turn the light on.
      //Otherwise, turn it off.
      if (!strcmp((char*) ad2.lastread, "ON"))
      {
        //active low logic
        digitalWrite(D0, LOW);
      }
      else
      {
        digitalWrite(D0, HIGH);
      }
    }
  }

  // ping the server to keep the mqtt connection alive<br> 
if (!mqtt.ping()){ mqtt.disconnect();}}

  void MQTT_connect() 
{
  int8_t ret;
  // Stop if already connected
  if (mqtt.connected())
  {
    return;
  }

  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) // connect will return 0 for connected
  { 
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) 
    {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
                  //LED AT D2 WILL BLINK DENOTING SUCCESSFUL MQTT CONNECTION
                for(int i=0;i<3;i++)
                {
                digitalWrite(2,HIGH);
                delay(100);   
                digitalWrite(2,LOW);
                delay(100);
                }

   val = digitalRead(inputPin); // read input value 
   if (val == HIGH)
   { // check if the input is HIGH
      digitalWrite(ledPin, LOW); // turn LED OFF   
   } 
   else 
   { 
      digitalWrite(ledPin, HIGH); // turn LED ON 
   }              
}
