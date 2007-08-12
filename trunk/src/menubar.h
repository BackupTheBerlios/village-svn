#ifndef _MENUBAR_H_
#define _MENUBAR_H_

#include <SDL.h>
#include "screen.h"
#include "library.h"
#include "buildings.h"

typedef struct menubar_s {
  SDL_Surface *bg;
  library_t *font;
  buildsystem_t *bs;
} menubar_t;

menubar_t * menubar_new();
void menubar_draw(menubar_t *menubar, screen_t *screen);
void menubar_free(menubar_t *menubar);

#endif
