#pragma once
#include "math/Point.h"
#include <string>

class Door
{
  protected:
  /** Центр масс двери */
  Point *massCenter;

  /** Угол поворота двери относительно оси, радианы */
  float angle;

  void updateMassCenter();

  public:
  /** Вес двери, кг */
    float weight;

  /** Размер двери, м */
    float length;
  
  /** Ось двери */
  Point axis;

  Door(float weight, float length, Point &axis);
  ~Door();

  void setAngle(float angle);
  float getAngle();

  Point getMassCenter();

  /** Печатает угол поворота двери */
  std::wstring printAngle();

  /** Печтает момент вращения двери относительно оси */
  std::wstring printMoment();

  /** Ручка двери */
  Point getGripPoint();

  /** Момент вращения двери относительно оси двери */
  float getMoment();

  /** Момент вращения двери относительно заданной точки */
  float getMoment(const Point &point);

  /** Сила поднятия, приложенная к ручке двери */
//  float getGripForce();
};

