#include "camera.h"
#include "definition.h"
#include "instances.h"
#include "matrix.h"
#include "scene.h"
#include "sdl-interface.h"
#include "shapes.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

SDL_Renderer *renderer;
SDL_Event event;
SDL_Window *window;

int main() {
  printf("[DEBUG] Render width: %i.\n", RENDER_WIDTH);

  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_WIDTH, 0, &window,
                              &renderer);
  SDL_SetWindowTitle(window, "window");
  const Uint8 *kbdState = SDL_GetKeyboardState(NULL);

  Camera *camera = malloc(sizeof(Camera));
  initCamera(camera);

  {
    Mat *pos = createMat(3, 1, false);
    setX(pos, .5);
    setY(pos, 0);
    setZ(pos, -5);
    camera->pos = pos;
  }

  Shape *cube = initShapeCube();

  Instance *cube1 = initInstance(cube);
  cube1->s = (float3d){1, 1, 1};
  cube1->t = (float3d){-4, 4, 20};
  cube1->r = (float3d){0, 0, 0};
  cube1->updateInstance = true;

  Instance *cube2 = initInstance(cube);
  cube2->s = (float3d){1, 10, 5};
  cube2->t = (float3d){0, 0, 50};
  cube2->r = (float3d){0, 0, 40};
  cube2->updateInstance = true;

  clearCanva();

  float3d rotation = {25, 25, 25};

  SDL_RenderPresent(renderer);

  double delta = 0;
  int a = 0, b = 0;
  _Bool quit = false;

  while (1) {
    a = SDL_GetTicks();
    delta = a - b;

    if (delta > 1000 / 60.0) {
      b = a;
      {
        SDL_PumpEvents();

        if (kbdState[SDL_SCANCODE_W])
          camera->pos->data[2][0] += 1e-1;
        if (kbdState[SDL_SCANCODE_S])
          camera->pos->data[2][0] -= 1e-1;
        if (kbdState[SDL_SCANCODE_D])
          camera->pos->data[0][0] += 1e-1;
        if (kbdState[SDL_SCANCODE_A])
          camera->pos->data[0][0] -= 1e-1;
        if (kbdState[SDL_SCANCODE_E])
          camera->pos->data[1][0] -= 1e-1;
        if (kbdState[SDL_SCANCODE_SPACE])
          camera->pos->data[1][0] += 1e-1;

        while (SDL_PollEvent(&event) != 0) {
          switch (event.type) {
          case SDL_QUIT:
            quit = true;
            break;
          }
        }
      }

      clearCanva();

      setCameraProp(camera);

      rotation.x += 20;
      setInstanceTransform(ROTATION, rotation, cube1);

      updateInstance(cube1);
      updateInstance(cube2);

      renderWireframeInstance(cube1, camera);
      renderWireframeInstance(cube2, camera);

      SDL_RenderPresent(renderer);
      if (quit)
        break;
    }
  }

  destroyInstance(cube1);
  destroyInstance(cube2);

  freeShape(cube);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
