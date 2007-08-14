#ifndef _LIBRARY_H_
#define _LIBRARY_H_
#include <SDL.h>

#define LIBGFX_SIGN "LIBFILGFX"

typedef struct libitem_s {
  char          name[32];
  SDL_Surface * image;
} libitem_t;

typedef struct library_s {
  libitem_t     items[256];
  int           items_count;
} library_t;

typedef struct resources_s {
  library_t *flora;
  library_t *humans;
  library_t *buildings;
  library_t *cursor;
  library_t *font;
  library_t *background;
  library_t *menubar;
  SDL_Color colors[256];
} resources_t;

extern resources_t resources;

library_t   * library_open(const char *name);
SDL_Surface * library_find(library_t *library, const char *name);
int           library_save(library_t *library, const char *filename);
void          library_free(library_t *library);
void          resources_init();
void          resources_free();

#endif
