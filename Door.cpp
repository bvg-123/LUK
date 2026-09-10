#include "Config.h"
#include "Door.h"
#include <math.h>
#include <sstream>
#include <iomanip>

Door::Door(float weight, float length, Point &axis) {
  this->weight = weight;
  this->length = length;
  this->angle = 0;
  this->axis = axis;
  this->massCenter = new Point((this->length/2)*cos(this->angle), (this->length/2)*sin(this->angle));
}

Door::~Door() {
  delete this->massCenter;
  this->massCenter = nullptr;
  this->angle = 0;
  this->length = 0;
  this->weight = 0;
}

void Door::updateMassCenter() {
  this->massCenter = new Point(this->axis.x + (this->length / 2) * cosf(this->angle), this->axis.y + (this->length / 2) * sinf(this->angle));
}

void Door::setAngle(float angle) {
  this->angle = angle;
  this->updateMassCenter();
}

float Door::getAngle() {
  return this->angle;
}

Point Door::getMassCenter() {
  return {
    this->massCenter->x,
    this->massCenter->y
  };
}

std::wstring Door::printAngle() {
  std::wostringstream sstream;
  sstream << std::setprecision(0) << std::fixed << this->angle*180/PI << L"°";
  return sstream.str();
}

std::wstring Door::printMoment() {
  std::wostringstream sstream;
  sstream << std::setprecision(0) << std::fixed << this->getMoment() << L" Нм";
  return sstream.str();
}


/** Ручка двери */
Point Door::getGripPoint() {
  return {
    this->axis.x+(this->length)*cos(this->angle),
    this->axis.y+(this->length)*sin(this->angle)
  };
}

/** Момент вращения двери относительно оси двери */
float Door::getMoment() {
  return this->getMoment(this->axis);
}

/** Момент вращения двери относительно заданной точки */
float Door::getMoment(const Point &point) {
  float weightArm = sqrt(pow((this->massCenter->x - point.x), 2) + pow((this->massCenter->y - point.y), 2))*cosf(this->angle);
  float moment = this->weight*G*weightArm;
  return moment;
}

