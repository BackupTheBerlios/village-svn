#include <stdlib.h>
#include "library.h"
#include "menubar.h"
#include "textrender.h"
#include "config.h"

menubar_t * menubar_new() {
  menubar_t *mb = malloc(sizeof(menubar_t));

  SDL_Surface *background = resources.menubar->items[0].image;
  mb->bg = SDL_CreateRGBSurface(SDL_HWSURFACE, 100, 480, 8, 0,0,0,0);
  SDL_SetColors(mb->bg, resources.colors, 0, 256);
  SDL_BlitSurface(background, NULL, mb->bg, NULL);

  SDL_Surface *title = text_render(PACKAGE_STRING, resources.font);
  SDL_Rect dst;
  dst.x = (mb->bg->w - title->w)/2;
  dst.y = 4;
  SDL_BlitSurface(title, NULL, mb->bg, &dst);
  SDL_FreeSurface(title);

  int i;
  SDL_Rect rect;
  rect.x = 6;
  rect.y = 32;
  rect.w = 90;
  for (i=0;i<resources.buildings->items_count;i++) {
    SDL_Surface *image = resources.buildings->items[i].image;
    char *name = resources.buildings->items[i].name;
    rect.h = image->h + 14;

    SDL_FillRect(mb->bg, &rect, SDL_MapRGB(mb->bg->format, 109,18,18));

    SDL_Surface *title = text_render(name, resources.font);    
    SDL_Rect dst;
    dst.x = rect.x + (rect.w - title->w) / 2;;
    dst.y = rect.y + image->h + 1;
    SDL_BlitSurface(title, NULL, mb->bg, &dst);
    SDL_FreeSurface(title);

    dst.x = rect.x + (rect.w - image->w) / 2;
    dst.y = rect.y;
    SDL_BlitSurface(image, NULL, mb->bg, &dst);

    rect.y += rect.h + 4;
  }

  return mb;
}

void menubar_free(menubar_t *menubar) {
  SDL_FreeSurface(menubar->bg);
  free(menubar);
}

void menubar_draw(menubar_t *menubar, screen_t *screen) {
  SDL_Rect dst; 
  dst.x = screen->video->w - 100;
  dst.y = 0;
  dst.w = 100;
  dst.h = 480;
  
  SDL_BlitSurface(menubar->bg, NULL, screen->video, &dst);
}
