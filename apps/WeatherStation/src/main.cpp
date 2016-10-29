#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include "BME280I2C.h"

#define SERIAL_BAUD 115200

BME280I2C bme;

bool metric = false;

void setup()
{
  Serial.begin(SERIAL_BAUD);
  while(!Serial) {} // Wait
  while(!bme.begin()){
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }
}

void loop()
{
  float pres=0, temp=0, hum=0;
  bme.read(pres, temp, hum, metric, 0B010);
  Serial.printf("Temp: %f Humidity: %f Pressure: %f", temp, pres, hum);
  Serial.println();
  delay(2000);
}
