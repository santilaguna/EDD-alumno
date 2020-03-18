#pragma once
#include <stdint.h>
#include <stdio.h>
#include "util.h"


/** Elementos posibles que pueden haber en el tablero */
typedef enum celltype {
  EMPTY = 0,
  BLACK = 1,
  BRIDGE = 2,
  END = 3
} CellType;

/** Celda de la matriz */
typedef struct cell {
  /** Tipo de celda */
  CellType type;
  /** posición celda */
  Position pos;
} Cell;

/** Estructura del tablero */
typedef struct board {
  /** Alto del tablero */
  uint8_t height;
  /** Ancho del tablero */
  uint8_t width;
  /** Matriz de celdas del tablero de height x width */
  Cell** cells;
  /** Cantidad de puentes al inicio */
  uint8_t bridges_number;
  /** cantidad de undo */
  uint32_t undo;
  /** puentes alineados con la meta */
  uint8_t aligned_bridges;
  /** convex hull */
  Position* hull;
  /** hull size */
  uint8_t hull_size;
} Board;

// Funciones

/** Inicializa el tablero a partir del archivo */
Board* board_init(uint8_t height, uint8_t width, uint8_t b_number);

/** Destruye el tablero liberando todo */
void board_destroy(Board* board);
