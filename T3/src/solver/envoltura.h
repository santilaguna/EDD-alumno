#pragma once
#include <stdint.h>
#include "bridge.h"
#include "util.h"
#include "board.h"

// SOURCE: https://en.wikibooks.org/wiki/Algorithm_Implementation/Geometry/Convex_hull/Monotone_chain
/** Buscamos la envoltura convexa de los puentes + portal */
void convex_hull(Board* board, Position* points, uint8_t npoints);

/** revisar si un punto está dentro de la envoltura convexa*/
bool inside(Position pos, Board* board);
