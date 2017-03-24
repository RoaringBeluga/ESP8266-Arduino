#include <Arduino.h>
#include <Wire.h>
#include <pgmspace.h>
#include <RtcDS3231.h>
#include <ESP8266WiFi.h>
#include <LiquidCrystal_PCF8574.h>
#include <RBD_Timer.h>
//#include <ArduinoJson.h>
//#include <PubSubClient.h>
//#include "MQTTcalls.h"

#include "Config.h"
#include "FlowMeter.h"
#include "Filter.h"
#include "EEPROM.h"
#include "ButtonCallbacks.h"

#define SERIAL_BAUD 115200

//void reconnect();

LiquidCrystal_PCF8574 lcd(0x27);

uint16_t addrOffset = 0x00;

bool WiFiOn = false;
bool mqttOn = false;

bool metric = true;

bool offsetRead = false;

bool btnOKPressed = false;
bool btnSelPressed = false;
uint8_t btnSelConfirmed = 0;

uint16_t btnOKState = 0;

RtcDS3231 Rtc;
RBD::Timer btnOKTimer(5000);
RBD::Timer btnSelTimer(5000);
RBD::Timer romWrite(60000);
EEPROM    rtcPROM;
Filter    theFilter;
FlowMeter theMeter;

void storeInt(uint16_t a, int data);
int readInt(uint16_t a);

void btnOKInterrupt();
void btnSelInterrupt();
void meterInterrupt();

void retrieve_eeprom(uint16_t aO);
void store_eeprom();

void setup()
  {
    pinMode(PIN_BTN_OK, INPUT);if(digitalRead(PIN_BTN_OK)==HIGH)offsetRead=true;
    Serial.begin(SERIAL_BAUD);
    lcd.begin(16, 2);lcd.setCursor(1, 0);lcd.setBacklight(128);
    lcd.print("Starting up...");lcd.setCursor(0,1);lcd.print("BME:");
    while(!Serial) {} // Wait

    WiFi.begin(NET_SSID, NET_PASSWD);
    WiFi.config(IPAddress(192,168,1,10),IPAddress(192,168,1,1),IPAddress(255,255,255,0),IPAddress(8,8,8,8),IPAddress(8,8,4,4));
    int i = -1; lcd.setCursor(0,1); lcd.print("Net:");
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
    if(offsetRead)addrOffset = 0x10;
    retrieve_eeprom(addrOffset);

    attachInterrupt(PIN_BTN_OK, btnOKInterrupt, RISING);
    pinMode(PIN_BTN_CART, INPUT);attachInterrupt(PIN_BTN_CART, btnSelInterrupt, RISING);
    pinMode(PIN_METER, INPUT);attachInterrupt(PIN_METER, meterInterrupt, CHANGE);

    lcd.clear();
    lcd.setCursor(0, 1); lcd.print(" Meter is ready ");
    lcd.setCursor(0, 0); lcd.print("L: "); lcd.print(theMeter.getFlowL());
    lcd.setCursor(8, 0); lcd.print("T: "); lcd.print(theMeter.getFlowT());
    delay(5000);
    lcd.setBacklight(0);
    romWrite.restart();
  }

