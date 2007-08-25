#include <stdlib.h>
#include <stdio.h>
#include "library.h"

resources_t resources;

library_t * library_open(const char *name) {
  FILE *libfile = fopen(name, "r");
  if (libfile == NULL) {
    printf("can't open %s\n", name);
    return NULL;
  }

  char sign[10];
  fread(sign, 1, 10, libfile);
  if (strcmp(sign, LIBGFX_SIGN)) {
    printf("file %s have bad signature\n", name);
    fclose(libfile);
    return 0;
  }
  
  library_t *l = malloc(sizeof(library_t));  
  l->items_count = fgetc(libfile);

  int i,c;
  for (i=0;i<l->items_count;i++) {
    libitem_t *item = &l->items[i];
    fread(item->name, 1, 32, libfile);

    int w=0, h=0;
    fread(&w, 1, 2, libfile);
    fread(&h, 1, 2, libfile);
    int nc = fgetc(libfile);
    SDL_Color colors[256];
    for (c=0;c<nc;c++) {
      colors[c].r = fgetc(libfile);
      colors[c].g = fgetc(libfile);
      colors[c].b = fgetc(libfile);
    }

    item->image = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 8, 0, 0, 0, 0);
    SDL_LockSurface(item->image);
    SDL_SetColors(item->image, colors, 0, nc);
    item->image->format->palette->ncolors = nc;
    int lines;
    for (lines=0;lines<h;lines++)
      fread(item->image->pixels + lines * item->image->pitch, 1, item->image->w, libfile);
    SDL_UnlockSurface(item->image);

    /* jesli jest odpowiedni kolor to ustawiam przezroczystosc */
    int c;
    for (c=0;c<item->image->format->palette->ncolors;c++) {
      SDL_Color *color = &item->image->format->palette->colors[c];
      if (color->r == 255 && color->g == 0 && color->b == 255) {
	SDL_SetColorKey(item->image, SDL_SRCCOLORKEY, c);
	break;
      }
    }
  }

  fclose(libfile);
  return l;
}

SDL_Surface *library_find(library_t *library, const char *name) {
  int i=0;

  for (i=0;i<library->items_count;i++) {  
    if (!strcmp(library->items[i].name, name))
      return library->items[i].image;
  }
  return NULL;
}

void library_free(library_t *library) {
  int i;
  for (i=0;i<library->items_count;i++) {
    SDL_FreeSurface(library->items[i].image);
  }
  free(library);
}

///////////////////////////////////////////////////////////////////////////

void append_color(int r, int g, int b) {
  if (r==0 && g==0 && b==0) return;
  int c;
  for (c=1;c<256;c++) {
    if (resources.colors[c].r == r &&
	resources.colors[c].g == g &&
	resources.colors[c].b == b) 
      return;
    if (resources.colors[c].r == 0 &&
	resources.colors[c].g == 0 &&
	resources.colors[c].b == 0) {
      resources.colors[c].r = r;
      resources.colors[c].g = g;
      resources.colors[c].b = b;
      return;
    }
  }
}

void map_colors(library_t *l) {
  int i, c;
  for (i=0;i<l->items_count;i++) {
    SDL_Palette *pal = l->items[i].image->format->palette;
    for (c=0;c<pal->ncolors;c++) {
      append_color(pal->colors[c].r, pal->colors[c].g, pal->colors[c].b);
    }
  }
}

void resources_init() {
  resources.flora = library_open("flora.lfg");
  resources.humans = library_open("humans.lfg");
  resources.buildings = library_open("buildings.lfg");
  resources.cursor = library_open("cursor.lfg");
  resources.font = library_open("miscfixed.lfg");
  resources.background = library_open("background.lfg");
  resources.menubar = library_open("menubar.lfg");

  if (resources.flora == NULL ||
      resources.humans == NULL ||
      resources.buildings == NULL ||
      resources.cursor == NULL ||
      resources.font == NULL ||
      resources.background == NULL ||
      resources.menubar == NULL) 
    exit(-1);

  memset(&resources.colors, 0, 256 * sizeof(SDL_Color));

  map_colors(resources.flora);
  map_colors(resources.humans);
  map_colors(resources.buildings);
  map_colors(resources.cursor);
  map_colors(resources.font);
  map_colors(resources.background);
  map_colors(resources.menubar);

  int c;
  for (c=1;c<256;c++) {
    if (resources.colors[c].r == 0 &&
	resources.colors[c].g == 0 &&
	resources.colors[c].b == 0) {
      printf("loaded colors: %d\n", c+1);
      break;
    }
  }
}

void resources_free() {
  library_free(resources.flora);
  library_free(resources.humans);
  library_free(resources.buildings);
  library_free(resources.cursor);
  library_free(resources.font);
  library_free(resources.background);
  library_free(resources.menubar);
}
