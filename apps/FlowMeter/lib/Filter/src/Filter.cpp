#include "Filter.h"

Filter::Filter()
{

}

Filter::Filter(int CL, int CM, int CR)
{
  cartResourse[CART_L] = CL;
  cartResourse[CART_M] = CM;
  cartResourse[CART_R] = CR;
}

void Filter::setResource(FCart c, int r)
{
  if(c!=CART_NONE)cartResourse[c] = r;
};

uint16_t Filter::getResource(FCart c)
{
  return (c!=CART_NONE?cartResourse[c]:0);
};

uint8_t Filter::incLitres()
{
  uint8_t r=0;
  for(uint8_t i=0;i<3;i++)
  {
    cartResourse[i]--;
    if(cartResourse[i]<cartThreshold){
      toReplace[i]=true;
      r=(i==0?1:r+(2*i));
    };
  };
  return r;
};

uint8_t Filter::addLitres(uint16_t l)
{
  uint8_t r=0;
  for(uint8_t i=0;i<3;i++)
  {
    cartResourse[i]-=l;
    if(cartResourse[i]<cartThreshold){
      toReplace[i]=true;
      r=(i==0?1:r+(2*i));
    };
  };
  return r;
};

uint8_t Filter::getReplace()
{
  uint8_t r;
  if(toReplace[0])r&=0x01;
  if(toReplace[1])r&=0x02;
  if(toReplace[2])r&=0x04;
  return r;
};

void Filter::replaceCart(FCart c, int r)
{
  if(c!=CART_NONE)cartResourse[c] = r;
};
