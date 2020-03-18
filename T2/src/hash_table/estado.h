#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "../snekbirb/snek.h"

/** Estructura de un estado */
typedef struct estado {

  /** hash */
  uint64_t hash;
  /** estado antecesor en bfs */
  struct estado* father;
  /** dirección en la cual nos acabamos de mover */
  Direction direction;
  /** próximo valor en hash_table */
  struct estado* next;
  /** snake copy */
  Snek* snek;
  /** próximo valor en queue */
  struct estado* q_next;
} Estado;

Estado* state_init(Direction dir, uint64_t hash_, Estado* father, Snek* snek);

void state_destroy(Estado* state);
