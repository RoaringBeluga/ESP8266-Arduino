#include "EEPROM.h"

EEPROM::EEPROM()
{

}

EEPROM::EEPROM(uint8_t a)
{
  IIC_ADDR = a;
}

uint8_t EEPROM::read(const uint16_t address)
{
	unsigned int rdata = 0xFF;
	Wire.beginTransmission(IIC_ADDR);
	Wire.write((int)(address >> 8)); // MSB
	Wire.write((int)(address & 0xFF)); // LSB
	Wire.endTransmission();
	Wire.requestFrom(IIC_ADDR, 1);
	if (Wire.available()) {
		rdata = Wire.read();
	}
	return rdata;
}

void EEPROM::write(const uint16_t address, const uint8_t data) {
		Wire.beginTransmission(IIC_ADDR);
		Wire.write((int)(address >> 8)); // MSB
		Wire.write((int)(address & 0xFF)); // LSB
		Wire.write(data);
		delay(5); // Little delay to assure EEPROM is able to process data; if missing and inside for look meses some values
		Wire.endTransmission();
}
