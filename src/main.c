#include "camera.h"
#include "definition.h"
#include "instances.h"
#include "matrix.h"
#include "models.h"
#include "sdl-interface.h"
#include "transform.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  SDL_Renderer *renderer;
  printf("Render width: %d\n", RENDER_WIDTH);

  // initialize SDL

  SDL_Event event;
  SDL_Window *window;
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_WIDTH, 0, &window,
                              &renderer);
  //  clearCanva(renderer);
  SDL_SetWindowTitle(window, "window");

  const Uint8 *kbdState = SDL_GetKeyboardState(NULL);

  // render the scene

  Camera camera;

  {
    Mat *pos = createMat(3, 0, false);
    pos->data[0][0] = .5;
    pos->data[1][0] = 0;
    pos->data[2][0] = -5;

    camera.pos = pos;
  }

  setCameraProp(&camera);

  Model *model = createModelCube();
  Instance instance = {
      instance.model = model,
      instance.s = (float3d){1, 1, 1},
      instance.t = (float3d){0, 0, 4},
      instance.r = (float3d){0, 0, 0},
  };

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

        while (SDL_PollEvent(&event) != 0) {
          switch (event.type) {
          case SDL_QUIT:
            quit = true;
            break;
          }
        }
      }

      clearCanva(renderer);

      instance.r.x += 1;
      instance.r.y -= 1;
      instance.r.z -= 2;

      renderInstance(renderer, &instance, &camera);
      setCameraProp(&camera);

      SDL_RenderPresent(renderer);

      if (quit)
        break;
    }
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}
