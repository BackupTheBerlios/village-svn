#ifndef _HUMANS_H_
#define _HUMANS_H_

#include "map.h"

#define HT_MAN   1
#define HT_WOMAN 2

#define MO_STAND 1
#define MO_WALK  2

typedef struct human_s {
  mapobject_t object;
  short int type;
  short int mode;

  short int walk_step;
  short int nextstep_delay;
  SDL_Surface *walk[4][6];
} human_t;

human_t * human_new(short int type);
void      human_free(human_t * human);

#endif
