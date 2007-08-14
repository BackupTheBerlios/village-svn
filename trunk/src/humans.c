#include "library.h"
#include "humans.h"

extern map_t *map;

human_t * human_new(short int type) {
  human_t * human = malloc(sizeof(human_t));

  if (type == HT_MAN) {
    human->walk[DI_W][0] = library_find(resources.humans, "man1");
    human->walk[DI_W][1] = library_find(resources.humans, "man2");
    human->walk[DI_W][2] = library_find(resources.humans, "man3");
    human->walk[DI_W][3] = library_find(resources.humans, "man4");
    human->walk[DI_W][4] = library_find(resources.humans, "man5");
    human->walk[DI_W][5] = library_find(resources.humans, "man6");
  } else if (type == HT_WOMAN) {
    human->walk[DI_W][0] = library_find(resources.humans, "woman1");
    human->walk[DI_W][1] = library_find(resources.humans, "woman2");
    human->walk[DI_W][2] = library_find(resources.humans, "woman3");
    human->walk[DI_W][3] = library_find(resources.humans, "woman4");
    human->walk[DI_W][4] = library_find(resources.humans, "woman5");
    human->walk[DI_W][5] = library_find(resources.humans, "woman6");
  }

  human->type = type;
  human->mode = MO_WALK;
  human->walk_step = 0;

  human->nextstep_delay = 0;
  human->object.type = MO_MAN;
  human->object.image = human->walk[DI_W][0];
  human->object.direction = DI_W;
  human->object.offset_x = 0;
  human->object.offset_y = 0;
  return human;
}

void human_free(human_t * human) {
  free(human);
}

void human_update(human_t *human) {
  switch (human->mode) {
  case MO_STAND: {
    human->object.offset_x = 0;
    human->object.offset_y = 0;
    human->object.image = human->walk[human->object.direction][0];
    break;
  }
  case MO_WALK: {
    human->nextstep_delay ++;
    if (human->nextstep_delay > 20) {
      if (human->walk_step > 5) {
	human->walk_step = 0;

	switch (human->object.direction) {
	case DI_W: {
	  human->object.offset_x -= 6;
	  if (human->object.offset_x < -11) {
	    cell_t *cell = &map->cells[human->object.parent->y * map->width + human->object.parent->x - 1];
	    cell_remove(human->object.parent, (mapobject_t*)human);
	    cell_append(cell, (mapobject_t*)human);
	    human->object.offset_x = 0;
	    break;
	  }
	}
	}
	
	if (human->object.parent->x == 0 || human->object.parent->x == map->width ||
	    human->object.parent->y == 0 || human->object.parent->y == map->height) 
	  human->mode = MO_STAND;
      }
      human->nextstep_delay = 0;
      human->object.image = human->walk[human->object.direction][human->walk_step];
      human->walk_step ++;
    }
    break;
  } 
  }
}

