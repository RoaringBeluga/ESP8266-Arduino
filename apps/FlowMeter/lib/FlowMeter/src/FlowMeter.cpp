#include "FlowMeter.h"

FlowMeter::FlowMeter(uint16_t pin, uint16_t ticks)
{
  pinNumber = pin;
  ticksPerLitre = ticks;
}

FlowMeter::FlowMeter(uint16_t pin)
{
  pinNumber = pin;
}

FlowMeter::FlowMeter()
{

}

void FlowMeter::start()
{

}

void FlowMeter::setPin(uint16_t pin)
{
  pinNumber = pin;
}

void FlowMeter::setTicksPerL(uint16_t ticks)
{
  ticksPerLitre = ticks;
}

void FlowMeter::setFlowL(uint16_t fl)
{
  litresCurrent = fl;
}

void FlowMeter::setFlowT(uint16_t ft)
{
  ticksCurrent = ft;
}

uint16_t FlowMeter::getPin()
{
  return pinNumber;
}

uint16_t FlowMeter::getTicksPerL()
{
  return ticksPerLitre;
}

uint16_t FlowMeter::getFlowT()
{
  return ticksCurrent;
}

uint16_t FlowMeter::getFlowL()
{
  return litresCurrent;
}

float FlowMeter::getLitres()
{
  return litresCurrent+(ticksCurrent/ticksPerLitre);
}

void FlowMeter::incTick()
{
  if((ticksCurrent++)>ticksPerLitre)
  {
    litresCurrent++;
    ticksCurrent = 0;
  }
}