void loop()
  {
    if(btnOKPressed)
    {
      btnOKState = doCycle(btnOKState);
      btnOKPressed = false;
      switch(btnOKState)
      {
        case btnOK_L:
                  lcd.setBacklight(128);
                  lcd.clear();
                  lcd.setCursor(0, 0); lcd.print("New:LEFT Sel=YES");
                  lcd.setCursor(0, 1); lcd.print("Resource: ");lcd.print(theFilter.getResource(CART_L));
                  break;
        case btnOK_M:
                  lcd.setBacklight(128);
                  lcd.clear();
                  lcd.setCursor(0, 0); lcd.print("New:MID  Sel=YES");
                  lcd.setCursor(0, 1); lcd.print("Resource: ");lcd.print(theFilter.getResource(CART_M));
                  break;
        case btnOK_R:
                  lcd.setBacklight(128);
                  lcd.clear();
                  lcd.setCursor(0, 0); lcd.print("New:RGHT Sel=YES");
                  lcd.setCursor(0, 1); lcd.print("Resource: ");lcd.print(theFilter.getResource(CART_R));
                  break;
        case btnOK_NONE:
        default:
                  lcd.setBacklight(128);
                  lcd.clear();
                  lcd.setCursor(0, 0); lcd.print("L: "); lcd.print(theMeter.getFlowL());
                  lcd.setCursor(8, 0); lcd.print("T: "); lcd.print(theMeter.getFlowT());
                  lcd.setCursor(0, 1); lcd.print("Cartridges: "); lcd.print(theFilter.getReplace(), BIN);
                  break;
      };
      //start OK button countdown...
      btnOKTimer.restart();
    };
    if(btnSelPressed)
    {
      btnSelConfirmed++;
      btnSelPressed = false;
      if((btnSelConfirmed++)==1)
      {
        //start SELECT button countdown...
        lcd.setCursor(0, 1); lcd.print(" SEL to confirm ");
        btnSelTimer.restart();
      } else {
        btnSelConfirmed = 0;
        switch(btnOKState)
        {
          case btnOK_L:
                    theFilter.replaceCart(CART_L, cart1_ResL);
                    lcd.setBacklight(128);
                    lcd.clear();
                    lcd.setCursor(0, 0); lcd.print("  LEFT changed");
                    lcd.setCursor(0, 1);
                    lcd.print("Resource: ");lcd.print(theFilter.getResource(CART_L));
                    break;
          case btnOK_M:
                    theFilter.replaceCart(CART_M, cart2_ResL);
                    lcd.setBacklight(128);
                    lcd.clear();
                    lcd.setCursor(0, 0); lcd.print("  MIDDLE changed");
                    lcd.setCursor(0, 1);
                    lcd.print("Resource: ");lcd.print(theFilter.getResource(CART_M));
                    break;
          case btnOK_R:
                    theFilter.replaceCart(CART_R, cart3_ResL);
                    lcd.setBacklight(128);
                    lcd.clear();
                    lcd.setCursor(0, 0); lcd.print("  RIGHT changed");
                    lcd.setCursor(0, 1);
                    lcd.print("Resource: ");lcd.print(theFilter.getResource(CART_R));
                    break;
          case btnOK_NONE:
          default:
                    lcd.setBacklight(128);
                    lcd.clear();
                    lcd.setCursor(0, 0); lcd.print("L: "); lcd.print(theMeter.getFlowL());
                    lcd.setCursor(8, 0); lcd.print("T: "); lcd.print(theMeter.getFlowT());
                    lcd.setCursor(0, 1); lcd.print("Cartridges: "); lcd.print(theFilter.getReplace(), BIN);
                    break;
        }
      }
    };
    if(btnOKTimer.isExpired())
    {
      btnOKState = btnOK_NONE;
      lcd.setBacklight(0);
      btnOKTimer.stop();
    }; // OK button timed out
    if(btnSelTimer.isExpired())
    {
      btnOKState =  btnOK_NONE;
      btnOKTimer.stop();
      btnSelTimer.stop();
      lcd.setBacklight(0);
    }; // SELECT button timed out
    theFilter.addLitres(theMeter.litresIncremented());
    if(romWrite.onRestart()) store_eeprom();
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

void btnOKInterrupt()   { btnOKPressed  = true; }

void btnSelInterrupt()  { btnSelPressed = true; }

void meterInterrupt()   { theMeter.incTick();   }

void retrieve_eeprom(uint16_t aO)
{
  theMeter.setFlowL(readInt(addrLitres+aO));
  theMeter.setFlowT(readInt(addrTicks+aO));
  theFilter.setResource(CART_L, readInt(addrResCL+aO));
  theFilter.setResource(CART_M, readInt(addrResCM+aO));
  theFilter.setResource(CART_R, readInt(addrResCR+aO));
}

void store_eeprom()
{
  storeInt(addrLitres, theMeter.getFlowL());
  storeInt(addrTicks, theMeter.getFlowT());
  storeInt(addrResCL, theFilter.getResource(CART_L));
  storeInt(addrResCM, theFilter.getResource(CART_M));
  storeInt(addrResCR, theFilter.getResource(CART_R));
}
