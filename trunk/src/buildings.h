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

typedef struct buildsystem_s {
  library_t * b_images;
} buildsystem_t;

buildsystem_t * buildsystem_new();
void buildsystem_free(buildsystem_t *bs);

building_t * building_new(buildsystem_t *bs);
void building_free(buildsystem_t *bs, building_t *b);

#endif
