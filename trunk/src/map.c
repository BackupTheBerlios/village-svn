#include <math.h>
#include <stdlib.h>
#include "map.h"
#include "library.h"

#define rand(min,max) (min) +(int) (((float)max)*rand()/(RAND_MAX+((float)min)))

mapobject_t *mapobject_new(SDL_Surface *image, int type) {
  mapobject_t *obj = malloc(sizeof(mapobject_t));
  obj->image = image;
  obj->type = type;
  switch (type) {
  case MO_TREE: obj->def_flags = CF_NOALL; break;
  case MO_ROCK: obj->def_flags = CF_NOALL; break;
  case MO_BUSH: obj->def_flags = CF_NOBUILD; break;
  case MO_BUILD: obj->def_flags = CF_NOALL; break;
  case MO_MAN: obj->def_flags = CF_NOBUILD; break;
  case MO_OTHER: obj->def_flags = 0; break;
  }
  return obj;
}

void mapobject_free(mapobject_t *obj) {
  free(obj);
}

void cell_append(cell_t *cell, mapobject_t *obj) {
  if (cell->object == NULL) {
    cell->object = obj;
    obj->next = NULL;
    obj->parent = cell;
  } else {
    mapobject_t *cur = cell->object;
    while (cur->next != NULL)
      cur = cur->next;
    if (cur != NULL) {
      cur->next = obj;
      obj->next = NULL;
      obj->parent = cell;
    }
  }
}

void cell_remove(cell_t *cell, mapobject_t *obj) {
  if (cell->object == NULL) return;
  else if (cell->object == obj) {

    if (cell->object->next == NULL) cell->object = NULL;
    else cell->object = obj->next;
    obj->parent = NULL;

  } else {

    mapobject_t *cur = cell->object;
    while (cur->next != obj) cur = cur->next;

    if (cur->next == obj) {
      cur->next = cur->next->next;
      obj->parent = NULL;
    };
  }
}

map_t * map_new(int width, int height) {
  map_t *map = malloc(sizeof(map_t));
  map->background = resources.background->items[0].image;
  map->width = width;
  map->height = height;
  map->cells = malloc(width * height * sizeof(cell_t));
  memset(map->cells, 0, width * height * sizeof(cell_t));
  int x,y;
  for (x=0; x < map->width; x++)
    for (y=0; y < map->height; y++) {
      cell_t *cell = &map->cells[y * map->width + x];
      cell->x = x;
      cell->y = y;
    }
 
  /* losuje drzewa */
  int i=0;
  for (i=0; i < (map->width * map->height) / 10; i++) {
  randposition:;
    int x = rand(0, map->width - 1);
    int y = rand(0, map->height - 1);
    cell_t *cell = &map->cells[x*map->width + y];
    if (cell->object == NULL) {
      SDL_Surface *img = resources.flora->items[rand(0,3)].image;
      mapobject_t *object = mapobject_new(img, MO_TREE);
      cell_append(cell, object);
      cell->flags = cell->object->def_flags;
    } else goto randposition;
    
  }

  for (i=0; i < (map->width * map->height) / 10; i++) {
  randposition2:;
    int x = rand(0, map->width - 1);
    int y = rand(0, map->height - 1);
    cell_t *cell = &map->cells[y*map->width + x];
    if (cell->object == NULL) {
      int r = rand(3,8);
      SDL_Surface *img = resources.flora->items[r].image;
      mapobject_t *object;
      if (r == 3) object = mapobject_new(img, MO_ROCK);
      if (r == 4) object = mapobject_new(img, MO_ROCK);
      if (r == 5) object = mapobject_new(img, MO_BUSH);
      if (r == 6) object = mapobject_new(img, MO_BUSH);
      if (r == 7) object = mapobject_new(img, MO_BUSH);
      if (r == 8) object = mapobject_new(img, MO_BUSH);
      if (r == 9) object = mapobject_new(img, MO_BUSH);
      if (r == 10) object = mapobject_new(img, MO_OTHER);
      cell_append(cell, object);
      cell->flags = cell->object->def_flags;

    } else goto randposition2;
    
  }

  return map;
}

void map_free(map_t *map) {
  int i;
  for (i=0; i < map->width * map->height; i++) {  
    if (map->cells[i].object != NULL) {
      mapobject_free(map->cells[i].object);
    }
  }
  free(map->cells);
  free(map);
}

void map_fix_camera(map_t *map, int viewport_w, int viewport_h) {
  if (map->camera_x < 0) map->camera_x = 0;
  if (map->camera_y < 0) map->camera_y = 0;
  if (map->camera_x > map->width * CELL_SIZE - viewport_w) map->camera_x = map->width * CELL_SIZE - viewport_w;
  if (map->camera_y > map->height * CELL_SIZE - viewport_h) map->camera_y = map->height * CELL_SIZE - viewport_h;
}

void map_redraw(map_t *map, screen_t *screen, int px, int py, int kx, int ky) {
  SDL_Rect srcr, dstr;

  /* rysuje tlo mapy */
  px += map->camera_x;
  py += map->camera_y;
  kx += map->camera_x;
  ky += map->camera_y;
  int x, y;
  for (x = px; x < kx; x += dstr.w) 
    for (y = py; y < ky; y += dstr.h) {

      srcr.x = x - floor((double)x / map->background->w) * map->background->w;
      srcr.y = y - floor((double)y / map->background->h) * map->background->h;
      srcr.w = (kx - x < map->background->w ? kx - x : map->background->w);
      srcr.h = (ky - y < map->background->h ? ky - y : map->background->h);
      
      dstr.x = x - map->camera_x;
      dstr.y = y - map->camera_y;
      dstr.w = map->background->w;
      dstr.h = map->background->h;

      SDL_BlitSurface(map->background, &srcr, screen->video, &dstr);
    }

  /* rysuje komorki ktore sa w zasiegu */
  int first_x = floor((double)px / CELL_SIZE);
  int last_x  = ceil((double)kx / CELL_SIZE) + 1;
  int first_y = floor((double)py / CELL_SIZE);
  int last_y  = ceil((double)ky / CELL_SIZE) + 2;
  
  for (y = first_y; y< last_y; y++) 
    for (x = first_x; x < last_x; x++) {
      cell_t *cell = &map->cells[y*map->width + x];
      mapobject_t *cur = cell->object;
      while (cur != NULL) {
	dstr.w = cur->image->w;
	dstr.h = cur->image->h;
	dstr.x = x * CELL_SIZE - map->camera_x + cur->offset_x;
	dstr.y = y * CELL_SIZE - map->camera_y - dstr.h + cur->offset_y;
	SDL_BlitSurface(cur->image, NULL, screen->video, &dstr);
	cur = cur->next;
      }
    }
}
