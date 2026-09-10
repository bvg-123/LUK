#include "BottomDoor.h"
#include "Config.h"

BottomDoor::BottomDoor(TopDoor &topDoor) : Door(
  BOTTOM_DOOR_WEIGHT,
  BOTTOM_DOOR_LENTH,
  *new Point(topDoor.axis.x + AXES_HORIZONTAL_OFFSET, topDoor.axis.y - AXES_VERTICAL_OFFSET)) {
}

BottomDoor::~BottomDoor() {
}

