#pragma once
#include <SDL3/SDL.h>
#include "math/Point.h"

class BaseRenderer
{
  private:
    bool isSDLInitialized;

    SDL_Window* gWindow;
    SDL_Surface* gScreenSurface;
    SDL_Renderer* gRenderer;
    float scale;
  protected:
    Point center;
    SDL_Color background;
    SDL_Color foreground;

    void initSDL();
    void doneSDL();

    void createWindow(int width, int height, float scale = 1, SDL_Color background = { 0, 0, 0, 0 });
    void closeWindow();
    Point scaleToScreen(Point* point);

    virtual void draw();
    void clear();
    void drawLine(Point* point1, Point* point2);
    void drawLineScaled(Point* scaledPoint1, Point* scaledPoint2);
    void drawText(Point& point, const char* const text, ...);
    void drawTextScaled(Point& scaledPoint, const char* const format, ...);
    void vdrawTextScaled(Point& scaledPoint, const char* const format, va_list);
    void DrawCircleScaled(Point center, int radius);
  public:
  BaseRenderer();
  ~BaseRenderer();

  void Execute();

};

