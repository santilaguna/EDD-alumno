#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "util.h"


/** Puente del tablero */
typedef struct bridge {
  /** Posición puente */
  Position pos;
  /** Grado puente */
  uint8_t degree;
  /** Positions changed to black */
  Position* changed;
  /** Puente alineado o no */
  bool aligned;
  /** Direcciones ordenadas */
  Direction* directions;
  /** Última dirección desplegada */
  Direction last_direction;
} Bridge;


// Funciones

/** Inicializa el tablero a partir del archivo */
Bridge* bridge_init(uint8_t row, uint8_t col, uint8_t degree);

/** Destruye el tablero liberando todo */
void bridge_destroy(Bridge* bridge);
