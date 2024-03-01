// generate an interface to SDL by a clearCava and setPixel method, that draws
// individual pixels to a canva of center (0, 0)

#ifndef SDL_INTERFACE_H
#define SDL_INTERFACE_H

#include "matrix.h"
#include "structs.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

extern SDL_Renderer *renderer;
extern SDL_Event event;
extern SDL_Window *window;

void setPixel(const Mat *coord, const Color *color);
void clearCanva();

#endif
