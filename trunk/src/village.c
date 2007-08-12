#include <stdio.h>
#include <SDL.h>

#include "map.h"
#include "screen.h"
#include "library.h"
#include "textrender.h"
#include "menubar.h"
#include "humans.h"

map_t *map;
screen_t *screen;
menubar_t *menubar;

int main(int argc, char **argv) {

  /* inicjacja mapy */

  resources_init();
  screen = screen_new();  
  map = map_new(128, 128);
  map->camera_x = 500;
  map->camera_y = 500;
  screen->draw = (draw_screen_fn*)map_redraw;
  screen->draw_data = map;
  menubar = menubar_new();

  int i=0, t = HT_MAN;
  human_t *human[500];
  for (i=0;i<500;i++) {
#define rand(min,max) (min) +(int) (((float)max)*rand()/(RAND_MAX+((float)min)))
    
    int x = rand(0,128);
    int y = rand(0,128);

    human[i] = human_new(t);
    cell_append(&map->cells[y * map->width + x], (mapobject_t*) human[i]);
    if (t == HT_MAN) t = HT_WOMAN;
    else t = HT_MAN;
    human[i]->walk_step = rand(0,5);
  }
  
  /* main loop */

  Uint32 last_ticks = SDL_GetTicks();
  SDL_Event event;
  Uint8 *keys = SDL_GetKeyState(NULL);
  int loop = 1;

  int fps = 0;
  Uint32 last_fps_update = SDL_GetTicks();
  SDL_Surface *fps_label = NULL;

  while (loop) {
    screen->draw(screen->draw_data, screen, 0, 0, screen->video->w - 100, screen->video->h);
    menubar_draw(menubar, screen);

    fps ++;
    if (SDL_GetTicks() - last_fps_update > 999) {
      char fpstxt[255];
      sprintf(fpstxt, "%d fps", fps);
      fps = 0;
      last_fps_update = SDL_GetTicks();
      if (fps_label != NULL) SDL_FreeSurface(fps_label);
      fps_label = text_render(fpstxt, menubar->font);
    }
    if (fps_label != NULL) {
      SDL_Rect rect;
      rect.x = screen->video->w - fps_label->w - 3;
      rect.y = screen->video->h - fps_label->h - 3;
      SDL_BlitSurface(fps_label, NULL, screen->video, &rect);
    }

    screen_draw_cursor(screen);
    SDL_Flip(screen->video);
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_MOUSEMOTION: 
	{
	  screen->mouse_x = event.motion.x;
	  screen->mouse_y = event.motion.y;
	  if (screen->mouse_x < 2) screen->current_cursor = CURSOR_LEFT;
	  else if (screen->mouse_x > screen->video->w - 2)  screen->current_cursor = CURSOR_RIGHT;
	  else if (screen->mouse_y  < 2) screen->current_cursor = CURSOR_UP;
	  else if (screen->mouse_y > screen->video->h - 2) screen->current_cursor = CURSOR_DOWN;
	  else screen->current_cursor = CURSOR_ARROW;
	}
      case SDL_KEYDOWN: 
	{
	  if (event.key.keysym.sym == SDLK_ESCAPE) loop = 0;
	  break;
	}
      }
    }
   
    while (SDL_GetTicks() - last_ticks > 9) {
      last_ticks += 10;

      for (i=0;i<500;i++) 
	human_update(human[i]);

      if (keys[SDLK_LEFT] || screen->current_cursor == CURSOR_LEFT) {
	map->camera_x -= 4;
	map_fix_camera(map, screen->video->w, screen->video->h);
      }
      if (keys[SDLK_RIGHT] || screen->current_cursor == CURSOR_RIGHT) {
	map->camera_x += 4;
	map_fix_camera(map, screen->video->w, screen->video->h);
      }
      if (keys[SDLK_UP] || screen->current_cursor == CURSOR_UP) {
	map->camera_y -= 4;
	map_fix_camera(map, screen->video->w, screen->video->h);
      }
      if (keys[SDLK_DOWN] || screen->current_cursor == CURSOR_DOWN) {
	map->camera_y += 4;
	map_fix_camera(map, screen->video->w, screen->video->h);
      }
    }
  }

  resources_free();
  menubar_free(menubar);
  map_free(map);
  screen_free(screen); 
  return 0;

}
