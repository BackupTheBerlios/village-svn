#ifndef _TEXTRENDER_H_
#define _TEXTRENDER_H_

#include <SDL.h>
#include "library.h"

SDL_Surface* text_render(const char *text, library_t *chars);

#endif
