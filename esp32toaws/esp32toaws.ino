//Include necessary libraries

#include "secrets.h"
#include <WiFiClientSecure.h>
#include <MQTTClient.h> //MQTT Library Source: https://github.com/256dpi/arduino-mqtt
#include <ArduinoJson.h> //ArduinoJson Library Source: https://github.com/bblanchon/ArduinoJson
#include "WiFi.h"

#define AWS_IOT_PUBLISH_TOPIC   "/telemetry"
#define AWS_IOT_SUBSCRIBE_TOPIC "/downlink" 

String THINGNAME = "";

long sendInterval = 20000;

void connectToAws(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  THINGNAME = WiFi.macAddress();
  //Remove colon
  THINGNAME.remove(":", "";)

  Serial.println();
  Serial.print("MAC Address: ");
  Serial.println(THINGNAME);

  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");
  }

  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to AWS broker using port 8883
  client.begin(AWS_IOT_ENDPOINT, 8883, net);

  while(!client.connest(THINGNAME.c_str())){
    Serial.print("-");
    delay(1000);
  }

  if(!client.connected()){
    Serial.println("AWS IoT Timed out!");
    return;
  }

  
  Serial.println("")
}


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
