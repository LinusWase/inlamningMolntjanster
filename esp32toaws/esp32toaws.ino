//Include necessary libraries

#include "secrets.h"
#include "sensor.h"
#include <WiFiClientSecure.h>
#include <MQTTClient.h> //MQTT Library Source: https://github.com/256dpi/arduino-mqtt
#include <ArduinoJson.h> //ArduinoJson Library Source: https://github.com/bblanchon/ArduinoJson
#include "WiFi.h"

#define AWS_IOT_PUBLISH_TOPIC   "/telemetry"
#define AWS_IOT_SUBSCRIBE_TOPIC "/downlink" 

String THINGNAME = "";

long sendInterval = 20000;

WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(1024); //Maximum size of packages recieved and published

void connectToAws(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  THINGNAME = WiFi.macAddress();
  //Remove colon
  THINGNAME.replace(":", "");

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

  while(!client.connect(THINGNAME.c_str())){
    Serial.print("-");
    delay(1000);
  }

  if(!client.connected()){
    Serial.println("AWS IoT Timed out!");
    return;
  }

  client.subscribe(THINGNAME + AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("AWS IoT Connected successfully");
}

void setupShadow() {
  client.subscribe("$aws/things/" + THINGNAME + "/shadow/get/accepted");
  client.subscribe("$aws/things/" + THINGNAME + "/shadow/get/rejected");
  //client.subscribe("$aws/things/" + THINGNAME + "/shadow/update/accepted");
  client.subscribe("$aws/things/" + THINGNAME + "/shadow/update/delta");

  client.publish("$aws/things/" + THINGNAME + "/shadow/get");
}

bool publishTelemetry(String payload){
  Serial.print("Publishing: ");
  Serial.println(payload);
  return client.publish(THINGNAME + AWS_IOT_PUBLISH_TOPIC, payload);
}

void messageHandler(String &topic, String &payload){
  Serial.println("incoming: " + topic + " - " + payload);

  DynamicJsonDocument doc(512);
  deserializeJson(doc, payload);

  //Device shadow
  if(topic.endsWith("/shadow/get/accepted")){
    updateSettings(doc["state"]["desired"]);
  } else if (topic.endsWith("/shadow/update/delta")){
    updateSettings(doc["state"]);
  }
}

void updateSettings(JsonDocument settingsObj) {
  sendInterval = settingsObj["sendIntervalSeconds"].as<int>() * 1000;

  JsonDocument docResponse;
  docResponse["state"]["reported"] = settingsObj;
  char jsonBuffer[512];
  serializeJson(docResponse, jsonBuffer);

  // report back to device shadow
  Serial.print("Sending reported state to AWS: ");
  serializeJson(docResponse, Serial);

  client.publish("$aws/things/" + THINGNAME + "/shadow/update", jsonBuffer);
}

void setup(){
  Serial.begin(115200);
  delay(2000);
  //connectToAws();
  initSensor();
}

void loop() {
  delay(2000);
  float temperature = getTemperature();
  float humidity = getHumidity();
  static unsigned long previousMillis = -sendInterval;

  client.loop();

  if(millis() - previousMillis >= sendInterval){
    previousMillis = millis();

    bool sendResult = publishTelemetry("{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + "}");
    //If program fail, restart
    //if (GNDsendResult == 0)
    //  ESP.restart();
  }

}
