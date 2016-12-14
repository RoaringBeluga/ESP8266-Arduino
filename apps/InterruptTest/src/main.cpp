#include "Config.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

void btnOKInterrupt(void);
void btnSelInterrupt(void);
void meterPinIterrupt(void);

bool btnOK_State = false;
bool btnSel_State = false;
uint16_t meterTicks = 0;

LiquidCrystal_PCF8574 lcd(0x27);

void setup()
{
  uint8_t i=0;
  Serial.begin(SERIAL_BAUD);
  lcd.begin(16, 2);lcd.setCursor(1, 0);lcd.setBacklight(128);
  lcd.print("Starting up...");lcd.setCursor(0,1);lcd.print("BME:");
  while(!Serial) {} // Wait

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
      WiFi.printDiag(Serial);
      Serial.println(WiFi.localIP());
      Serial.println(WiFi.softAPIP());
      Serial.println(WiFi.gatewayIP());
      lcd.setCursor(0, 1);lcd.print("WiFi connect: OK");
      delay(5000);
  }
  if(WiFi.isConnected())
  {

  }

  pinMode(btnOK, INPUT);attachInterrupt(btnOK, btnOKInterrupt, RISING);
  pinMode(btnSel, INPUT);attachInterrupt(btnSel, btnSelInterrupt, RISING);
  pinMode(meterPin, INPUT);attachInterrupt(meterPin, meterPinIterrupt, CHANGE);

  lcd.clear();
  lcd.setCursor(0, 0);lcd.print(" meter:");lcd.print(meterTicks);
  lcd.setCursor(0, 1);lcd.print("btnOK:");lcd.print(btnOK_State);
  lcd.print(" btnSel:");lcd.print(btnSel_State);
}

void loop()
{
  Serial.print("btnOK:");Serial.print(btnOK_State);
  Serial.print(" btnSel:");Serial.print(btnSel_State);
  Serial.print(" meterTicks:");Serial.println(meterTicks);
  lcd.setCursor(7, 0);/*lcd.print("        ");lcd.setCursor(7,0);*/lcd.print(meterTicks);
  lcd.setCursor(0, 1);lcd.print("btnOK:");lcd.print(btnOK_State);
  lcd.print(" btnSel:");lcd.print(btnSel_State);
  if(btnOK_State)
  {
    btnOK_State = false;
  };
  if(btnSel_State)
  {
    btnSel_State = false;
  };
  delay(500);
}

void btnOKInterrupt(void)
{
  btnOK_State = true;
}

void btnSelInterrupt(void)
{
  btnSel_State = true;
}

void meterPinIterrupt(void)
{
  meterTicks++;
}
