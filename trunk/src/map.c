#include <math.h>
#include <stdlib.h>
#include "map.h"
#include "library.h"

#define rand(min,max) (min) +(int) (((float)max)*rand()/(RAND_MAX+((float)min)))

map_t *map;

mapobject_t *mapobject_new(SDL_Surface *image, int type) {
  mapobject_t *obj = malloc(sizeof(mapobject_t));
  memset(obj, 0, sizeof(mapobject_t));
  obj->image = image;
  obj->type = type;
  obj->direction = DI_STAND;
  return obj;
}

void mapobject_free(mapobject_t *obj) {
  list_clean(&obj->road);
  free(obj);
}

void mapobject_update(mapobject_t *obj) {
  /* aktualizuje pozycje obiektu */
  if (obj->step_delay == obj->step_current) {
    switch (obj->direction) {
    case DI_W: 
      {
	obj->offset_x --;
	if (obj->offset_x < CELL_SIZE/-2) {
	  cell_t *cell = &map->cells[obj->parent->y * map->width + obj->parent->x - 1];
	  cell_remove(obj->parent, obj);
	  cell_append(cell, obj);
	  obj->offset_x = CELL_SIZE/2 - 1;
	  break;
	}
      }
    }
    obj->step_current = 0;
  } else obj->step_current ++;

  /* aktualizuje klatke animacji obiektu */
  animation_t *ani = &obj->animations[obj->direction];
  if (ani->current == ani->delay) {
    ani->move_current++;
    if (ani->move_current > ani->move_count) ani->move_current = 0;
    obj->image = ani->images[ani->move_current];
    ani->current = 0;
  } else ani->current ++;
  
  /* sprawdzam czy obiekt nie wyszedl poza mape */
  if (obj->parent->x == 0 || obj->parent->x == map->width ||
      obj->parent->y == 0 || obj->parent->y == map->height) 
    obj->direction = DI_STAND;
}

void cell_append(cell_t *cell, mapobject_t *obj) {
  /* jezeli w komorce nie ma zadnego obietku to dodaje ten
     jako obiekt pierwszy */
  if (cell->object == NULL) {
    cell->object = obj;
    obj->next = NULL;
    obj->parent = cell;
  } else {
    mapobject_t *cur = cell->object;
    /* jade do konca i dopisuje obiekt do konca komorki */
    while (cur->next != NULL) cur = cur->next;
    cur->next = obj;
    obj->next = NULL;
    obj->parent = cell;
  }
}

void cell_remove(cell_t *cell, mapobject_t *obj) {
  /* pomijam jesli komorka nie ma obiektow */
  if (cell->object == NULL) return;
  /* jesli usuwany obiekt jest obiektem glowny trzeba 
     obiekt nastepny ustawic jako glowny */
  if (cell->object == obj) {
    cell->object = obj->next;
    obj->parent = NULL;
  } else {
  /* szukam usuwanego obiektu na liscie i jesli go znajde
     to przesuwam obiekty a tego usuwam */
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
    int x = rand(0, map->width);
    int y = rand(0, map->height);
    cell_t *cell = &map->cells[x*map->width + y];
    if (cell->object == NULL) {
      SDL_Surface *img = resources.flora->items[rand(0,3)].image;
      mapobject_t *object = mapobject_new(img, MO_TREE);
      cell_append(cell, object);
    } else goto randposition;
    
  }

  for (i=0; i < (map->width * map->height) / 10; i++) {
  randposition2:;
    int x = rand(0, map->width-1);
    int y = rand(0, map->height);
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
  if (last_y > map->height) last_y = map->height;
  if (last_x > map->width) last_x = map->width;
  
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
