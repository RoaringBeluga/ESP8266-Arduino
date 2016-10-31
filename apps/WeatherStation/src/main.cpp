#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include "BME280I2C.h"
#include <LiquidCrystal_PCF8574.h>

#define SERIAL_BAUD 115200

BME280I2C bme;
LiquidCrystal_PCF8574 lcd(0x27);

bool metric = true;

void setup()
{
  Serial.begin(SERIAL_BAUD);
  lcd.begin(16, 2);
  lcd.setCursor(1, 1);
  lcd.setBacklight(128);
  lcd.print("Starting up...");
  while(!Serial) {} // Wait
  while(!bme.begin()){
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }
  lcd.clear();
}

void loop()
{
  float pres=0, temp=0, hum=0;
  bme.read(pres, temp, hum, metric, 0B010);
  lcd.setCursor(0, 0);
  Serial.print("Temp: "); Serial.print(temp,2);Serial.print("°C ");
  lcd.print("T:"); lcd.print(temp,2);
  Serial.print("Humidity: "); Serial.print(hum,2); Serial.print("% ");
  lcd.print(" H:"); lcd.print(hum,2); lcd.print("%");
  Serial.print("Pressure: "); Serial.print(pres*25.4, 2); Serial.print(" mmHg");
  lcd.setCursor(0,1);
  lcd.print("Press:"); lcd.print(pres*25.4, 1); lcd.print(" mmHg");
  Serial.println();
  delay(2000);
}
