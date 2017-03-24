#include <Arduino.h>
#include <Wire.h>
#include <pgmspace.h>
#include <RtcDS3231.h>
#include <ESP8266WiFi.h>

#include "EEPROM.h"

#define SERIAL_BAUD 115200

EEPROM    rtcPROM;

const uint16_t  addrTicks   = 0x10;
const uint16_t  addrLitres  = 0x12;
const uint16_t  addrResCL   = 0x14;
const uint16_t  addrResCM   = 0x16;
const uint16_t  addrResCR   = 0x18;

void storeInt(uint16_t a, int data);
int readInt(uint16_t a);

void store_eeprom();

void setup()
{
  Serial.begin(115200);
  store_eeprom();
}

void loop()
{
  Serial.println("Finished!");
}

void storeInt(uint16_t a, int data)
{
    rtcPROM.write(a, highByte(data));
    rtcPROM.write(a+1, lowByte(data));
}

int readInt(uint16_t a)
{
  return word(rtcPROM.read(a),rtcPROM.read(a+1));
}

void store_eeprom()
{
  storeInt(addrLitres, 0);
  if(0!=readInt(addrLitres)) {
    Serial.println("Litres NOT stored");
  } else {
    Serial.println("Litres stored");
  };
  storeInt(addrTicks, 0);
  if(0!=readInt(addrTicks)) {
    Serial.println("Ticks NOT stored");
  } else {
    Serial.println("Ticks stored");
  };
  storeInt(addrResCL, 6000);
  if(6000!=readInt(addrResCL)) {
    Serial.println("Resource:LEFT NOT stored");
  } else {
    Serial.println("Resource:LEFT stored");
  };
  storeInt(addrResCM, 3000);
  if(3000!=readInt(addrResCM)) {
    Serial.println("Resource:MID NOT stored");
  } else {
    Serial.println("Resource:MID stored");
  };
  storeInt(addrResCR, 9000);
  if(9000!=readInt(addrResCR)) {
    Serial.println("Resource:RIGHT NOT stored");
  } else {
    Serial.println("Resource:RIGHT stored");
  };
}
