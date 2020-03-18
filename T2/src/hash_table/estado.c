#include "hash_table.h"
#include <stdlib.h>
#include <stdio.h>


Estado* state_init(Direction dir, uint64_t hash_, Estado* father, Snek* snek) {
  Estado* state = malloc(sizeof(Estado));
  state->direction = dir;
  state->hash = hash_;
  state->father = father;  // crear nodo inicial con padre nulo
  state->next = NULL;
  state->snek = snek;
  state->q_next = NULL;
  return state;
}


/** Libera la memoria de un estado */
void state_destroy(Estado* state) {
  //printf("\n error in state_destroy - %llu - d: %d\n", state->hash, state->direction);
  if (state) {
    if (state->snek) {
      //printf("error call to snek_destroy %llu\n", state->hash);
      snek_destroy(state->snek);
    }
    //printf("error call state_destroy \n");
    if (state->next) {
      state_destroy(state->next);
    }
    //printf("error after state_destroy \n");
    free(state);
  }
}
