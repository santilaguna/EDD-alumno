#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include "board.h"
#include "util.h"
#include "bridge.h"
#include "dfs.h"
#include "envoltura.h"


int main(int argc, char** argv) {
	// Revisamos que los parámetros sean correctos
	if(argc != 3) {
		printf("Uso: %s <test> <out>\nDonde\n", argv[0]);
    printf("\t<test> es la ruta al archivo a resolver\n");
		printf("\t<out> es la ruta al archivo donde se imprimirá el output\n");
		return 1;
	}

  // Abrimos el archivo en modo lectura
  char* input_filename = argv[1];
  FILE* input_file = fopen(input_filename, "r");

  if(!input_file) {
    fprintf(stderr, "El archivo %s no se pudo abrir. ¿Seguro que existe?\n", input_filename);
    return 2;
  }

	// Las dimensiones del problema
	uint8_t height;
	uint8_t width;
	// Leemos las dimensiones del  a partir del archivo
	fscanf(input_file, "%hhu %hhu", &height, &width);
  
  // La cantidad de puentes que tiene el problema
  uint8_t cantidad_puentes;
  fscanf(input_file, "%hhu", &cantidad_puentes);
 
  Board* board = board_init(height, width, cantidad_puentes);
  Bridge** puentes = malloc(sizeof(Bridge*) * cantidad_puentes);

  for(int i = 0; i < cantidad_puentes; i++) {

    uint8_t row;
    uint8_t col;
    uint8_t degree;

    fscanf(input_file, "%hhu %hhu %hhu", &row, &col, &degree);

    board->cells[row][col].type = BRIDGE;
    // agregamos al array de puentes
    puentes[i] = bridge_init(row, col, degree);
  }

  // Las coordenadas de la meta
  uint8_t goal_row;
  uint8_t goal_col;
  fscanf(input_file, "%hhu %hhu", &goal_row, &goal_col);
  Position goal_pos;
  goal_pos.row = goal_row;
  goal_pos.col = goal_col;
  board->cells[goal_row][goal_col].type = END;

	// Cerramos el archivo
	fclose(input_file);

  // Preprocesar el orden de revisión de los puentes, alineados al final
  uint8_t final = cantidad_puentes - 1;
  for (int i = 0; i < final; i++) {
    if (position_aligned(puentes[i]->pos, goal_pos)) {
      Bridge* aux = puentes[i];
      puentes[i] = puentes[final];
      puentes[final] = aux;
      final -= 1;
      i -= 1;
    } 
  }

  // array que indica las posiciones de revisión
  int* orden_visitados =  malloc(sizeof(int) * (cantidad_puentes));
  Position* puntos_envoltura = malloc(sizeof(Position) * (cantidad_puentes + 1));
  // Asignar Direcciones
  for (uint8_t i = 0; i < cantidad_puentes; i++) {
    puntos_envoltura[i].col = puentes[i]->pos.col;
    puntos_envoltura[i].row = puentes[i]->pos.row;
    orden_visitados[i] = -1;
    // sin heurísitca
    //puentes[i]->directions[0] = UP;
    //puentes[i]->directions[1] = LEFT;
    //puentes[i]->directions[2] = RIGHT;
    //puentes[i]->directions[3] = DOWN;
    // HEURÍSTICA DE DIRECCIONES
    // posiciones alineadas       
    if (position_aligned(puentes[i]->pos, goal_pos)) {
      // aprovechamos de indicarlo para la poda
      puentes[i]->aligned = true;
      board->aligned_bridges += 1;
      Direction first_dir = position_direction(puentes[i]->pos, goal_pos);
      if (first_dir == UP) {
        puentes[i]->directions[0] = UP;
        puentes[i]->directions[1] = LEFT;
        puentes[i]->directions[2] = RIGHT;
        puentes[i]->directions[3] = DOWN;
      }
      else if (first_dir == DOWN) {
        puentes[i]->directions[0] = DOWN;
        puentes[i]->directions[1] = LEFT;
        puentes[i]->directions[2] = RIGHT;
        puentes[i]->directions[3] = UP;

      }
      else if (first_dir == LEFT) {
        puentes[i]->directions[0] = LEFT;
        puentes[i]->directions[1] = UP;
        puentes[i]->directions[2] = DOWN;
        puentes[i]->directions[3] = RIGHT;
      }
      else {
        puentes[i]->directions[0] = RIGHT;
        puentes[i]->directions[1] = UP;
        puentes[i]->directions[2] = DOWN;
        puentes[i]->directions[3] = LEFT;
      }
    }
    //  no están alineadas
    else {
      if (puentes[i]->pos.row > goal_row) {
        if (puentes[i]->pos.col < goal_col) {
          puentes[i]->directions[0] = RIGHT;
          puentes[i]->directions[1] = UP;
          puentes[i]->directions[2] = DOWN;
          puentes[i]->directions[3] = LEFT;
        }
        else {
          puentes[i]->directions[0] = LEFT;
          puentes[i]->directions[1] = UP;
          puentes[i]->directions[2] = DOWN;
          puentes[i]->directions[3] = RIGHT;
        }
      }
      else {
        if (puentes[i]->pos.col < goal_col) {
          puentes[i]->directions[0] = RIGHT;
          puentes[i]->directions[1] = DOWN;
          puentes[i]->directions[2] = UP;
          puentes[i]->directions[3] = LEFT;
        }
        else {
          puentes[i]->directions[0] = LEFT;
          puentes[i]->directions[1] = DOWN;
          puentes[i]->directions[2] = UP;
          puentes[i]->directions[3] = RIGHT;
        }
      }
    }
  }
  puntos_envoltura[cantidad_puentes].row = goal_row;
  puntos_envoltura[cantidad_puentes].col = goal_col;

  convex_hull(board, puntos_envoltura, cantidad_puentes + 1);
  /** for(int i = 0; i < board->hull_size; i++) {
    printf("%hhu %hhu", board->hull[i].row, board->hull[i].col);
    printf("--%d\n", inside(board->hull[i], board));
  }*/
  int last = dfs_start_2(board, puentes, orden_visitados);
  if (last == -1) printf("ERROR\n");

  // Abrimos el archivo en modo escritura
  char* output_filename = argv[2];
  FILE* output_file = fopen(output_filename, "w");

  if(!output_file) {
    fprintf(stderr, "El archivo %s no se pudo abrir. ¿Tienes los permisos necesarios?\n", input_filename);
    return 2;
  }

  // Escribimos el resultado en el archivo de output
  fprintf(output_file,"%d\n", last);

  for (int i = 1; i <= last; i++) {
    for (int bridge_pos = 0; bridge_pos < cantidad_puentes; bridge_pos ++) {
      if (orden_visitados[bridge_pos] == i) {
        fprintf(output_file,"%d %d %d\n", puentes[bridge_pos]->pos.row, puentes[bridge_pos]->pos.col, 
                puentes[bridge_pos]->last_direction);
        break;
      }
    }
    
  }

  // Cerramos el archivo
  fclose(output_file);

  // liberamos orden visitados
  free(orden_visitados);
  // liberamos puntos contenidos en envolura
  free(puntos_envoltura);
  // liberamos envoltura convexa
  free(board->hull); 
  // liberamos el tablero
  board_destroy(board);
  // liberamos puentes
  for (uint8_t i = 0; i < cantidad_puentes; i++){
    bridge_destroy(puentes[i]);
  }
  free(puentes);

	/* Retornamos 0 indicando que todo salió bien */
  return 0;
}
