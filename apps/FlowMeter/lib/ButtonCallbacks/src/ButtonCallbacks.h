#include <Arduino.h>
#include "Config.h"

#define CYCLE_MAX 3

extern uint16_t doCycle(uint16_t, uint16_t = CYCLE_MAX, uint16_t = 0); // prevVal, maxVal, minVal
