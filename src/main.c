#include "camera.h"
#include "definition.h"
#include "instances.h"
#include "matrix.h"
#include "renderer.h"
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

  Scene *scene = createScene();
  Shape *cube = initShapeCube();
  sceneAddModel(scene, cube);

  sceneAddCamera(scene, camera);

  Instance *cube1 = initInstance(cube);
  cube1->s = (float3d){3, 5, 8};
  cube1->t = (float3d){-4, 4, 20};
  cube1->r = (float3d){0, 5, 0};
  cube1->updateInstance = true;
  sceneAddInstance(scene, cube1);

  Instance *cube2 = initInstance(cube);
  cube2->s = (float3d){1, 10, 5};
  cube2->t = (float3d){0, 0, 50};
  cube2->r = (float3d){0, 0, 40};
  cube2->updateInstance = true;
  sceneAddInstance(scene, cube2);

  clearCanva();

  float3d rotation = {25, 25, 25};
  float3d scale = {1, 1, 1};

  SDL_RenderPresent(renderer);

  double delta = 0;
  int a = 0, b = 0;
  bool quit = false;

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

      rotation.x += 2;
      rotation.z += 1;
      scale.x += 1e-2;
      setInstanceTransform(ROTATE, rotation, cube1);
      setInstanceTransform(SCALE, scale, cube2);
      setInstanceTransform(TRANSLATE, scale, cube2);

      updateScene(scene);
      renderScene(scene);

      SDL_RenderPresent(renderer);
      if (quit)
        break;
    }
  }

  destroyInstance(cube1);
  destroyInstance(cube2);

  destroyShape(cube);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
