#pragma once
#include "TopDoor.h"
#include "BottomDoor.h"
#include "math/Force.h"

class Model
{
  private:
    bool getBottomDoorAngle(float x1, float y1, float R1, float x2, float y2, float R2, float* bottomDoorAngle, float* x, float* y);
    bool calculate();
public:
  float topDoorMaxAngle;
    TopDoor* topDoor;
    BottomDoor* bottomDoor;
    Point topDoorGripPoint;
    float ammoLength;
    bool isCordTense;
    float bottomDoorAngle;
    float bottomDoorMoment;
    Point bottomDoorGripPoint;
    float cordAngle;
    Force* bottomDoorGripForce;
    Force* topDoorCordForce;
    float topDoorCordMoment;
    Point topDoorAmmoPoint;
    float ammoAngle;
    Force* topDoorAmmoForce;
    float topDoorAmmoMoment;
    float topDoorMoment;
    float topDoorGripForceAngle;
    Force* topDoorGripForce;

    Model();
    ~Model();

    bool setTopDoorAngle(float& angle);
};

