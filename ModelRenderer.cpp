#include "ModelRenderer.h"
#include "Config.h"

ModelRenderer::ModelRenderer(Model* model, int width, int height, float scale): BaseRenderer() {
  this->model = model;
  this->width = width;
  this->height = height;
  this->createWindow(this->width, this->height, scale, { 0xFF, 0xFF, 0xFF, 0xFF });
  this->center = { width*0.36f, height*0.75f };
}

ModelRenderer::~ModelRenderer() {
  this->closeWindow();
}

void ModelRenderer::drawDoor(Door* door) {
  Point p1 = { door->axis.x, door->axis.y };
  Point p2 = door->getGripPoint();
  this->drawLine(&p1, &p2);
  Point c = door->getMassCenter();
  this->scaleToScreen(&c);
  const int centerMassPointRadiusInScreenPixels = 3;
  this->DrawCircleScaled(c, centerMassPointRadiusInScreenPixels);
  c.y -= 10;
  this->drawTextScaled(c, "%.0fHm", door->getMoment(door->axis));
}

void ModelRenderer::drawCord() {
  Point p1 = this->model->bottomDoor->getGripPoint();
  Point p2 = this->model->topDoor->getGripPoint();
  this->drawLine(&p1, &p2);
  // После drawLine p1 и p2 имеют уже экранный масштаб.
  const int labelOffsetInScreenPixels = 5;
  if (this->model->topDoorCordForce != nullptr) {
    Point labelPoint = Point{ (p1.x + p2.x) / 2 + labelOffsetInScreenPixels, (p1.y + p2.y) / 2 };
    this->drawTextScaled(labelPoint, "%.0fH", this->model->topDoorCordForce->value);
  }
}

void ModelRenderer::drawAmmo() {
  Point p1 = this->model->topDoor->getFixedAmmoPoint();
  Point p2 = this->model->topDoor->getAmmoPoint();
  this->drawLine(&p1, &p2);
  // После drawLine p1 и p2 имеют уже экранный масштаб.
  const int labelOffsetInScreenPixels = 5;
  Point labelPoint = Point{ (p1.x + p2.x) / 2 + labelOffsetInScreenPixels, (p1.y + p2.y) / 2 };
  this->drawTextScaled(labelPoint, "%.2f", this->model->ammoLength);
}

void ModelRenderer::drawAngle() {
  Point* labelPoint = new Point{ this->model->topDoor->axis };
  this->scaleToScreen(labelPoint);
  const int labelOffsetInScreenPixels = 20;
  labelPoint->x -= labelOffsetInScreenPixels;
  this->drawTextScaled(*labelPoint, "%.0f°", this->model->topDoor->getAngle()*180/PI);
}

void ModelRenderer::drawForce() {
  Point topDoorGripPoint = this->model->topDoor->getGripPoint();
  float scaledForceLength = this->model->topDoorGripForce->value*0.001f;
  Point forceEndPoint = {
    topDoorGripPoint.x+scaledForceLength * cosf(this->model->topDoorGripForce->angle),
    topDoorGripPoint.y+scaledForceLength * sinf(this->model->topDoorGripForce->angle),
  };
  this->drawLine(&topDoorGripPoint, &forceEndPoint);
  // После drawLine topDoorGripPoint и forceEndPoint имеют уже экранный масштаб.
  float arrowLeftWingAngle = this->model->topDoorGripForce->angle - PI - PI / 6;
  float arrowRightWingAngle = this->model->topDoorGripForce->angle - PI + PI / 6;
  const float arrowWingLengthInScreenPixels = 10;
  
  Point arrowLeftWing = Point {
    forceEndPoint.x + copysignf(arrowWingLengthInScreenPixels, this->model->topDoorGripForce->value) * cosf(arrowLeftWingAngle),
    forceEndPoint.y - copysignf(arrowWingLengthInScreenPixels, this->model->topDoorGripForce->value) * sinf(arrowLeftWingAngle),
  };
  Point arrowRightWing = Point{
    forceEndPoint.x + copysignf(arrowWingLengthInScreenPixels, this->model->topDoorGripForce->value) * cosf(arrowRightWingAngle),
    forceEndPoint.y - copysignf(arrowWingLengthInScreenPixels, this->model->topDoorGripForce->value) * sinf(arrowRightWingAngle),
  };
  this->drawLineScaled(&forceEndPoint, &arrowLeftWing);
  this->drawLineScaled(&forceEndPoint, &arrowRightWing);
  const int labelOffsetInScreenPixels = 10;
  forceEndPoint.x += labelOffsetInScreenPixels;
  forceEndPoint.y -= labelOffsetInScreenPixels;
  this->drawTextScaled(forceEndPoint, "%.0fH", this->model->topDoorGripForce->value);
}

void ModelRenderer::draw() {
  // Верхняя дверь - коричневая
  this->foreground = { 0xAE, 0x7D, 0x1F, 0xFF };
  this->drawDoor(this->model->topDoor);

  // Угол верхней двери.
  this->drawAngle();

  // Усилие для подъема верхней двери.
  this->drawForce();

  // Нижняя дверь - зеленая
  this->foreground = { 0x00, 0xFF, 0x00, 0xFF };
  this->drawDoor(model->bottomDoor);

  // Веревочка - черная
  this->foreground = { 0x00, 0x00, 0x00, 0xFF };
  this->drawCord();

  // Амортизатор - красный
  this->foreground = { 0xFF, 0x00, 0x00, 0xFF };
  this->drawAmmo();
}

