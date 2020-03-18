#include "board.h"
#include <stdlib.h>


/** Inicializa el tablero a partir del archivo */
Board* board_init(uint8_t height, uint8_t width, uint8_t b_number) {
  // Creo el tablero
  Board* board = malloc(sizeof(Board));

  board->height = height;
  board->width = width;
  board->bridges_number = b_number;
  board->undo = 0;
  board->aligned_bridges = 0;
  board->hull_size = 0;

  // Creo la matriz y agrego los elementos del tablero
  board->cells = malloc(sizeof(Cell*) * board -> height);
  for (uint8_t row = 0; row < board -> height; row++) {
    board->cells[row] = malloc(sizeof(Cell) * board -> width);
    for (uint8_t col = 0; col < board -> width; col++) {
        board->cells[row][col].type = EMPTY;
        board->cells[row][col].pos.col = col;
        board->cells[row][col].pos.row = row;
    }
  }
  // Retorno el tablero
  return board;
}

/** Destruye el tablero liberando todo */
void board_destroy(Board* board) {
  // Libero la matriz de celdas
  for (uint8_t row = 0; row < board->height; row++) {
    free(board -> cells[row]);
  }
  free(board -> cells);

  // Libero el tablero
  free(board);
}
