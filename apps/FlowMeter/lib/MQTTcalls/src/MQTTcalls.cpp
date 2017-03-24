#include "MQTTcalls.h"

WiFiClient    derWiFi;

PubSubClient  mqttClient(MQTT_SERVER, 1883, mqttCallback, derWiFi);

DynamicJsonBuffer  jsonBuffer;
JsonObject& sensorInfo1  = jsonBuffer.createObject();
JsonObject& sensorInfo2  = jsonBuffer.createObject();
JsonObject& sensorInfo3  = jsonBuffer.createObject();

JsonObject& mqttTelemetry = jsonBuffer.createObject();

JsonObject& sensorInfo    = jsonBuffer.createObject();

String strJSON = "";


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

char* mqttString (char* payload)
{
  sensorInfo["ID"]=NODE_ID;
}
