#ifndef _BUILDINGS_H_
#define _BUILDINGS_H_

#include "map.h"
#include "library.h"

#define BT_GARNER     1
#define BT_LUMBERJACK 2
#define BT_LUMBERMILL 4
#define BT_HOUSE      8

typedef struct building_s {
  mapobject_t object;
  int building_type;
} building_t;

building_t * building_new();
void building_free(building_t *b);

#endif
