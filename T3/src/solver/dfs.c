#include "dfs.h"
#include <stdlib.h>
#include <stdint.h>


/** Recursivamente buscamos el camino hasta encontrar una combinación que llegue al final */
int dfs(Board* board, Bridge** puentes, Direction dir, uint8_t bridge_number, int* orden_visitados, int recursion_level) {
    orden_visitados[bridge_number] = recursion_level;
    puentes[bridge_number]->last_direction = dir;
    Position pos;
    pos.row = puentes[bridge_number]->pos.row;
    pos.col = puentes[bridge_number]->pos.col;
    // si es un puente alineado lo descontamos
    if (puentes[bridge_number]->aligned) board->aligned_bridges -= 1;
    // desplegamos el puente
    for(int i = 0; i < puentes[bridge_number]->degree; i++) {
        pos = position_shift(pos, dir);
        if (pos.col >= board->width || pos.row >= board->height || pos.col < 0 || pos.row < 0) {
            // facilitamos el undo, le asignamos la misma posicion que el puente para indicar que no hay más cambios
            puentes[bridge_number]->changed[i].row = puentes[bridge_number]->pos.row;
            puentes[bridge_number]->changed[i].col = puentes[bridge_number]->pos.col;
            break;
        }
        else if (board->cells[pos.row][pos.col].type == END) {
            return recursion_level;
        }
        else if (board->cells[pos.row][pos.col].type == EMPTY) {
            board->cells[pos.row][pos.col].type = BLACK;
            puentes[bridge_number]->changed[i].row = pos.row;
            puentes[bridge_number]->changed[i].col = pos.col;
        }
        else if (board->cells[pos.row][pos.col].type == BRIDGE || board->cells[pos.row][pos.col].type == BLACK) {
            i -= 1;
        }
    }
    // poda envoltura, si no esta dentro dejamos de revisar este camino
    Direction new_dir;
    bool inside_hull = false;
    if (dir == UP) new_dir = DOWN;
    else if (dir == DOWN) new_dir = UP;
    else if (dir == LEFT) new_dir = RIGHT;
    else new_dir = LEFT;
    pos = position_shift(pos, new_dir);
    if (inside(pos, board)) inside_hull = true;
    // fin poda
    // Poda puentes alineados
    if (board->aligned_bridges > 0 && inside_hull) {
        // llamado recursivo
        for (uint8_t new_bridge_number = 0; new_bridge_number < board->bridges_number; new_bridge_number++) {
            if (orden_visitados[new_bridge_number] != -1) {
                continue;
            }
            for (uint8_t i = 0; i < 2; i++) {  // Direction is puentes[bridge_number]->directions[i]
                int ret = dfs(board, puentes, puentes[new_bridge_number]->directions[i], new_bridge_number, 
                            orden_visitados, recursion_level + 1);
                if (ret != -1) return ret;
            }
        }
        for (uint8_t new_bridge_number = 0; new_bridge_number < board->bridges_number; new_bridge_number++) {
            if (orden_visitados[new_bridge_number] != -1) {
                continue;
            }
            int ret = dfs(board, puentes, puentes[new_bridge_number]->directions[2], new_bridge_number, 
                            orden_visitados, 1);
            if (ret != -1) return ret;
        }
        for (uint8_t new_bridge_number = 0; new_bridge_number < board->bridges_number; new_bridge_number++) {
            if (orden_visitados[new_bridge_number] != -1) {
                continue;
            }
            int ret = dfs(board, puentes, puentes[new_bridge_number]->directions[3], new_bridge_number, 
                            orden_visitados, 1);
            if (ret != -1) return ret;
        }
    }
    // undo 
    if (puentes[bridge_number]->aligned) board->aligned_bridges += 1;
    orden_visitados[bridge_number] = -1;
    board->undo += 1;
    for(int i = 0; i < puentes[bridge_number]->degree; i++) {
        if (board->cells[puentes[bridge_number]->changed[i].row][puentes[bridge_number]->changed[i].col].type == BRIDGE) {
            break;
        }
        board->cells[puentes[bridge_number]->changed[i].row][puentes[bridge_number]->changed[i].col].type = EMPTY;
    }
    return -1;
}

int dfs_start(Board* board, Bridge** puentes, int* orden_visitados) {
    for (uint8_t new_bridge_number = 0; new_bridge_number < board->bridges_number; new_bridge_number++) {
        if (orden_visitados[new_bridge_number] != -1) {
            continue;
        }
        for (uint8_t i = 0; i < 2; i++) {  // Direction is puentes[bridge_number]->directions[i]
            int ret = dfs(board, puentes, puentes[new_bridge_number]->directions[i], new_bridge_number, 
                          orden_visitados, 1);
            if (ret != -1) return ret;
        }
    }
    for (uint8_t new_bridge_number = 0; new_bridge_number < board->bridges_number; new_bridge_number++) {
        if (orden_visitados[new_bridge_number] != -1) {
            continue;
        }
        int ret = dfs(board, puentes, puentes[new_bridge_number]->directions[2], new_bridge_number, 
                        orden_visitados, 1);
        if (ret != -1) return ret;
    }
    for (uint8_t new_bridge_number = 0; new_bridge_number < board->bridges_number; new_bridge_number++) {
        if (orden_visitados[new_bridge_number] != -1) {
            continue;
        }
        int ret = dfs(board, puentes, puentes[new_bridge_number]->directions[3], new_bridge_number, 
                        orden_visitados, 1);
        if (ret != -1) return ret;
    }
    return -1;
}

