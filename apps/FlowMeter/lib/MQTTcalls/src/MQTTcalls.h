#ifndef DSU_MQTTCALLS_H
#define DSU_MQTTCALLS_H

#include "Config.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

#include <ESP8266WiFi.h>

extern void   mqttCallback(char* topic, byte* payload, unsigned int length);
//extern char*  mqttString(char* payload);

#endif
