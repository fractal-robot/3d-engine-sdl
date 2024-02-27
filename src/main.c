#include "definition.h"
#include "instances.h"
#include "matrix.h"
#include "models.h"
#include "sdl-interface.h"
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

  Model *cube = createModelCube();

  Instance instance = {cube, (Mat3d){0, 0, 0}};
  renderInstance(renderer, &instance);

  FloatMatrix *m1 = createFloatMatrix(2, 2, true);
  FloatMatrix *m2 = createFloatMatrix(2, 2, true);

  setElement(m1, 0, 0, 1);
  setElement(m1, 0, 1, 2);
  setElement(m1, 1, 0, 3);
  setElement(m1, 1, 1, 4);

  setElement(m2, 0, 0, 5);
  setElement(m2, 0, 1, 6);
  setElement(m2, 1, 0, 7);
  setElement(m2, 1, 1, 8);

  printf("he %f\n", getElement(m1, 1, 1));

  printMatrix(m1);
  printMatrix(m2);

  FloatMatrix *m3 = multiplyMatrices(m1, m2);
  printMatrix(m3);

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
