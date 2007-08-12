#ifndef _MAP_H_
#define _MAP_H_

#include <SDL.h>
#include "screen.h"

#define CELL_SIZE    16
#define CF_NOWALK    1
#define CF_NOBUILD   2
#define CF_NOALL     CF_NOWALK | CF_NOBUILD
#define CF_LINK      4

#define MO_TREE      1
#define MO_ROCK      2
#define MO_BUSH      3
#define MO_BUILD     4
#define MO_MAN       8
#define MO_OTHER     16

#define DI_N         0
#define DI_S         1
#define DI_W         2
#define DI_E         3

struct cell_s;
typedef struct mapobject_s {
  short int            type;
  SDL_Surface        * image;
  short int            offset_x;
  short int            offset_y;
  short int            direction;
  int                  def_flags;
  
  struct cell_s       * parent;
  struct mapobject_s  * next;
} mapobject_t;

typedef struct cell_s {
  short int            x;
  short int            y;
  mapobject_t        * object;
  int                  flags;
  struct cells       * link;
} cell_t;

typedef struct map_s {
  SDL_Surface        * background;
  int                  width;
  int                  height;
  int                  camera_x;
  int                  camera_y;
  cell_t             * cells;
} map_t;


void    cell_append(cell_t *cell, mapobject_t *obj);
void    cell_remove(cell_t *cell, mapobject_t *obj);
map_t * map_new(int width, int height);
void    map_redraw(map_t *map, screen_t *screen, int px, int py, int kx, int ky);
void    map_fix_camera(map_t *map, int viewport_w, int viewport_h);

#endif
