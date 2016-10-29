#ifndef DSU_FLOWMETER_H
#define DSU_FLOWMETER_H
#endif

#include <Arduino.h>

class FlowMeter
{
protected:
  uint16_t  ticksPerLitre = 0;
  uint8_t   pinNumber = 14;

  uint16_t  ticksCurrent = 0;
  uint16_t  litresCurrent = 0;

public:
  FlowMeter(uint16_t, uint16_t); // (pin, ticksPerLitre)
  FlowMeter(uint16_t); // (pin)
  FlowMeter();

  void setPin(uint16_t); // (pin)
  void setTicksPerL(uint16_t); // (ticksPerLitre)
// Flow data setting (i.e. after we restored those from EEPROM)
  void setFlowL(uint16_t); // (fl) Flow in litres
  void setFlowT(uint16_t); // (ft) Flow in ticks

  uint16_t getPin();
  uint16_t getTicksPerL();

  uint16_t getFlowT(); // get flow in ticks
  uint16_t getFlowL(); // get flow in litres
  float getLitres(); // get litres and ml

  void incTick(); // Incrementing the tick counter

};
