#ifndef _MAP_H_
#define _MAP_H_

#include <SDL.h>
#include "screen.h"
#include "astar.h"

#define CELL_SIZE    16

#define MO_OTHER     0
#define MO_TREE      1
#define MO_ROCK      2
#define MO_BUSH      3
#define MO_BUILD     4
#define MO_MAN       5
#define MO_WOMAN     6

#define DI_STAND     0
#define DI_N         1
#define DI_NE        2
#define DI_NW        3
#define DI_S         4
#define DI_SE        5
#define DI_SW        6
#define DI_E         7
#define DI_W         8

typedef struct animation_s {
  SDL_Surface  * images[32];
  int            move_current;
  int            move_count;
  int            delay;
  int            current;
} animation_t;

struct cell_s;
typedef struct mapobject_s {
  short int             type;
  SDL_Surface         * image;
  short int             offset_x;
  short int             offset_y;
  short int             direction;
  short int             step_delay;
  short int             step_current;
  animation_t           animations[9];
  pointslist_t          road;
  struct cell_s       * parent;
  struct mapobject_s  * next;
} mapobject_t;

typedef struct cell_s {
  short int            x;
  short int            y;
  mapobject_t        * object;
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

void          cell_append(cell_t *cell, mapobject_t *obj);
void          cell_remove(cell_t *cell, mapobject_t *obj);
map_t       * map_new(int width, int height);
void          map_redraw(map_t *map, screen_t *screen, int px, int py, int kx, int ky);
void          map_fix_camera(map_t *map, int viewport_w, int viewport_h);
void          mapobject_update(mapobject_t *obj);

#endif
