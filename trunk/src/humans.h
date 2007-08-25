#ifndef _HUMANS_H_
#define _HUMANS_H_

#include "map.h"

typedef struct human_s {
  mapobject_t object;
} human_t;

human_t * human_new(short int type);
void      human_free(human_t * human);

#endif
