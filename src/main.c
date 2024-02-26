#include "definition.h"
#include "instances.h"
#include "models.h"
#include "sdl-interface.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
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

  Model *cube = createModelCube();

  Instance instance = {cube, (Mat3d){0, 0, 0}};
  renderInstance(renderer, &instance);

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
