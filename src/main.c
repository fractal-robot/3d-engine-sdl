#include "camera.h"
#include "definition.h"
#include "instances.h"
#include "matrix.h"
#include "models.h"
#include "sdl-interface.h"
#include "transform.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
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

  // render the scene

  clearCanva(renderer);

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
      instance.t = (float3d){-2, 0, 0},
      instance.r = (float3d){0, 0, 0},
  };

  renderInstance(renderer, &instance, &camera);

  while (1) {
    SDL_RenderPresent(renderer);
    if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
      break;
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}
