#include "envoltura.h"
#include <stdlib.h>
#include <stdint.h>

// SOURCE: https://en.wikibooks.org/wiki/Algorithm_Implementation/Geometry/Convex_hull/Monotone_chain

/** comparison function to sort positions */
static int compare(const void * pos_1, const void * pos_2) {
    Position* pos1 = (Position*) pos_1;
    Position* pos2 = (Position*) pos_2;
    if (pos1->col > pos2->col) return 1;
    else if (pos1->col < pos2->col) return -1;
    else if(pos1->row > pos2->row) return 1;
    else return -1;
}

/* Three points are a counter-clockwise turn if ccw > 0, clockwise if
 * ccw < 0, and collinear if ccw = 0 because ccw is a determinant that
 * gives the signed area of the triangle formed by p1, p2 and p3.
 */
static double ccw(Position p1, Position p2, Position p3) {
  return ((double) p2.col - (double) p1.col) * ((double) p3.row - (double) p1.row) 
        - ((double) p2.row - (double) p1.row) * ((double) p3.col - (double) p1.col);
}

/** Buscamos la envoltura convexa de los puentes + portal */
/* Returns a list of points on the convex hull in counter-clockwise order.
 * Note: the last point in the returned list is the same as the first one.
 */
void convex_hull(Board* board, Position* points, uint8_t npoints) {
    //Position** out_hull, uint8_t* out_hullsize

  uint8_t i, t, k = 0;
  qsort(points, npoints, sizeof(Position), compare);

  board->hull = malloc(sizeof(Position) * (npoints));

  /* lower hull */
  for (i = 0; i < npoints; ++i) {
    while (k >= 2 && ccw(board->hull[k-2], board->hull[k-1], points[i]) <= 0) {
      --k;
    }
    board->hull[k] = points[i];
    k += 1;
  }

  /* upper hull */
  for (i = npoints-1, t = k+1; i > 0; --i) {
    while (k >= t && ccw(board->hull[k-2], board->hull[k-1], points[i-1]) <= 0) {
      --k;
    }
    board->hull[k++] = points[i-1];
  }
 
  //if (k > 3) 
  // si la envoltura tiene 3 o menos puntos no necesitamos probar
  board->hull_size = k;  // hull size (+1) 
}

bool inside(Position pos, Board* board) {
    bool inside = true;
    for(uint8_t i = 0; i < board->hull_size; i++) {
      if (position_eq(pos, board->hull[i])) return true;
    }
    double res;
    for(uint8_t ind = 1; ind < board->hull_size; ind ++) {
        res = ccw(board->hull[ind-1], board->hull[ind], pos);
        if (res < 0) {
            inside = false;
        }
    }
    return inside;
}
