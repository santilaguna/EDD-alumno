#include "../snekbirb/board.h"
#include "../random/pcg_basic.h"
#include "../hash_table/hash_table.h"
#include "../hash_table/estado.h"
#include <stdlib.h>


int main(int argc, char *argv[])
{
  // Revisar el input
  if (argc != 3)
  {
    printf("Modo de uso: ./snakebirb test.txt output.txt\n");
    return 0;
  }

  /*
  El main actualmente usa las funciones de crear y liberar tablero y se aplican
  movimientos según lo que se escriba por consola. También se abre la interfaz
  gráfica para mostrar su uso. Este código no tiene leaks ni errores y solamente
  sirve para mostrara la forma en que se usan las funciones.
  La modelación del programa ya está dada para que no tengan que modelar los
  movimientos. Sin embargo son libres de agregar o eliminar lo que quieran en
  los archivos de la carpeta snekbirb. Por ejemplo está recomendado almacenar
  una matriz de números aleatorios junto con el tablero y un valor de hash junto
  con la serpiente.
  La modelación actual está hecha de manera de que cada operación tenga costo
  igual al largo de la serpiente. Si bien se puede mover la serpiente en O(1)
  siempre y cuando no caiga, para la búsqueda es necesario copiar la serpiente.
  Es por esto que cada operación toma O(snek). Para tu función de hash trata de
  que el tiempo en calcularla no tome más que O(snek).
  Tu debes implementar el algoritmo de BFS para encontrar la secuencia de pasos
  mínima que resuelve el problema. Este algoritmo va a necesitar de una tabla de
  hash para alamacenar los estados ya visitados para no repetirlos. Finalmente
  tu código debe escribir en el archivo dado la secuencia de pasos ejecutados
  en el formato especificado en el enunciado.
  La interfaz es solamente para que puedas debuguear tu programa y no tiene
  influencia en la correccion.
  */

  // Abro el archivo
  FILE* file = fopen(argv[1], "r");

  // Creo el tablero
  Board* board = board_init(file);

  // Cierro el archivo
  fclose(file);

  // pedimos memoria para los randoms
  board->randoms = malloc(sizeof(uint64_t*) * board->height);
  for (int i = 0; i < board->height; i++) {
    board->randoms[i] = malloc(sizeof(uint64_t) * board->width);
    for (int j = 0; j < board->width; j++) {
      uint64_t posible = (uint64_t) get_random();
      while (posible - (posible % 10) == 0) posible = (uint64_t) get_random();
      board->randoms[i][j] = posible;
    }
  }

  // Arreglo de movimientos
  Direction movs[1000];
  uint16_t mov_count = 0;

  HashTable* dict = table_init(50023);  // escoger un primo adecuado

  Estado* initial = malloc(sizeof(Estado));
  initial->direction = HEAD;
  initial->hash = board->hash;
  initial->father = NULL;
  initial->next = NULL;
  initial->snek = board->snek;

  // agregamos el hash board
  hash_board(board);

  table_insert(dict, initial);

  Estado* father = initial;
  Estado* node_agregar = initial;   

  bool winner = false;
  Estado* curr_winner;
  int i = 0;
  
  while (!winner) {
    /*
    Esto es una demo de los movimientos de la serpiente y del modo de uso de la
    funcion board_move. La idea es que si la serpiente se mueve en una direccion
    invalida o si la serpiente muere al hacer el movimiento, se retorna NULL y
    no se modifica el tablero actual.
    Si el movimiento es válido, se retorna una nueva serpiente con la nueva
    posición. La serpiente anterior sigue existiendo.
    Los movimientos hechos se almacenan en el arreglo de movimientos para luego
    escribirlos en el archivo de output.
    */
    board->snek = father->snek;
    // printf("%d-%llu\n", i, father->hash);
    i+=1;

    for (int new_direction = UP; new_direction <= DOWN; new_direction++) {
      if (new_direction == UP && father->direction == DOWN) continue;
      if (new_direction == DOWN && father->direction == UP) continue;
      if (new_direction == LEFT && father->direction == RIGHT) continue;
      if (new_direction == RIGHT && father->direction == LEFT) continue;
      
      // Ejecuto el movimiento
      Snek* copy = board_move(board, new_direction);
      // Si hago un movimiento invalido o la serpiente muere, termino
      if (!copy) {
        continue;
      }
      
      uint64_t hash_ = incremental_board(board, new_direction, copy);
      Estado* new_state = state_init(new_direction, hash_, father, copy);

      // Si gane, termino
      if (board_is_win(board, copy)) {
        winner = true;
        curr_winner = new_state;
        int answer = table_insert(dict, new_state); 
        // ya pasamos por ese estado
        if (answer == 0) {
          return 2;  // nunca deberíamos haber pasado por el estado del portal y no haber ganado
        }
        break;
      }

      int answer = table_insert(dict, new_state); 
      // ya pasamos por ese estado
      if (answer == 0) {
        // printf("freed rep %llu\n", new_state->hash);
        //free(new_state);
        continue;
      }
      // se insertó pero debemos migrar la tabla
      else if (answer == -1) {
        // printf("MIGRATE TABLE\n");
        dict = table_migrate(dict);
      }
      // insertar a queue en ambos casos 
      node_agregar->q_next = new_state;
      node_agregar = new_state;
    } 

    father = father->q_next;
  }
  
  while (curr_winner->direction != HEAD) {
    movs[mov_count-1] = curr_winner->direction;
    //printf("%u-%u\n", curr_winner->direction, curr_winner->snek->tail->direction);
    mov_count += 1;
    curr_winner = curr_winner->father;
  }

  // Abro el archivo de output
  FILE* out = fopen(argv[2], "w");

  // Escribo el numero de movimientos
  fprintf(out, "%hu\n", mov_count);

  // Escribo los movimientos
  for (int i = mov_count - 1; i >= 0; i--) {
    fprintf(out, "%u\n", movs[i - 1]);
  }
 
  // Cierro el archivo de output
  fclose(out);
 
  // libero tabla con estados y sneks
  //printf("error call table_destroy \n");
  //table_destroy(dict);
  //printf("error after table_destroy \n");
  
  // Libero la memoria de los numero random
  //for (int i = 0; i < board->height; i++) {
  //  free(board->randoms[i]);
  //}
  //free(board->randoms);
  //printf("error call to board_destroy \n");
  // Libero la memoria del tablero
  //board_destroy(board);

  // Retorno que todo esta bien
  //printf("no error \n");
  return 0;
}
