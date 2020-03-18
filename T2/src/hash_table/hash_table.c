#include "hash_table.h"
#include <stdlib.h>
#include <stdio.h>


/** Busca el valor de un elemento en la tabla de hash y retorna true si existe*/
bool in_table(HashTable* table, Estado* state) {
    uint32_t pos = state->hash % table->size;
    Estado* current = table->values[pos];
    while (current) {
        if (current->hash == state->hash) {
            // si hay un error tenemos muy mala suerte
            return true;      
        }
        current = current->next;      
    }
    return false;
}

/** Función que crea una tabla de hash*/
HashTable* table_init(uint32_t size_) {
    HashTable* table = malloc(sizeof(HashTable));
    if (table == NULL) return NULL;  // not enough memory
    table->size = size_;
    table->count = 0;
    table->values = calloc(size_, sizeof(Estado*));
    for (int row = 0; row < table->size; row++) {  // revisar si esto es necesario
        table->values[row] = NULL;
    }
    return table;
}

/** Inserta un elemento en la tabla de hash, retorna 0 si el elemento ya existía */
int table_insert(HashTable* table, Estado* state) {
    if (in_table(table, state)) {
        // false
        return 0;
    }
    uint32_t pos = state->hash % table->size;
    Estado* current = table->values[pos];
    // insertamos state
    // printf("\n state: %llu \n ", state->hash);
    if (current) {
        // printf("initial current: %llu \n ", current->hash);
        while (current->next) {
            // printf("test-%llu \n", current->hash);
            current = current->next;
        }

        current->next = state;
        // printf("test2-%llu-%llu-%llu\n", current->hash, current->next->hash, state->hash);
        
    }
    else {
        // printf("NP\n");
        table->values[pos] = state;
    }
    table->count += 1;
    // CHECK COUNT/SIZE
    if (table->count * 10 > 7 * table->size) {
        // true but need to migrate
        return -1;
    }
    // true
    return 1;
}

/** Inserta un elemento en la tabla de hash, retorna 0 si el elemento ya existía */
void table_migrate_insert(HashTable* table, Estado* state) {
    uint32_t pos = state->hash % table->size;
    Estado* current = table->values[pos];
    // insertamos state
    // printf("f4 state: %llu\n", state->hash);
    if (current) {
        while (current->next) {
            // printf("f5-%llu-%llu \n", current->hash, current->next->hash);
            current = current->next;
        }
        current->next = state;
    }
    else {
        table->values[pos] = state;
    }
    table->count += 1;
}

/** next hashtable size */
uint32_t new_size(uint32_t size) {
    uint32_t new = (2 * size) + 1;
    return new;
}

/** recursive insert to new table */
void insert_new_table(HashTable* new_table, Estado* state){
    if (state) {
        insert_new_table(new_table, state->next);
        // printf("f3\n");
        state->next = NULL;
        table_migrate_insert(new_table, state);
    }
}


/** migrate table */
HashTable* table_migrate(HashTable* table) {
    HashTable* new_table = table_init(new_size(table->size));
    for (int row = 0; row < table->size; row++) {
        // printf("f1: %d\n", row);
        Estado* current = table->values[row];
        if (table->values[row]) {
            //printf("f2\n");
            insert_new_table(new_table, current);
        }
    }
    // no liberamos los estados pues están en la nueva tabla
    free(table->values);
    free(table);
    return new_table;
}

/** Libera la memoria de la tabla */
void table_destroy(HashTable* table) {
    // liberamos cada puntero a un posible estado
    for (int row = 0; row < table->size; row++) {
        if (table->values[row]) state_destroy(table->values[row]);
    } 
    // liberamos array de valores
    //printf("after states \n");
    free(table->values);
    
    free(table);
}
