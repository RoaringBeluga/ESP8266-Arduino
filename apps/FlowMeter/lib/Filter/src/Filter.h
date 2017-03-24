#ifndef DSU_FILTER_H
#define DSU_FILTER_H
#include <Arduino.h>

typedef enum {
  CART_L      = 0x00,
  CART_M      = 0x01,
  CART_R      = 0x02,
  CART_NONE   = 0xFF
} FCart;

class Filter {
protected:
  int  cartResourse[3];
  uint16_t  cartThreshold = 100;
  bool      toReplace[3];
public:
  Filter();
  Filter(int CL, int CM, int CR);

  void setResource(FCart c, int r);

  uint16_t getResource(FCart c);

  uint8_t incLitres();

  uint8_t addLitres(uint16_t l);

  uint8_t getReplace();

  void replaceCart(FCart c, int r);
};

#endif
