#include <Arduino.h>
#include <Wire.h>
#include <pgmspace.h>
#include "FlowMeter.h"
#include <RtcDS3231.h>
#include "Config.h"
#include <ESP8266WiFi.h>
#include <LiquidCrystal_PCF8574.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

#define SERIAL_BAUD 115200

void mqttCallback(char* topic, byte* payload, unsigned int length);
void reconnect();

WiFiClient    derWiFi;
PubSubClient  mqttClient(MQTT_SERVER, 1883, mqttCallback, derWiFi);


LiquidCrystal_PCF8574 lcd(0x27);
DynamicJsonBuffer  jsonBuffer;
JsonObject& sensorInfo1  = jsonBuffer.createObject();
JsonObject& sensorInfo2  = jsonBuffer.createObject();
JsonObject& sensorInfo3  = jsonBuffer.createObject();

JsonObject& mqttTelemetry = jsonBuffer.createObject();

String strJSON = "";

bool WiFiOn = false;
bool mqttOn = false;

bool metric = true;

RtcDS3231 Rtc;

void setup()
  {

  }

void loop()
  {

  }

  void mqttCallback(char* topic, byte* payload, unsigned int length)
  {
    Serial.print(topic);
    JsonObject& mqttCommand = jsonBuffer.parseObject((char*) payload);
    if(mqttCommand.success())
    {
      byte cmd = mqttCommand["cmd"].as<byte>();
      switch(cmd){
        case NODE_CMD_CHECK:
          mqttTelemetry["ID"]=NODE_ID;
          if (true) {
            mqttTelemetry["msg"]="Sensors online";
            mqttTelemetry["code"]=NODE_OK;
          }
          else {
            mqttTelemetry["msg"]="Sensors offline";
            mqttTelemetry["code"]=NODE_ERR_HW;
          };
          mqttTelemetry.printTo(strJSON);
          Serial.println(strJSON);
          mqttClient.publish(MQTT_TOPICS[NODE_TELEMETRY], strJSON.c_str());
          break;
        case NODE_CMD_RESTART:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Restart imminent");
          mqttTelemetry["ID"]=NODE_ID;mqttTelemetry["msg"]="Node restart";
          mqttTelemetry["code"]=NODE_RESTART; mqttTelemetry.printTo(strJSON);
          Serial.println(strJSON);
          mqttClient.publish(MQTT_TOPICS[NODE_TELEMETRY], strJSON.c_str());
          delay(5000);
          mqttClient.disconnect();
          delay(2000);
          ESP.restart();
          break;
        default: break;
      }
    } else
    {
      mqttTelemetry["ID"]=NODE_ID; mqttTelemetry["msg"]="Bad command";
      mqttTelemetry["code"]=NODE_ERR_CMD;
      mqttTelemetry.printTo(strJSON);
      mqttClient.publish(MQTT_TOPICS[NODE_TELEMETRY],strJSON.c_str());
      Serial.println(strJSON);
    }
  }
