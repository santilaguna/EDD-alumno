#pragma once
#include <stdint.h>
#include "bridge.h"
#include "util.h"
#include "board.h"
#include "envoltura.h"


/** Recursivamente buscamos el camino hasta encontrar una combinación que llegue al final*/
int dfs_start(Board* board, Bridge** puentes, int* orden_visitados);

/** En este caso podamos menos, pero nos aseguramos de encontrar una solución*/
int dfs_start_2(Board* board, Bridge** puentes, int* orden_visitados);
