#ifndef _LIBRARY_H_
#define _LIBRARY_H_

#include <SDL.h>

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
} resources_t;

library_t   * library_open(const char *name);
SDL_Surface * library_find(library_t *library, const char *name);
void          library_free(library_t *library);
void          resources_init();
void          resources_free();

#endif
