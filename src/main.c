#include "definition.h"
#include "instances.h"
#include "scene.h"
#include "sdl-interface.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

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

  Camera camera;

  {
    Mat *pos = createMat(3, 0, false);
    pos->data[0][0] = .5;
    pos->data[1][0] = 0;
    pos->data[2][0] = -5;

    camera.pos = pos;
  }

  setCameraProp(&camera);

  Scene *scene = createScene();

  sceneAddCamera(scene, &camera);

  Shape *cube = initShapeCube();
  sceneAddModel(scene, cube);

  Instance cube1;
  cube1.model = cube;
  cube1.s = (float3d){1, 1, 1};
  cube1.t = (float3d){0, 4, 10};
  cube1.r = (float3d){0, 0, 0};
  cube1.pv = (float3d){0, 0, 0};
  calculateInstanceBoundingSphere(&cube1);
  sceneAddInstance(scene, &cube1);

  Instance cube2;
  cube2.model = cube;
  cube2.s = (float3d){1, 10, 5};
  cube2.t = (float3d){0, 0, 15};
  cube2.r = (float3d){0, 0, 40};
  cube2.pv = (float3d){0, 0, 0};
  calculateInstanceBoundingSphere(&cube2);
  sceneAddInstance(scene, &cube2);

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
          camera.pos->data[2][0] += 1e-1;
        if (kbdState[SDL_SCANCODE_S])
          camera.pos->data[2][0] -= 1e-1;
        if (kbdState[SDL_SCANCODE_D])
          camera.pos->data[0][0] += 1e-1;
        if (kbdState[SDL_SCANCODE_A])
          camera.pos->data[0][0] -= 1e-1;
        if (kbdState[SDL_SCANCODE_E])
          camera.pos->data[1][0] -= 1e-1;
        if (kbdState[SDL_SCANCODE_SPACE])
          camera.pos->data[1][0] += 1e-1;

        while (SDL_PollEvent(&event) != 0) {
          switch (event.type) {
          case SDL_QUIT:
            quit = true;
            break;
          }
        }
      }

      clearCanva();

      cube1.r.x += 1;
      cube1.r.y -= 1;
      cube1.r.z -= 2;

      setCameraProp(&camera);

      renderScene(scene);

      SDL_RenderPresent(renderer);

      if (quit)
        break;
    }
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  freeModel(cube);

  return EXIT_SUCCESS;
}
