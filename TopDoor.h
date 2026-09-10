#pragma once
#include "Door.h"

class TopDoor : public Door
{
  private:
    float ammoPointOffset;
  public:
  TopDoor();
  ~TopDoor();

  Point getAmmoPoint();
  Point getFixedAmmoPoint();
  float getAmmoLength();
};

