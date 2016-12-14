#include "ButtonCallbacks.h"

uint16_t doCycle(uint16_t prevVal, uint16_t maxVal, uint16_t minVal) // prevVal, maxVal, minVal
{
  uint16_t i = prevVal;
  if(maxVal>minVal)
  {
    return (++i>maxVal?minVal:i);
  } else {
    return (--i<minVal?maxVal:i);
  }
}
