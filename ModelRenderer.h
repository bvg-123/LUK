#pragma once
#include "Model.h"
#include "BaseRenderer.h"


class ModelRenderer: public BaseRenderer
{
  private:
    int width;
    int height;

    Model* model;

    void drawDoor(Door* door);
    void drawCord();
    void drawAmmo();
    void drawAngle();
    void drawForce();

  public:
  ModelRenderer(Model* model, int width, int height, float scale = 100);
  ~ModelRenderer();

  void draw() override;
};

