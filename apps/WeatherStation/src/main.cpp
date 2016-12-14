#include "Config.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include "BME280I2C.h"
#include <LiquidCrystal_PCF8574.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

#define SERIAL_BAUD 115200

void mqttCallback(char* topic, byte* payload, unsigned int length);
void reconnect();
void weatherStation();

WiFiClient    derWiFi;
PubSubClient  mqttClient(MQTT_SERVER, 1883, mqttCallback, derWiFi);


BME280I2C bme;
LiquidCrystal_PCF8574 lcd(0x27);
DynamicJsonBuffer  jsonBuffer;
JsonObject& sensorInfo1  = jsonBuffer.createObject();
JsonObject& sensorInfo2  = jsonBuffer.createObject();
JsonObject& sensorInfo3  = jsonBuffer.createObject();

JsonObject& mqttTelemetry = jsonBuffer.createObject();

String strJSON = "";

bool bmeOnline = false;
bool WiFiOn = false;
bool mqttOn = false;

bool metric = true;

float pres=0, temp=0, hum=0;

float pig=0;
uint16_t piglet1=0;
uint16_t piglet2=0;

void setup()
{
  uint8_t i = 0;
  sensorInfo1["type"] = SENSOR1_TYPE;sensorInfo1["ID"] = SENSOR1_ID;sensorInfo1["value"] = temp;
  sensorInfo2["type"] = SENSOR2_TYPE;sensorInfo2["ID"] = SENSOR2_ID;sensorInfo2["value"] = hum;
  sensorInfo3["type"] = SENSOR3_TYPE;sensorInfo3["ID"] = SENSOR3_ID;sensorInfo3["value"] = pres;

  Serial.begin(SERIAL_BAUD);
  lcd.begin(16, 2);lcd.setCursor(1, 0);lcd.setBacklight(128);
  lcd.print("Starting up...");lcd.setCursor(0,1);lcd.print("BME:");
  while(!Serial) {} // Wait

  while(!(bmeOnline=bme.begin()) && i++<NET_RETRIES){
    Serial.println("Could not find BME280 sensor!");
    lcd.print(i);
    delay(1000);
  }
  if(bmeOnline) {
    lcd.print("ON");delay(5000);
  } else {
    lcd.setCursor(0, 0); lcd.print("BME 280: Offline");delay(2000);
  }
  WiFi.begin(NET_SSID, NET_PASSWD);
  WiFi.config(IPAddress(192,168,1,10),IPAddress(192,168,1,1),IPAddress(255,255,255,0),IPAddress(8,8,8,8),IPAddress(8,8,4,4));
  i = -1; lcd.setCursor(0,1); lcd.print("Net:");
  while ((WiFi.status() != WL_CONNECTED) && i++<NET_RETRIES){
    delay(500);Serial.print(".");
    lcd.print(i);
  };
  if((i==NET_RETRIES) && (WiFi.status() != WL_CONNECTED)) {
    Serial.println(F("WiFi connection failed!"));
    lcd.setCursor(0, 1);lcd.print("WiFi conn failed"); delay(2000);
  } else {
      Serial.println(F("WiFi connected: "));
      WiFiOn = true;
      WiFi.printDiag(Serial);
      Serial.println(WiFi.localIP());
      Serial.println(WiFi.softAPIP());
      Serial.println(WiFi.gatewayIP());
      lcd.setCursor(0, 1);lcd.print("WiFi connect: OK");
      delay(5000);
  }
  if(WiFi.isConnected())
  {
    Serial.print(F("Connecting to mqtt://"));Serial.print(MQTT_SERVER);
    if (mqttClient.connect(NODE_ID, MQTT_USER, MQTT_PASSWD))
    {
      mqttOn = true;
      Serial.println(F(" - OK"));
      mqttTelemetry["ID"]=NODE_ID;
      if (bmeOnline) {
        mqttTelemetry["msg"]="Sensors online";
        mqttTelemetry["code"]=NODE_OK;
      }
      else {
        mqttTelemetry["msg"]="Sensors offline";
        mqttTelemetry["code"]=NODE_ERR_HW;
      };
      mqttTelemetry.printTo(strJSON);
      mqttClient.publish(MQTT_TOPICS[NODE_TEMP_SENSOR], strJSON.c_str()); strJSON = "";
      lcd.setCursor(0, 1); lcd.print(" Telemetry sent "); delay(5000);
    }
    else {
      Serial.print(" - failed, rc=");
      Serial.println(mqttClient.state());
      lcd.setCursor(0, 1); lcd.print(" MQTT: failure! ");delay(5000);
    }
  }
}

void loop()
{
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
  weatherStation();
  //pig=temp*
  //Serial.println(temp*10000)
  delay(2000);
}


void weatherStation()
{
  bme.read(pres, temp, hum, metric, 0B010);
  lcd.clear();lcd.setCursor(0, 0);
  lcd.print("T:"); lcd.print(temp); lcd.print(" H:");lcd.print(hum);
  lcd.setCursor(0,1);
  lcd.print("P:"); lcd.print(in2mm(pres), 1); lcd.print(" mmHg");
  lcd.setCursor(13,1);
  if (bmeOnline) lcd.print("S"); else lcd.print("$");
  if (WiFiOn) lcd.print("W"); else lcd.print(" ");
  if (mqttOn) lcd.print("M"); else lcd.print(" ");
  sensorInfo1["value"]=temp; sensorInfo1.printTo(strJSON);
  mqttClient.publish(MQTT_TOPICS[NODE_TEMP_SENSOR], strJSON.c_str()); strJSON = "";
  sensorInfo2["value"]=hum; sensorInfo2.printTo(strJSON);
  mqttClient.publish(MQTT_TOPICS[NODE_HUMI_SENSOR], strJSON.c_str());strJSON = "";
  sensorInfo3["value"]=in2mm(pres); sensorInfo3.printTo(strJSON);
  mqttClient.publish(MQTT_TOPICS[NODE_PRES_SENSOR], strJSON.c_str());strJSON = "";
}

void reconnect() {
  // Loop until we're reconnected
  int i=-1;
  while ((NET_RETRIES > i++)&&(!mqttClient.connected())) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect(NODE_ID)) {
      mqttOn = true;
      Serial.println("connected");
      // Once connected, publish an announcement...
      mqttTelemetry["ID"]=NODE_ID; mqttTelemetry["msg"]="Connected";
      mqttTelemetry.printTo(strJSON);
      mqttClient.publish(MQTT_TOPICS[NODE_TELEMETRY], strJSON.c_str());
      // ... and resubscribe
      mqttClient.subscribe(MQTT_TOPICS[NODE_CONTROL]);strJSON="";
      lcd.setCursor(0, 1); lcd.print(" Telemetry sent "); delay(5000);
      i--;
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      lcd.setCursor(0, 1); lcd.print(" MQTT: failure! ");delay(5000);
    }
  }
  if (i==NET_RETRIES)
  {
    Serial.println(F("Connection failed!"));
//    ESP.reset();
  }
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
        if (bmeOnline) {
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
