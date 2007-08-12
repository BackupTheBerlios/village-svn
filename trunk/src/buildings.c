#include "buildings.h"

buildsystem_t * buildsystem_new() {
  buildsystem_t *bs = malloc(sizeof(buildsystem_t));
  bs->b_images = library_open("buildings");
  return bs;
}

void buildsystem_free(buildsystem_t *bs) {
  library_free(bs->b_images);
  free(bs);
}

building_t * building_new(buildsystem_t *bs) {
}

void building_free(buildsystem_t *bs, building_t *b) {
}
