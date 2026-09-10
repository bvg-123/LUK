#include "../Config.h"
#include "Force.h"
#include <math.h>
#include <sstream>
#include <iomanip>
#include <Windows.h>

Force::Force(Point forcePoint, float angle, float value) {
  this->point = forcePoint;
  this->angle = angle;
  this->value = value;
}

/**
* // image-comment: [../.codeimages/img-ForceMoment.png]
*/
Force::Force(Point forcePoint, float angle, float moment, Point momentPoint) {
  float d = sqrt(pow(forcePoint.x-momentPoint.x, 2)+pow(forcePoint.y-momentPoint.y, 2));
  float beta = acos((forcePoint.x-momentPoint.x)/d);
  float arm = abs(sin(beta-angle)*d);
  this->point = forcePoint;
  this->angle = angle;
  this->value = moment/arm;
}

Force::~Force() {
  this->point.~Point();
  this->angle = 0;
  this->value = 0;
}

/*
Force Force::operator +(Force v) {
  float xEnd = this->x+this->length*cos(this->angle);
  float yEnd = this->y+this->length*sin(this->angle);
  xEnd

  if (fabs(this->a) < 0.0001) {

  } else {
    float a = atan(v.y-this->y*(1-sin(this->a)))/(v.x-this->x*(1-cos(this->a)));
    return *new Vector(this->x, this->y, a);
  }
}
*/

float Force::getMoment(Point point) {
  // Плечо силы
//  float forceArm = fabs(-sin(this->angle)*point.x+point.y+this->point.x*sin(this->angle)-this->point.y)/sqrt(pow(sin(this->angle), 2)+1);
  float d = sqrt(pow(this->point.x - point.x, 2) + pow(this->point.y - point.y, 2));
  float beta = acos((this->point.x - point.x) / d);
  float forceArm = abs(sin(beta - this->angle) * d);

  // Момент силы
  float moment = forceArm*this->value;
  return moment;
}

std::wstring Force::print() {
  std::wostringstream sstream;
  sstream << std::setprecision(0) << std::fixed << this->value << L" Н" << L", " << this->angle*180/PI << L"°";
  std::wstring result = sstream.str();
//  const WCHAR* qq = result.c_str();
  return result;
}