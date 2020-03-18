#include "bridge.h"
#include <stdlib.h>
#include <stdbool.h>


/** Inicializa el puente a partir del archivo */
Bridge* bridge_init(uint8_t row, uint8_t col, uint8_t degree) {
    // Creo el puente
    Bridge* bridge= malloc(sizeof(Bridge));
    
    bridge->aligned = false;
    bridge->pos.col = col;
    bridge->pos.row = row;
    bridge->degree =  degree;
    bridge->changed =  malloc(sizeof(Position) * degree);
    bridge->directions = malloc(sizeof(Direction) * 4);
    //bridge->last_direction = NULL;
    
    // Retorno el puente
    return bridge;
}

/** Destruye el puente liberando todo */
void bridge_destroy(Bridge* bridge) {
    free(bridge->directions);
    free(bridge->changed);
    free(bridge);
}
