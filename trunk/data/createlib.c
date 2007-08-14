#include <stdlib.h>
#include <stdio.h>
#include <SDL_image.h>
#include "library.h"

library_t * library_ini_open(const char *name) {

  /* otwieram potrzebne pliki */
  char filename_ini[256];
  sprintf(filename_ini, "%s.ini", name);
  FILE *fini = fopen(filename_ini, "r");
  if (fini == NULL) return NULL;

  char filename_png[256]; 
  sprintf(filename_png, "%s.png", name);
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
    memset(name, 0, 32);
    SDL_Rect coords;
    fscanf(fini, "%31s = %hd,%hd,%hd,%hd\n", l->items[l->items_count].name, 
	   &coords.x, &coords.y, &coords.w, &coords.h);

    /* tworze nowa powierzchnie */
    SDL_Surface *image = SDL_CreateRGBSurface(SDL_HWSURFACE, coords.w, coords.h, 8, 0, 0, 0, 0); 
    SDL_LockSurface(image);
    SDL_SetColors(image, fpng->format->palette->colors, 0, fpng->format->palette->ncolors);
    image->format->palette->ncolors = fpng->format->palette->ncolors;
    SDL_UnlockSurface(image);
    SDL_BlitSurface(fpng, &coords, image, NULL);

    /* przypisuje nowo utworzona powierzchnie */
    l->items[l->items_count].image = image;   
    l->items_count ++;
    if (l->items_count == 256) break;    
  }

  /* zwalniam zasoby i wracam */
  SDL_FreeSurface(fpng);
  fclose(fini);
  return l;
}

int main(int argc, char **argv) {

  SDL_Init(SDL_INIT_VIDEO);

  if (argc == 4) {
    library_t * lib = library_ini_open(argv[2]);
    library_save(lib, argv[3]);
    library_free(lib);
  }

  SDL_Quit();
  return 0;
}
