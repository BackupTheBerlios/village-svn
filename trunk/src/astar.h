/**********************************************************
 
Algortym: Wyszukiwanie drogi A-Star
Licencja: http://www.gnu.org/licenses/lgpl.txt
Autor:    Kamil Pawlowski <kamilpe@gmail.com>

**********************************************************/
#ifndef _ASTAR_H_
#define _ASTAR_H_

typedef int (is_roadblock_fn) (void *, int, int);

typedef struct point_s {
  int  x;                               /* polozenie pola na osi X             */
  int  y;                               /* polozenia pola na osi Y             */
  int  g;                               /* koszt dotarcia do pola              */
  int  h;                               /* koszt dotarcia do celu od tego pola */
  int  f;                               /* laczny koszt tego pola              */
  int  parent_x;                        /* rodzic tego pola                    */
  int  parent_y;                        /* rodzic tego pola                    */
} point_t;

typedef struct pointslist_s {
  point_t *points;                      /* lista punktow na liscie             */
  int size;                             /* dlugosc zaalokowanej listy          */
  int count;                            /* faktyczna ilosc punktow na liscie   */
} pointslist_t;

typedef struct astar_s {
  int               final;              /* flaga okresla czy osiagnieto koniec sciezki */
  int               src_x;              /* startowe pole na osi X              */
  int               src_y;              /* startowe pole na osi Y              */
  int               dst_x;              /* docelowe pole na osi X              */
  int               dst_y;              /* docelowe pole na ois Y              */
  point_t           q;                  /* wskaznik na aktualne pole           */
  pointslist_t      openlist;           /* lista pol otwartych                 */
  pointslist_t      closedlist;         /* lista pol zamknietych               */
  pointslist_t      road;               /* lista pol zamknietych               */
  void            * callback_data;      /* dane przekazywane do callback       */
  is_roadblock_fn * callback_roadblock; /* funkcja wirtualna do sprawdzania czy pole jest przeszkoda */
} astar_t;

void astar_init(astar_t *astar);
void astar_start(astar_t *astar);
void astar_clean(astar_t *astar);

#endif
