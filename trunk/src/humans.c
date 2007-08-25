#include "library.h"
#include "humans.h"

extern map_t *map;

human_t * human_new(short int type) {
  human_t * human = malloc(sizeof(human_t));
  memset(human, 0, sizeof(human_t));

  if (type == MO_MAN) {
    human->object.animations[DI_W].images[0] = library_find(resources.humans, "man1");
    human->object.animations[DI_W].images[1] = library_find(resources.humans, "man2");
    human->object.animations[DI_W].images[2] = library_find(resources.humans, "man3");
    human->object.animations[DI_W].images[3] = library_find(resources.humans, "man4");
    human->object.animations[DI_W].images[4] = library_find(resources.humans, "man5");
    human->object.animations[DI_W].images[5] = library_find(resources.humans, "man6");
    human->object.animations[DI_STAND].images[0] = library_find(resources.humans, "man1");
  } else if (type == MO_WOMAN) {
    human->object.animations[DI_W].images[0] = library_find(resources.humans, "woman1");
    human->object.animations[DI_W].images[1] = library_find(resources.humans, "woman2");
    human->object.animations[DI_W].images[2] = library_find(resources.humans, "woman3");
    human->object.animations[DI_W].images[3] = library_find(resources.humans, "woman4");
    human->object.animations[DI_W].images[4] = library_find(resources.humans, "woman5");
    human->object.animations[DI_W].images[5] = library_find(resources.humans, "woman6");
    human->object.animations[DI_STAND].images[0] = library_find(resources.humans, "woman1");
  }
  human->object.animations[DI_W].move_count=5;
  human->object.animations[DI_W].delay=20;
  human->object.type = type;
  human->object.image = human->object.animations[DI_W].images[0];
  human->object.direction = DI_W;
  human->object.step_delay = 20;
  return human;
}

void human_free(human_t * human) {
  free(human);
}
