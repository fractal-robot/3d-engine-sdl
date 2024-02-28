// generate an interface to SDL by a clearCava and setPixel method, that draws
// individual pixels to a canva of center (0, 0)

#include "sdl-interface.h"
#include "definition.h"
#include "matrix.h"
#include "structs.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <assert.h>

#if (SCALE_FACTOR & (SCALE_FACTOR - 1)) != 0
#error "[ERROR] SCALE_FACTOR must be a power of 2.\n"
#endif
#if (WINDOW_WIDTH & (WINDOW_WIDTH - 1)) != 0
#error "[ERROR] WINDOW_WIDTH must be a power of 2.\n"
#endif

void setPixel(SDL_Renderer *renderer, const Mat *coord, const Color *color) {
  // (0, 0): center of the canva
  // cartesian coordinates

  assert(coord->rows == 2 && coord->cols == 1);

  const int pX = ((int)(RENDER_WIDTH / 2) + coord->data[0][0]) * SCALE_FACTOR;
  const int pY = ((int)(RENDER_WIDTH / 2) - coord->data[1][0]) * SCALE_FACTOR;

  const int scaleOffset = SCALE_FACTOR / 2; // / 2 because we want (0,0) to be
                                            // centered on the screen

  SDL_SetRenderDrawColor(renderer, color->red, color->green, color->blue, 1);
  for (int x = -scaleOffset; x <= scaleOffset; ++x)
    for (int y = -scaleOffset; y <= scaleOffset; ++y)
      SDL_RenderDrawPoint(renderer, pX + x, pY + y);
}

void clearCanva(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
}
