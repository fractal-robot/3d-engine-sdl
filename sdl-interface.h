// generate an interface to SDL by a clearCava and setPixel method, that draws
// individual pixels to a canva of center (0, 0)

#ifndef SDL_INTERFACE_H
#define SDL_INTERFACE_H

#include "structs.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#define WINDOW_WIDTH 1024 // should be a power of 2
#define SCALE_FACTOR 1    // should be a power of 2
#define RENDER_WIDTH (WINDOW_WIDTH / SCALE_FACTOR)

void setPixel(SDL_Renderer *renderer, Point2d coord, const Color *color);
void clearCanva(SDL_Renderer *renderer);

#endif
