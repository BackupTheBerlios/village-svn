#include <stdlib.h>
#include <SDL_image.h>
#include "menubar.h"
#include "textrender.h"
#include "config.h"

menubar_t * menubar_new() {
  menubar_t *mb = malloc(sizeof(menubar_t));

  SDL_Surface *bg = IMG_Load(DATADIR "/menubar.png");
  mb->bg = SDL_CreateRGBSurface(SDL_HWSURFACE, bg->w, bg->h, 24, 0, 0, 0, 0);
  SDL_BlitSurface(bg, NULL, mb->bg, NULL);
  SDL_FreeSurface(bg);

  mb->font = library_open("miscfixed");

  SDL_Surface *title = text_render(PACKAGE_STRING, mb->font);
  SDL_Rect dst;
  dst.x = (mb->bg->w - title->w)/2;
  dst.y = 4;
  SDL_BlitSurface(title, NULL, mb->bg, &dst);
  SDL_FreeSurface(title);

  mb->bs = buildsystem_new();

  int i;
  SDL_Rect rect;
  rect.x = 6;
  rect.y = 32;
  rect.w = 90;
  for (i=0;i<mb->bs->b_images->items_count;i++) {
    SDL_Surface *image = mb->bs->b_images->items[i].image;
    char *name = mb->bs->b_images->items[i].name;
    rect.h = image->h + 14;

    SDL_FillRect(mb->bg, &rect, SDL_MapRGB(mb->bg->format, 109,18,18));

    SDL_Surface *title = text_render(name, mb->font);    
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
  buildsystem_free(menubar->bs);
  library_free(menubar->font);
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
