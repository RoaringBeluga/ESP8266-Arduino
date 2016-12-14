#include <Arduino.h>

#ifndef DSU_CONFIG_H
#define DSU_CONFIG_H


#define NET_SSID      "ChinaNet-M5Pc"
#define NET_PASSWD    "upkjbdk7"
#define NET_RETRIES   10

#define MQTT_SERVER   IPAddress(192,168,1,4) //F("")
#define MQTT_USER "desman"
#define MQTT_PASSWD "1234"

enum {
  NODE_CONTROL      = 0x00,
  NODE_TELEMETRY    = 0x01,
  NODE_FLOW_1_GET   = 0x02,
  NODE_FLOW_2_GET   = 0x03,
  NODE_FLOW_3_GET   = 0x04,
  NODE_FLOW_TOTAL   = 0x15
};

const char* MQTT_TOPICS[6] = {
  "bus/sensors/utility/US01_FILTER/control",
  "bus/sensors/utility/US01_FILTER/telemetry",
  "bus/sensors/utility/US01_FILTER/cart1",
  "bus/sensors/utility/US01_FILTER/cart2",
  "bus/sensors/utility/US01_FILTER/cart3",
  "bus/sensors/utility/US01_FILTER/total",
};

#define NODE_ID       "US01_FILTER"
#define SENSOR1_TYPE  "FLOW"
#define SENSOR1_ID    "US01.FILTER.01"
#define SENSOR2_TYPE  "FLOW"
#define SENSOR2_ID    "US01.FILTER.02"
#define SENSOR3_TYPE  "FLOW"
#define SENSOR3_ID    "US01.FILTER.03"

#define NODE_CMD_CHECK    0x00
#define NODE_CMD_RESTART  0x01

#define NODE_OK       0x00
#define NODE_ERR_HW   0xF1
#define NODE_ERR_CMD  0xF0
#define NODE_RESTART  0xFF

#define NTP_SERVER "time.asia.apple.com"

#define PIN_METER     14
#define PIN_BTN_CART  6
#define PIN_BTN_OK    7

#define TICKS_PER_LITRE 1920
const uint16_t cart1_ResL = 7000;
const uint16_t cart2_ResL = 4000;
const uint16_t cart3_ResL = 10000;

const uint16_t cartWarn = 100;

#endif
