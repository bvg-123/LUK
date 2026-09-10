#pragma once
#include "BaseRenderer.h"
#include <iostream>
#include <stdarg.h>

BaseRenderer::BaseRenderer() {
  this->initSDL();
}

BaseRenderer::~BaseRenderer() {
  this->doneSDL();
}

void BaseRenderer::initSDL() {
  isSDLInitialized = false;
  if (SDL_Init(SDL_INIT_VIDEO)==false) {
    std::wcout<<"SDL could not initialize! SDL error: "<<SDL_GetError()<<std::endl;
    return;
  }
  this->center = {0, 0};
  this->scale = 1;
  this->background = { 0, 0, 0, 0 };
  this->foreground = { 0xFF, 0xFF, 0xFF, 0xFF };
  isSDLInitialized = true;
}

void BaseRenderer::doneSDL() {
  if (this->isSDLInitialized) {
    SDL_Quit();
    this->isSDLInitialized = false;
  }
}

void BaseRenderer::createWindow(int width, int height, float scale, SDL_Color background) {
  this->center = { width/2.0f, height/2.0f };
  this->scale = scale;
  this->background = background;

  if (SDL_CreateWindowAndRenderer(u8"Визуализация моментов", width, height, 0, &this->gWindow, &this->gRenderer)==false) {
    std::wcout<<"Window could not be created! SDL error: "<<SDL_GetError()<<std::endl;
    SDL_Quit();
    return;
  }
  this->gScreenSurface = SDL_GetWindowSurface(this->gWindow);
  if (!this->gScreenSurface) {
    std::wcout<<"Getting surface is failed! SDL error: "<<SDL_GetError()<<std::endl;
    SDL_DestroyWindow(this->gWindow);
    this->gWindow = nullptr;
    SDL_Quit();
    return;
  }
}

void BaseRenderer::closeWindow() {
  SDL_DestroyRenderer(this->gRenderer);
  this->gRenderer = nullptr;
  SDL_DestroySurface(this->gScreenSurface);
  this->gScreenSurface = nullptr;
  SDL_DestroyWindow(this->gWindow);
  this->gWindow = nullptr;
}

Point BaseRenderer::scaleToScreen(Point* point) {
  point->x = this->center.x+point->x*this->scale;
  point->y = this->center.y-point->y*this->scale;
  return *point;
}

void BaseRenderer::Execute() {
  this->clear();
  this->draw();
  SDL_RenderPresent(gRenderer);
}

void BaseRenderer::draw() {
}

void BaseRenderer::clear() {
  SDL_SetRenderDrawColor(gRenderer, this->background.r, this->background.g, this->background.b, this->background.a);
  SDL_RenderClear(gRenderer);
}

void BaseRenderer::drawLine(Point* point1, Point* point2) {
  /*
      SDL_RenderLine(gRenderer, 0, kScreenHeight/2-100, 0, kScreenHeight/2+100);
      SDL_RenderLine(gRenderer, 0, kScreenHeight/2+100, 200, kScreenHeight/2+100);
      SDL_RenderLine(gRenderer, 200, kScreenHeight/2+100, 200, kScreenHeight/2-100);
      SDL_RenderLine(gRenderer, 200, kScreenHeight/2-100, 0, kScreenHeight/2-100);
  */
  SDL_SetRenderDrawColor(gRenderer, this->foreground.r, this->foreground.g, this->foreground.b, this->foreground.a);
  Point p1 = this->scaleToScreen(point1);
  Point p2 = this->scaleToScreen(point2);
  SDL_RenderLine(this->gRenderer, point1->x, point1->y, point2->x, point2->y);
}

void BaseRenderer::drawLineScaled(Point* scaledPoint1, Point* scaledPoint2) {
  SDL_SetRenderDrawColor(gRenderer, this->foreground.r, this->foreground.g, this->foreground.b, this->foreground.a);
  SDL_RenderLine(this->gRenderer, scaledPoint1->x, scaledPoint1->y, scaledPoint2->x, scaledPoint2->y);
}

void BaseRenderer::drawText(Point& point, const char* const format, ...) {
  Point scaledPoint = this->scaleToScreen(&point);
  va_list argPtr;
  va_start(argPtr, format);
  this->vdrawTextScaled(scaledPoint, format, argPtr);
  va_end(argPtr);
}

void BaseRenderer::vdrawTextScaled(Point& scaledPoint, const char* const format, va_list argPtr) {
  SDL_SetRenderDrawColor(gRenderer, this->foreground.r, this->foreground.g, this->foreground.b, this->foreground.a);
  size_t needed = vsnprintf(NULL, 0, format, argPtr) + 1;
  char* text = (char*)malloc(needed);
  vsnprintf(text, needed, format, argPtr);
  SDL_RenderDebugText(this->gRenderer, scaledPoint.x, scaledPoint.y, text);
  free((void*)text);
}

void BaseRenderer::drawTextScaled(Point& scaledPoint, const char* const format, ...) {
  va_list argPtr;
  va_start(argPtr, format);
  this->vdrawTextScaled(scaledPoint, format, argPtr);
  va_end(argPtr);
}

int roundUpToMultipleOfEight(int v)
{
  return (v + (8 - 1)) & -8;
}

void BaseRenderer::DrawCircleScaled(Point center, int radius)
{
  const int arrSize = roundUpToMultipleOfEight(radius * 8 * 35 / 49);
  SDL_FPoint* points = new SDL_FPoint[arrSize];
  int drawCount = 0;

  const int32_t diameter = (radius * 2);

  int32_t x = (radius - 1);
  int32_t y = 0;
  int32_t tx = 1;
  int32_t ty = 1;
  int32_t error = (tx - diameter);

  while (x >= y)
  {
    // Each of the following renders an octant of the circle
    points[drawCount + 0] = { center.x + x, center.y - y };
    points[drawCount + 1] = { center.x + x, center.y + y };
    points[drawCount + 2] = { center.x - x, center.y - y };
    points[drawCount + 3] = { center.x - x, center.y + y };
    points[drawCount + 4] = { center.x + y, center.y - x };
    points[drawCount + 5] = { center.x + y, center.y + x };
    points[drawCount + 6] = { center.x - y, center.y - x };
    points[drawCount + 7] = { center.x - y, center.y + x };

    drawCount += 8;

    if (error <= 0)
    {
      ++y;
      error += ty;
      ty += 2;
    }

    if (error > 0)
    {
      --x;
      tx += 2;
      error += (tx - diameter);
    }
  }

  SDL_RenderPoints(this->gRenderer, points, drawCount);
  delete[] points;
}