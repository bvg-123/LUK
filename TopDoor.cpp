#include "TopDoor.h"
#include "Config.h"

TopDoor::TopDoor(): Door(TOP_DOOR_WEIGHT, TOP_DOOR_LENGTH, *new Point(0, 0))  {
  this->ammoPointOffset = AMMO_AXIS_HORIZONTAL_OFFSET + sqrt(pow(AMMO_MIN_LENGTH, 2) - pow(AMMO_AXIS_VERTICAL_OFFSET-AMMO_TOP_DOOR_OFFSET, 2));
}

TopDoor::~TopDoor() {
}

/**
// image-comment: [.codeimages/img-AmmoPoint.png]
*/
Point TopDoor::getAmmoPoint() {
  return {
    this->axis.x + this->ammoPointOffset*cosf(this->angle) + AMMO_TOP_DOOR_OFFSET * cosf(this->angle - PI/2),
    this->axis.y + this->ammoPointOffset*sinf(this->angle) + AMMO_TOP_DOOR_OFFSET * sinf(this->angle - PI/2),
  };
}

Point TopDoor::getFixedAmmoPoint() {
  return { 
    this->axis.x + AMMO_AXIS_HORIZONTAL_OFFSET,
    this->axis.y - AMMO_AXIS_VERTICAL_OFFSET
  };
}

float TopDoor::getAmmoLength() {
  Point ammoPoint = this->getAmmoPoint();
  Point fixedAmmoPoint = this->getFixedAmmoPoint();
  float ammoLength = sqrt(pow(ammoPoint.x-fixedAmmoPoint.x, 2)+pow(ammoPoint.y- fixedAmmoPoint.y, 2));
  return ammoLength;
}