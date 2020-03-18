#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "estado.h"

/** Estructura de una tabla de hash*/
typedef struct hash_table {
    uint32_t size;
    uint32_t count;
    Estado** values;
} HashTable;

/** Busca el valor de un elemento en la tabla de hash */
bool in_table(HashTable* table, Estado* state);

/** Función que crea una tabla de hash*/
HashTable* table_init(uint32_t size);

/** Inserta un elemento en la tabla de hash */
int table_insert(HashTable* table, Estado* state);

/** Libera la memoria de la tabla */
void table_destroy(HashTable* table);

/** migrate table to a bigger one */
HashTable* table_migrate(HashTable* table) ;
