// generate an interface to SDL by a clearCava and setPixel method, that draws
// individual pixels to a canva of center (0, 0)

#include "sdl-interface.h"
#include "definition.h"
#include "structs.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#if (SCALE_FACTOR & (SCALE_FACTOR - 1)) != 0
#error "SCALE_FACTOR must be a power of two."
#endif
#if (WINDOW_WIDTH & (WINDOW_WIDTH - 1)) != 0
#error "WINDOW_WIDTH must be a power of two."
#endif

void setPixel(SDL_Renderer *renderer, Mat2d coord, const Color *color) {
  // (0, 0) is the center of the screen
  // y is increasing to the top of the screen
  //  assert(coord.x < RENDER_WIDTH && coord.y < RENDER_WIDTH);

  coord.x = ((int)(RENDER_WIDTH / 2) + coord.x) * SCALE_FACTOR;
  coord.y = ((int)(RENDER_WIDTH / 2) - coord.y) * SCALE_FACTOR;
  const int scaleOffset = SCALE_FACTOR / 2;

  SDL_SetRenderDrawColor(renderer, color->red, color->green, color->blue, 1);
  for (int x = -scaleOffset; x <= scaleOffset; ++x) {
    for (int y = -scaleOffset; y <= scaleOffset; ++y) {
      SDL_RenderDrawPoint(renderer, coord.x + x, coord.y + y);
    }
  }
}

void clearCanva(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
}
