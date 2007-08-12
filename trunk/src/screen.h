#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <SDL.h>
#include "library.h"

struct screen_s;
typedef void (draw_screen_fn) (void *data, struct screen_s *screen, int px, int py, int kx, int ky);

typedef enum {
  CURSOR_ARROW,
  CURSOR_UP,
  CURSOR_RIGHT,
  CURSOR_DOWN,
  CURSOR_LEFT,
} cursor_t;

typedef struct screen_s {
  SDL_Surface *video;
  cursor_t current_cursor;
  int mouse_x;
  int mouse_y;
  draw_screen_fn *draw;
  void *draw_data;
} screen_t;

screen_t * screen_new();
void screen_free(screen_t *screen);
void screen_draw_cursor(screen_t * screen);

#endif
