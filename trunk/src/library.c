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
    int w = fgetc(libfile);
    int h = fgetc(libfile);
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
    fread(item->image->pixels, 1, w*h, libfile);
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

int library_save(library_t *library, const char *filename) {
  int i, c;
  FILE *file = fopen(filename, "w");
  if (file == NULL) return -1;

  fwrite(LIBGFX_SIGN, 1, 10, file);
  fputc(library->items_count, file);

  for (i=0;i<library->items_count;i++) {

    libitem_t *item = &library->items[i];
    fwrite(item->name, 1, 32, file);
    fputc(item->image->w, file);
    fputc(item->image->h, file);
    fputc(item->image->format->palette->ncolors, file);

    for (c=0;c<item->image->format->palette->ncolors;c++) {
      SDL_Color *color = &item->image->format->palette->colors[c];
      fputc(color->r, file);
      fputc(color->g, file);
      fputc(color->b, file);
    }

    fwrite(item->image->pixels, 1, item->image->w * item->image->h, file);
  }

  fclose(file);
  return 0;
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
    }
  }
}

void sync_colors(library_t *l) {
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

  sync_colors(resources.flora);
  sync_colors(resources.humans);
  sync_colors(resources.buildings);
  sync_colors(resources.cursor);
  sync_colors(resources.font);
  sync_colors(resources.background);
  sync_colors(resources.menubar);
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
