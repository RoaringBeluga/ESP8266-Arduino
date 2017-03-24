#ifndef DSU_EEPROM_H
#define DSU_EEPROM_H

#include <Wire.h>
#include <Arduino.h>

class EEPROM
{
private:
  uint8_t IIC_ADDR = 0x57;
public:
  EEPROM();
  EEPROM(uint8_t a);

  uint8_t read(const uint16_t address);
  void    write(const uint16_t address, const uint8_t data);

};
#endif