// PODA MENOS EFICIENTE, PERO ASEGURA SOLUCIÓN

/** En caso de podar más de lo necesario podando con el último elemento del puente, intentamos podando el primero*/
int dfs_2(Board* board, Bridge** puentes, Direction dir, uint8_t bridge_number, int* orden_visitados, int recursion_level) {
    // poda envoltura, si no esta dentro dejamos de revisar este camino
    Position first_move;
    first_move.col = puentes[bridge_number]->pos.col;
    first_move.row = puentes[bridge_number]->pos.row;
    first_move = position_shift(first_move, dir);
    // fin poda
    if (!inside(first_move, board)) return -1;
    orden_visitados[bridge_number] = recursion_level;
    puentes[bridge_number]->last_direction = dir;
    Position pos;
    pos.row = puentes[bridge_number]->pos.row;
    pos.col = puentes[bridge_number]->pos.col;
    // si es un puente alineado lo descontamos
    if (puentes[bridge_number]->aligned) board->aligned_bridges -= 1;
    // desplegamos el puente
    for(int i = 0; i < puentes[bridge_number]->degree; i++) {
        pos = position_shift(pos, dir);
        if (pos.col >= board->width || pos.row >= board->height || pos.col < 0 || pos.row < 0) {
            // facilitamos el undo, le asignamos la misma posicion que el puente para indicar que no hay más cambios
            puentes[bridge_number]->changed[i].row = puentes[bridge_number]->pos.row;
            puentes[bridge_number]->changed[i].col = puentes[bridge_number]->pos.col;
            break;
        }
        else if (board->cells[pos.row][pos.col].type == END) {
            return recursion_level;
        }
        else if (board->cells[pos.row][pos.col].type == EMPTY) {
            board->cells[pos.row][pos.col].type = BLACK;
            puentes[bridge_number]->changed[i].row = pos.row;
            puentes[bridge_number]->changed[i].col = pos.col;
        }
        else if (board->cells[pos.row][pos.col].type == BRIDGE || board->cells[pos.row][pos.col].type == BLACK) {
            i -= 1;
        }
    }
    // Poda puentes alineados
    if (board->aligned_bridges > 0) {
        // llamado recursivo
        for (uint8_t new_bridge_number = 0; new_bridge_number < board->bridges_number; new_bridge_number++) {
            if (orden_visitados[new_bridge_number] != -1) {
                continue;
            }
            for (uint8_t i = 0; i < 2; i++) {  // Direction is puentes[bridge_number]->directions[i]
                int ret = dfs_2(board, puentes, puentes[new_bridge_number]->directions[i], new_bridge_number, 
                            orden_visitados, recursion_level + 1);
                if (ret != -1) return ret;
            }
        }
        for (uint8_t new_bridge_number = 0; new_bridge_number < board->bridges_number; new_bridge_number++) {
            if (orden_visitados[new_bridge_number] != -1) {
                continue;
            }
            int ret = dfs_2(board, puentes, puentes[new_bridge_number]->directions[2], new_bridge_number, 
                            orden_visitados, recursion_level + 1);
            if (ret != -1) return ret;
        }
        for (uint8_t new_bridge_number = 0; new_bridge_number < board->bridges_number; new_bridge_number++) {
            if (orden_visitados[new_bridge_number] != -1) {
                continue;
            }
            int ret = dfs_2(board, puentes, puentes[new_bridge_number]->directions[3], new_bridge_number, 
                            orden_visitados, recursion_level + 1);
            if (ret != -1) return ret;
        }
    }
    // undo 
    if (puentes[bridge_number]->aligned) board->aligned_bridges += 1;
    orden_visitados[bridge_number] = -1;
    board->undo += 1;
    for(int i = 0; i < puentes[bridge_number]->degree; i++) {
        if (board->cells[puentes[bridge_number]->changed[i].row][puentes[bridge_number]->changed[i].col].type == BRIDGE) {
            break;
        }
        board->cells[puentes[bridge_number]->changed[i].row][puentes[bridge_number]->changed[i].col].type = EMPTY;
    }
    return -1;
}

/** En caso de podar más de lo debido */
int dfs_start_2(Board* board, Bridge** puentes, int* orden_visitados) {
    for (uint8_t new_bridge_number = 0; new_bridge_number < board->bridges_number; new_bridge_number++) {
        if (orden_visitados[new_bridge_number] != -1) {
            continue;
        }
        for (uint8_t i = 0; i < 2; i++) {  // Direction is puentes[bridge_number]->directions[i]
            int ret = dfs_2(board, puentes, puentes[new_bridge_number]->directions[i], new_bridge_number, 
                          orden_visitados, 1);
            if (ret != -1) return ret;
        }
    }
    for (uint8_t new_bridge_number = 0; new_bridge_number < board->bridges_number; new_bridge_number++) {
        if (orden_visitados[new_bridge_number] != -1) {
            continue;
        }
        int ret = dfs_2(board, puentes, puentes[new_bridge_number]->directions[2], new_bridge_number, 
                        orden_visitados, 1);
        if (ret != -1) return ret;
    }
    for (uint8_t new_bridge_number = 0; new_bridge_number < board->bridges_number; new_bridge_number++) {
        if (orden_visitados[new_bridge_number] != -1) {
            continue;
        }
        int ret = dfs_2(board, puentes, puentes[new_bridge_number]->directions[3], new_bridge_number, 
                        orden_visitados, 1);
        if (ret != -1) return ret;
    }
    return -1;
}
