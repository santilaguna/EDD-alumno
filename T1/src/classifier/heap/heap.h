#pragma once
#include "object.h"

// código basado en taller 3, se modifica para que sea un max heap
// source: https://github.com/IIC2133-PUC/2019-1/tree/master/Talleres/Taller%203/src/heap

/** Estructura de un max heap */
typedef struct heap
{
  /** Numero de elementos */
  int count;
  /** Tamanio del array */
  int size;
  /** Arreglo de objetos */
  Object** array;
} Heap;

/** Inicializa un heap vacio */
Heap* heap_init(int size);

/** Inserta un elemento en el heap */
void heap_insert(Heap* heap, Object* obj);

/** Elimina la cabeza del heap y la retorna */
Object* heap_pop(Heap* heap);

/** Ordena un arreglo de objetos usando un heap */
void heap_sort(Object** array, int size);

/** Destruye el heap */
void heap_destroy(Heap* heap);
