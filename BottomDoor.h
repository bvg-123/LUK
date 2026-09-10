#pragma once
#include "Door.h"
#include "TopDoor.h"

class BottomDoor: public Door
{
  public:
  BottomDoor(TopDoor &topDoor);
  ~BottomDoor();
};

