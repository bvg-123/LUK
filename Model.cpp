#include "Config.h"
#include "Model.h"
#include "math/Force.h"
#include <math.h>
#include <windows.h>
#include <iostream>

Model::Model() {
  this->topDoor = new TopDoor();
  this->bottomDoor = new BottomDoor(*this->topDoor);
  this->topDoorMaxAngle = PI-atan(AXES_VERTICAL_OFFSET/AXES_HORIZONTAL_OFFSET);
  this->ammoLength = 0;
  this->bottomDoorAngle = 0;
  this->bottomDoorMoment = 0;
}

Model::~Model() {
  delete this->bottomDoor;
  this->bottomDoor = nullptr;
  delete this->topDoor;
  this->topDoor = nullptr;
}

/*
// image-comment: [.codeimages/img-BottomDoorAngle.png]
*/
bool Model::getBottomDoorAngle(float x1, float y1, float R1, float x2, float y2, float R2, float* bottomDoorAngle, float* x, float* y) {
  float z = (pow(R2, 2) - pow(R1, 2) + pow(x1, 2) - pow(x2, 2) + pow(y1, 2) - pow(y2, 2)) / 2.0 / (x1 - x2);
  float a = 1.0 + pow((y2 - y1) / (x1 - x2), 2);
  float b = 2 * ((z - x1) * (y2 - y1) / (x1 - x2) - y1);
  float c = pow(z - x1, 2) + pow(y1, 2) - pow(R1, 2);
  float D = pow(b, 2) - 4 * a * c;
  if (D < 0) {
    *y = 0;
    *x = 0;
    *bottomDoorAngle = 0;
    return false;
  }

  // Из двух корней берем тот, для которого угол нижней двери будет мельше угла верхней двери (физический вариант).
  float yy1 = (-b + sqrt(D)) / 2 / a;
  float yy2 = (-b - sqrt(D)) / 2 / a;
  float xx1 = yy1 * (y2 - y1) / (x1 - x2) + z;
  float xx2 = yy2 * (y2 - y1) / (x1 - x2) + z;
  float angle1 = atan2(yy1 - y1, xx1 - x1);
  float angle2 = atan2(yy2 - y1, xx2 - x1);

  float bottomDoorAnglePrev = this->bottomDoor->getAngle();
  float bottomDoorAngleDelta1 = angle1 - bottomDoorAnglePrev;
  float bottomDoorAngleDelta2 = angle2 - bottomDoorAnglePrev;

  if (bottomDoorAngleDelta1 < bottomDoorAngleDelta2) {
    *bottomDoorAngle = angle1;
    *x = xx1;
    *y = yy1;
  } else {
    *bottomDoorAngle = angle2;
    *x = xx2;
    *y = yy2;
  }
  if (*y < y1) {
    *y = 0;
    *x = 0;
    *bottomDoorAngle = 0;
    return false;
  }
  return true;
}


bool Model::calculate() {
  this->topDoorGripPoint = this->topDoor->getGripPoint();
  this->ammoLength = this->topDoor->getAmmoLength();
  if (this->ammoLength > AMMO_MAX_LENGTH) {
    return false;
  }
  float x = 0;
  float y = 0;
  this->isCordTense = this->getBottomDoorAngle(this->bottomDoor->axis.x, this->bottomDoor->axis.y, this->bottomDoor->length, this->topDoorGripPoint.x, this->topDoorGripPoint.y, CORD_LENGTH, &this->bottomDoorAngle, &x, &y);
  if (this->isCordTense) {
    this->bottomDoor->setAngle(this->bottomDoorAngle);
    this->bottomDoorMoment = this->bottomDoor->getMoment();
  }
  else {
    this->bottomDoor->setAngle(0);
  }
  this->bottomDoorGripPoint = this->bottomDoor->getGripPoint();
  /*
  * // image-comment: [.codeimages/img-Cord.png]
  */
  this->bottomDoorGripForce = nullptr;
  this->topDoorCordForce = nullptr;
  this->topDoorCordMoment = 0;
  if (this->isCordTense) {
    this->cordAngle = atan2(this->topDoorGripPoint.y - this->bottomDoorGripPoint.y, this->topDoorGripPoint.x - this->bottomDoorGripPoint.x);
    this->bottomDoorGripForce = new Force(this->bottomDoorGripPoint, this->cordAngle, this->bottomDoorMoment, this->bottomDoor->axis);
    this->topDoorCordForce = new Force(this->topDoorGripPoint, this->cordAngle - PI, this->bottomDoorGripForce->value);
    // Отрицательная сила натяжения веревки - это ее провисание на последних углах открытия и отсутствие влияния момента нижней двери на верхнюю.
    if (this->topDoorCordForce->value > 0) {
      this->topDoorCordMoment = this->topDoorCordForce->getMoment(this->topDoor->axis);
    }
  }
  this->topDoorAmmoPoint = this->topDoor->getAmmoPoint();
  this->ammoAngle = atan2(this->topDoorAmmoPoint.y - (this->topDoor->axis.y - AMMO_AXIS_VERTICAL_OFFSET), this->topDoorAmmoPoint.x - (this->topDoor->axis.x + AMMO_AXIS_HORIZONTAL_OFFSET));
  this->topDoorAmmoForce = new Force(this->topDoorAmmoPoint, this->ammoAngle, AMMO_FORCE * AMMO_COUNT);
  this->topDoorAmmoMoment = this->topDoorAmmoForce->getMoment(this->topDoor->axis);
  float dummyTopDoorAngle = topDoor->getAngle();
  float dummyAmmoForceAngle = this->topDoorAmmoForce->angle;
  if (dummyAmmoForceAngle < dummyTopDoorAngle) {
    return false;
  }
  this->topDoorMoment = this->topDoor->getMoment() - this->topDoorAmmoMoment + this->topDoorCordMoment;
  this->topDoorGripForceAngle = this->topDoor->getAngle() + PI / 2;  // Будем за верхнюю ручку тянуть под прямым углом к плоскости верхней двери.
//  this->topDoorGripForceAngle = this->cordAngle;  // Другой вариант - тянуть противоположно веревочке.
  this->topDoorGripForce = new Force(this->topDoorGripPoint, this->topDoorGripForceAngle, this->topDoorMoment, this->topDoor->axis);
  Point fixedAmmoPoint = this->topDoor->getFixedAmmoPoint();
  if (this->ammoAngle > atan2(topDoor->axis.y - fixedAmmoPoint.y, topDoor->axis.x - fixedAmmoPoint.x)) {
    return false;
  }
}

bool Model::setTopDoorAngle(float& angle) {
  bool result = true;
  float prevAngle = this->topDoor->getAngle();
  if (angle > this->topDoorMaxAngle) {
    this->topDoor->setAngle(angle = this->topDoorMaxAngle);
    this->calculate();
    return false;
  }
  if (angle < 0) {
    this->topDoor->setAngle(angle = 0);
    result = false;
  } else {
    this->topDoor->setAngle(angle);
  }
  result = result && this->calculate();
  if (!result) {
    this->topDoor->setAngle(angle = prevAngle);
    this->calculate();
  }
  return result;
}