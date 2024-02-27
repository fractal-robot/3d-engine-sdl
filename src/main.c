#include "definition.h"
#include "helper.h"
#include "matrix.h"
#include "sdl-interface.h"
#include "structs.h"
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

  Color blue = BLUE;

  Mat *p1 = createMat(1, 2, false);
  p1->data[0][0] = 20;
  p1->data[0][1] = -20;

  Mat *p2 = createMat(1, 2, false);
  p2->data[0][0] = -60;
  p2->data[0][1] = 30;

  Mat *p3 = createMat(1, 2, false);
  p3->data[0][0] = 20;
  p3->data[0][1] = 50;

  drawShadeTriangle(renderer, p2, p1, p3, 0.0, 0.2, 1, &blue);

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
