#include <stdlib.h>
#include <stdio.h>
#include <SDL_image.h>
#include "library.h"

resources_t resources;

library_t * library_open(const char *name) {

  /* otwieram potrzebne pliki */
  char filename_ini[256];
  sprintf(filename_ini, "%s/%s.ini", DATADIR, name);
  FILE *fini = fopen(filename_ini, "r");
  if (fini == NULL) return NULL;

  char filename_png[256]; 
  sprintf(filename_png, "%s/%s.png", DATADIR, name);
  SDL_Surface *fpng = IMG_Load(filename_png);
  if (fpng == NULL) {
    fclose(fini);
    return NULL;
  }

  /* wczytuje elementy z biblioteki */
  library_t *l = malloc(sizeof(library_t));
  memset(l, 0, sizeof(library_t));
  while (!feof(fini)) {
    char name[32];
    SDL_Rect coords;
    fscanf(fini, "%31s = %hd,%hd,%hd,%hd\n", l->items[l->items_count].name, 
	   &coords.x, &coords.y, &coords.w, &coords.h);

    l->items[l->items_count].image = 
      SDL_CreateRGBSurface(SDL_HWSURFACE, coords.w, coords.h, 24, 0, 0, 0, 0);

    SDL_BlitSurface(fpng, &coords, l->items[l->items_count].image, NULL);

    SDL_SetColorKey(l->items[l->items_count].image, SDL_SRCCOLORKEY, 
		    SDL_MapRGB(l->items[l->items_count].image->format, 255, 0, 255));
    
    l->items_count ++;
    if (l->items_count == 256) break;
    
  }

  /* zwalniam zasoby i wracam */
  SDL_FreeSurface(fpng);
  fclose(fini);
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

void resources_init() {
  resources.flora = library_open("flora");
  resources.humans = library_open("humans");
  resources.buildings = library_open("buildings");
  resources.cursor = library_open("cursor");
  resources.font = library_open("font");
}

void resources_free() {
  library_free(resources.flora);
  library_free(resources.humans);
  library_free(resources.buildings);
  library_free(resources.cursor);
  library_free(resources.font);
}
