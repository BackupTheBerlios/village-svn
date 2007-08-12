/**********************************************************
 
Algortym: Wyszukiwanie drogi A-Star
Licencja: http://www.gnu.org/licenses/lgpl.txt
Autor:    Kamil Pawlowski <kamilpe@gmail.com>

**********************************************************/
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "astar.h"

#define LIST_NOMINAL 4096


/* *** IMPLEMENTACJA LISTY DYNAMICZNEJ ***  */


void inline list_clean(pointslist_t *list) {
  free(list->points);
  list->points = NULL;
  list->size = 0;
  list->count = 0;
}

void inline list_append(pointslist_t *list, const point_t *point) {
  if (list->points == NULL) {
    list->size = sizeof(point_t) * LIST_NOMINAL;
    list->count = 0;
    list->points = malloc(list->size);
  }
  if (list->size == list->count * sizeof(point_t)) {
    list->size = list->size * 2;
    list->points = realloc(list->points, list->size);
  }
  memcpy(&list->points[list->count], point, sizeof(point_t));
  list->count ++;
}

void inline list_prepend(pointslist_t *list, const point_t *point) {
  if (list->points == NULL) {
    list->size = sizeof(point_t) * LIST_NOMINAL;
    list->count = 0;
    list->points = malloc(list->size);
  }
  if (list->size == list->count * sizeof(point_t)) {
    list->size = list->size * 2;
    list->points = realloc(list->points, list->size);
  }
  memmove(&list->points[1], list->points, (list->count * sizeof(point_t)));
  memcpy(&list->points[0], point, sizeof(point_t));
  list->count ++;
}

void inline list_remove(pointslist_t *list, const point_t *point) {
  int position;
  for (position = 0; position < list->count; position++) {
    if (list->points[position].x == point->x &&
	list->points[position].y == point->y) break;
  }
  list->count --;
  memmove(&list->points[position], &list->points[position+1], (list->count - position) * sizeof(point_t));
}


/* *** OBSLUGA PUNKTOW *** */


point_t inline * list_find(pointslist_t *list, int x, int y) {
  int position;
  for (position = list->count - 1; position >= 0; position--) {
    if (list->points[position].x == x &&
	list->points[position].y == y) return &list->points[position];
  }
  return NULL;
}

point_t inline * list_find_low(pointslist_t *list) {
  int position;
  point_t *low = &list->points[0];
  for (position = list->count - 1; position >= 0; position--) {
    if (list->points[position].f < low->f) 
      low = &list->points[position];
  }
  return low;
}

void inline point_calc(astar_t *astar, point_t *point) {
  int w,h;
  
  point->g = 1;

  w = astar->dst_x - point->x;
  h = astar->dst_y - point->y;
  if (w < 0) w *= -1;
  if (h < 0) h *= -1;
  point->h = w + h;

  point->f = point->g + point->h;
}


/*  *** ALGORYTM A-STAR ***  */


void inline point_check(astar_t *astar, int x, int y) {
  if (list_find(&astar->closedlist, x, y) == NULL && 
      !astar->callback_roadblock(astar->callback_data, x, y)) {
    
    point_t *point;
    if ((point = list_find(&astar->openlist, x, y)) == NULL) {
      point_t point;
      point.x = x;
      point.y = y;
      point.parent_x = astar->q.x;
      point.parent_y = astar->q.y;
      point_calc(astar, &point);
      list_append(&astar->openlist, &point);
      if (point.x == astar->dst_x && point.y == astar->dst_y) astar->final = 1;
    } else if (point->g < astar->q.g) {
      point->parent_x = astar->q.x;
      point->parent_y = astar->q.y;
    }
  }
}

void astar_init(astar_t *astar) {
  memset(astar,0,sizeof(astar_t));
}

void astar_start(astar_t *astar) {
  astar->openlist.count = 0;
  astar->closedlist.count = 0;
  astar->road.count = 0;
  
  point_t point;
  point.x = astar->src_x;
  point.y = astar->src_y;
  point.f = 0;
  point.g = 0;
  point.h = 0;
  list_append(&astar->openlist, &point);

  astar->final = 0;
  while (!(astar->final && astar->openlist.count)) {

    memcpy(&astar->q, list_find_low(&astar->openlist), sizeof(point_t));
    list_remove(&astar->openlist, &astar->q);
    list_append(&astar->closedlist, &astar->q);

    if (!astar->final) point_check(astar, astar->q.x-1, astar->q.y-1);
    if (!astar->final) point_check(astar, astar->q.x,   astar->q.y-1);
    if (!astar->final) point_check(astar, astar->q.x+1, astar->q.y-1);
    if (!astar->final) point_check(astar, astar->q.x-1, astar->q.y);
    if (!astar->final) point_check(astar, astar->q.x+1, astar->q.y);
    if (!astar->final) point_check(astar, astar->q.x-1, astar->q.y+1);
    if (!astar->final) point_check(astar, astar->q.x,   astar->q.y+1);
    if (!astar->final) point_check(astar, astar->q.x+1, astar->q.y+1);
  }

  point_t *last = &astar->q;
  while (last != NULL) {
    list_prepend(&astar->road, last);
    last = list_find(&astar->closedlist, last->parent_x, last->parent_y);
  }
};

void astar_clean(astar_t *astar) {
  list_clean(&astar->openlist);
  list_clean(&astar->closedlist);
  list_clean(&astar->road);
}
