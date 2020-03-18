#include <stdlib.h>
#include <stdio.h>
#include "heap.h"

// código basado en taller 3, se altera para que sea un max heap
// source: https://github.com/IIC2133-PUC/2019-1/tree/master/Talleres/Taller%203/src/heap

////////////////////////////////////////////////////////////////////////////////
//                  Funciones utiles para no escribir tanto                   //
////////////////////////////////////////////////////////////////////////////////

/** Retorna la key del objeto de la posicion dada */
static double key(Heap* heap, int i) {
  return heap -> array[i] -> key;
}

/** Hace swap de los elementos indicados */
static void swap(Heap* heap, int i1, int i2) {
  Object* aux = heap -> array[i1];
  heap -> array[i1] = heap -> array[i2];
  heap -> array[i2] = aux;
}

////////////////////////////////////////////////////////////////////////////////
//             Funciones para conservar las propiedades del heap              //
////////////////////////////////////////////////////////////////////////////////

/** Mueve el elemento hacia arriba hasta que este en su posicion correcta */
static void sift_up(Heap* heap, int pos) {

  // Si ya estoy en la raiz, termino
  if (!pos) return;

  // Posicion del padre
  int father = (pos - 1) / 2;

  // Si el padre es menor hago swap y sigo iterando
  if (key(heap, father) < key(heap, pos)) {
    swap(heap, pos, father);
    sift_up(heap, father);
  }
}

/** Mueve el elemento hacia abajo hasta que este en su posicion correcta */
static void sift_down(Heap* heap, int pos) {
  // Posicion de los hijos
  int left = pos * 2 + 1;
  int right = left + 1;

  // Si estoy en una hoja termino
  if (heap -> count <= left) return;

  // Veo cual de los hijos es mayor
  int bigger;
  if (heap -> count == right || key(heap, left) > key(heap, right)) {
    bigger = left;
  }
  else {
    bigger = right;
  }

  // Si la key actual es menor a la del hijo menor, hago swap y sigo bajando
  if (key(heap, pos) < key(heap, bigger))
  {
    swap(heap, pos, bigger);
    sift_down(heap, bigger);
  }
}

////////////////////////////////////////////////////////////////////////////////
//                 Funciones de uso publico del heap                          //
////////////////////////////////////////////////////////////////////////////////

/** Inicializa un heap vacio */
Heap* heap_init(int size) {
  Heap* heap = malloc(sizeof(Heap));

  heap -> count = 0;
  heap -> size = size;
  heap -> array = calloc(heap -> size, sizeof(Object*));  

  return heap;
}

/** Inserta un elemento en el heap */
void heap_insert(Heap* heap, Object* obj) {

  // cuando el heap está lleno, agregamos un elemento sólo si es menor que el máximo 
  if (heap -> count == heap -> size) {
    // si el vector está a menor distancia eliminamos la cabeza y lo agregamos
    if (heap->array[0]->key > obj->key) {
      Object* bigger = heap_pop(heap); // sólo necesitamos eliminarlo en este caso
      free(bigger);
      // insertamos el elemento
      heap -> array[heap -> count] = obj;
      heap -> count += 1;
      sift_up(heap, heap -> count - 1);
    }
    else {
      // si no lo usamos, liberamos altiro la memoria
      free(obj);
    }
  }
  else {
    // Inserto el elemento al final del heap
    heap -> array[heap -> count] = obj;
    heap -> count += 1;
    // Lo desplazo hasta su posicion correcta del heap
    sift_up(heap, heap -> count - 1);
  }
}

/** Elimina la cabeza del heap y la retorna */
Object* heap_pop(Heap* heap)
{
  // Si el heap esta vacio retorno NULL
  if (!heap -> count) return NULL;
  
  // Obtengo la cabeza del heap
  Object* obj = heap -> array[0];
  // Disminuyo el contador de elementos en 1
  heap -> count -= 1;

  // Si quedan elementos, remplazo la cabeza por el ultimo elemento
  if (heap -> count > 0)
  {
    heap -> array[0] = heap -> array[heap -> count];
    heap -> array[heap -> count] = NULL;
    // Restauro la propiedad de heap
    sift_down(heap, 0);
  }
  else
  {
    // Si no quedan elementos, seteo la cabeza en NULL
    heap -> array[0] = NULL;
  }
  // Retorno el elemento que elimine
  return obj;
}

/** Destruye el heap */
void heap_destroy(Heap* heap)
{
  for (int i = 0; i < heap -> size; i++) {
    free(heap->array[i]);
  }

  free(heap->array);
  free(heap);
}

/** Ordena un arreglo de objetos usando un heap */
void heap_sort(Object** array, int size) {
  Heap* heap = heap_init(size);
  for(int i = 0; i < size; i++){
    heap_insert(heap, array[i]);
  }
  for(int i = 0; i < size; i++){
    array[i] = heap_pop(heap);
  }
  heap_destroy(heap);
}
