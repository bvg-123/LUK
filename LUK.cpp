#include <windows.h>
#include <iostream>
#include <iomanip>
#include <io.h>
#include <fcntl.h>
#include <math.h>
#include "Config.h"
#include "TopDoor.h"
#include "BottomDoor.h"
#include "math/Force.h"
#include "string"

#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "Model.h"
#include "ModelRenderer.h"

/*
// image-comment: [.codeimages/img-BottomDoorAngle.png]
*/
bool getBottomDoorAngle(float x1, float y1, float R1, float x2, float y2, float R2, float* bottomDoorAngle, float* x, float* y) {
  float z = (pow(R2, 2)-pow(R1, 2)+pow(x1, 2)-pow(x2, 2)+pow(y1, 2)-pow(y2, 2))/2.0/(x1-x2);
  float a = 1.0+pow((y2-y1)/(x1-x2), 2);
  float b = 2*((z-x1)*(y2-y1)/(x1-x2)-y1);
  float c = pow(z-x1, 2)+pow(y1, 2)-pow(R1, 2);
  float D = pow(b, 2)-4*a*c;
  if (D < 0) {
    *y = 0;
    *x = 0;
    *bottomDoorAngle = 0;
    return false;
  }
  *y = min((-b+sqrt(D))/2/a, (-b-sqrt(D))/2/a);  // берем нижнюю точку пересечения окружностей как физический вариант.
  *x = *y*(y2-y1)/(x1-x2)+z;
  if (*y < y1) {
    *y = 0;
    *x = 0;
    *bottomDoorAngle = 0;
    return false;
  }
  *bottomDoorAngle = atan2(*y-y1, *x-x1);
  return true;
}

constexpr int windowWidth{ 900 };
constexpr int windowHeight{ 900 };
constexpr float modelScale = 400;



void initConsole() {
  SetConsoleCP(CP_UTF8);
  SetConsoleOutputCP(CP_UTF8);
  if (_setmode(_fileno(stdout), _O_U8TEXT) == -1) {
    std::wcout << "ASSERT: stdout utf8 mode failed";
  }
  if (_setmode(_fileno(stdin), _O_U8TEXT) == -1) {
    std::wcout << "ASSERT: stdin utf8 mode failed";
  }
  if (_setmode(_fileno(stderr), _O_U8TEXT) == -1) {
    std::wcout << "ASSERT: stderr utf8 mode failed";
  }
}

int main()
{
  initConsole();

  wprintf(L"Расчет моментов.\n");
  wprintf(L"Количество амортизаторов: %d\n", AMMO_COUNT);
  wprintf(L"Усилие амортизатора: %.0f Н\n", AMMO_FORCE);
  wprintf(L"Расстояние по вертикали от неподвижной оси амортизатора до оси верхней двери: %.1f см\n", AMMO_AXIS_VERTICAL_OFFSET*100);
  wprintf(L"Расстояние по горизонтали от неподвижной оси амортизатора до оси верхней двери: %.1f см\n", AMMO_AXIS_HORIZONTAL_OFFSET*100);

  Model* model = new Model();
  std::wcout << L"Угол верхней двери";
  std::wcout << ", ";
  std::wcout << L"Угол нижней двери";
  std::wcout << ", ";
  std::wcout << L"Длина амортизатора";
  std::wcout << ", ";
  std::wcout << L"Угол амортизатора";
  std::wcout << ", ";
  std::wcout << L"Момент нижней двери";
  std::wcout << ", ";
  std::wcout << L"Сила на ручке нижней двери";
  std::wcout << ", ";
  std::wcout << L"Угол силы на ручке нижней двери";
  std::wcout << ", ";
  std::wcout << L"Сила на ручке верхней двери";
  std::wcout << ", ";
  std::wcout << L"Угол силы на ручке верхней двери";
  std::wcout << ", ";
  std::wcout << std::endl;

  float angleStep = 1 * PI / 180;
  float angle = 0;
  while(model->setTopDoorAngle(angle)) {
    // Угол верхней двери:
    std::wcout << model->topDoor->printAngle() << ", ";
    // Угол нижней двери:
    std::wcout << (model->isCordTense ? model->bottomDoor->printAngle() : std::wstring(L"-")) << ", ";
    // Длина амортизатора:
    std::wcout << std::setprecision(2) << std::fixed << model->ammoLength << L" м, ";
    // Угол амортизатора:
    std::wcout << std::setprecision(0) << std::fixed << model->ammoAngle*180/PI << L"°, ";
    // Момент нижней двери:
    std::wcout << (model->isCordTense ? model->bottomDoor->printMoment() : std::wstring(L"-")) << ", ";
    // Сила для компенсации момента на ручке нижней двери:
    std::wcout << (model->isCordTense ? model->bottomDoorGripForce->print() : std::wstring(L"-, -")) << ", ";
    // Сила для компенсации момента на ручке верхней двери:
    std::wcout << model->topDoorGripForce->print();
    std::wcout << std::endl;
    angle += angleStep;
  }

  ModelRenderer* modelRenderer = new ModelRenderer(model, windowWidth, windowHeight, modelScale);
  bool quit = false;
  SDL_Event e;
  angle = 0;
  model->setTopDoorAngle(angle);
  while (!quit)
  {
    while (SDL_PollEvent(&e) != 0)
    {
      if (e.type == SDL_EVENT_KEY_DOWN) {
        if (e.key.key == SDLK_UP || e.key.key == SDLK_LEFT) {
          angle = angle + angleStep;
          model->setTopDoorAngle(angle);
        } else if (e.key.key == SDLK_DOWN || e.key.key == SDLK_RIGHT) {
          angle = angle - angleStep;
          model->setTopDoorAngle(angle);
        }
      } else if (e.type == SDL_EVENT_QUIT) {
        quit = true;
      }
    }
    modelRenderer->Execute();
  }

  return 0;
}
