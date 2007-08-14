#include <SDL_image.h>
#include "screen.h"
#include "library.h"

screen_t * screen_new() {
  //SDL_Surface *video = SDL_SetVideoMode(640,480,24,SDL_HWSURFACE | SDL_HWPALETTE | SDL_FULLSCREEN);
  SDL_Surface *video = SDL_SetVideoMode(640,480,24,SDL_HWSURFACE | SDL_HWPALETTE);
  if (video == NULL) return NULL;
  SDL_SetColors(video, resources.colors, 0, 256);

  SDL_ShowCursor(SDL_DISABLE);

  screen_t * screen = malloc(sizeof(screen_t));
  screen->current_cursor = CURSOR_ARROW;
  screen->video = video;

  SDL_GetMouseState(&screen->mouse_x, &screen->mouse_y);
  return screen;
}

void screen_draw_cursor(screen_t * screen) {
  SDL_Rect dst;
  dst.x = screen->mouse_x;
  dst.y = screen->mouse_y;

  switch (screen->current_cursor) {
  case CURSOR_LEFT: dst.x = 0; break;
  case CURSOR_RIGHT: dst.x = screen->video->w - 16; break;
  case CURSOR_UP: dst.y = 0; break;
  case CURSOR_DOWN: dst.y = screen->video->h - 16; break;
  }

  SDL_BlitSurface(resources.cursor->items[screen->current_cursor].image, NULL, screen->video, &dst);
}

void screen_free(screen_t *screen) {
  free(screen);
}
