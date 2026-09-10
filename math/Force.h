#pragma once
#include "Point.h"
#include <string>

class Force 
{
  public:
  /** Точка приложения силы. */
  Point point;

  /** Угол направления силы (радианы). */
  float angle;

  /** Величина силы, H. */
  float value;

  /**
  * Конструктор:
  *   forcePoint - точка приложения силы.
  *   angle - угол направления действия силы, радианы.
  *   value - величина силы, Н.
  */
  Force(Point forcePoint, float angle, float value);

  /**
  * Конструктор:
  *   forcePoint - точка приложения силы.
  *   angle - угол направления действия силы, радианы.
  *   moment - величина момента силы, Нм.
  *   momentPoint - точка, относительно которой задана величина момента.
  */
  Force(Point forcePoint, float angle, float moment, Point momentPoint);

  ~Force();

//  Force operator +(Force v);

  /** Возвращает момент силы относительно указанной точки. */
  float getMoment(Point point);

  /** Печатает значение силы и ее направления */
  std::wstring print();
};

