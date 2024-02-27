// generate an interface to SDL by a clearCava and setPixel method, that draws
// individual pixels to a canva of center (0, 0)

#ifndef SDL_INTERFACE_H
#define SDL_INTERFACE_H

#include "matrix.h"
#include "structs.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

void setPixel(SDL_Renderer *renderer, const Mat *coord, const Color *color);
void clearCanva(SDL_Renderer *renderer);

#endif
