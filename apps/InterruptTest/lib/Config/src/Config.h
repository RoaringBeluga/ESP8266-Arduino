#include <Arduino.h>

#ifndef DSU_CONFIG_H
#define DSU_CONFIG_H
#endif

#define NET_SSID      "ChinaNet-M5Pc"
#define NET_PASSWD    "upkjbdk7"
#define NET_RETRIES   10

#define MQTT_SERVER   IPAddress(192,168,1,4) //F("")
#define MQTT_USER "desman"
#define MQTT_PASSWD "1234"

#define SERIAL_BAUD 115200
/*
enum {
  NODE_CONTROL      = 0,
  NODE_TELEMETRY    = 1,
  NODE_TEMP_SENSOR  = 2,
  NODE_HUMI_SENSOR  = 3,
  NODE_PRES_SENSOR  = 4
};

const char* MQTT_TOPICS[5] = {
  "bus/sensors/ambient/TEST_TEST/control",
  "bus/sensors/ambient/TEST_TEST/telemetry",
  "bus/sensors/ambient/TEST_TEST/temp",
  "bus/sensors/ambient/TEST_TEST/humi",
  "bus/sensors/ambient/TEST_TEST/baro"
};

#define NODE_ID       "TEST_TEST"
#define SENSOR1_TYPE  "TEMP"
#define SENSOR1_ID    "TEST.BME280.01"
#define SENSOR2_TYPE  "HUMI"
#define SENSOR2_ID    "TEST.BME280.02"
#define SENSOR3_TYPE  "PRES"
#define SENSOR3_ID    "TEST.BME280.03"

#define in2mm(in_v)   (in_v*25.4)

#define NODE_CMD_CHECK    0x00
#define NODE_CMD_RESTART  0x01

#define NODE_OK       0x00
#define NODE_ERR_HW   0xF1
#define NODE_ERR_CMD  0xF0
#define NODE_RESTART  0xFF
*/

#define btnOK     13
#define btnSel    12
#define meterPin  14
