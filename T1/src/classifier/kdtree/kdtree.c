#include <stdlib.h>
#include <math.h>
#include "kdtree.h"


/** Retorn true si el segmento de recta choca circulo */
static bool collision_line(double cx, double cy, double r, double x, double my, double My)
{
  /*
  Esta funcion es para ayudarlos, no es importante entender su funcionamiento
  */

  // Veo si la recta intersecta al circulo
  double det = pow(r, 2) - pow(cx - x, 2);
  if (det < 0) return false;

  // Calculo los puntos de interseccion
  double y0 = cy + sqrt(det);
  double y1 = cy - sqrt(det);

  // Veo si alguno esta entre my y My
  if ((my <= y0 && y0 <= My) || (my <= y1 && y1 <= My)) return true;

  // Retorno false
  return false;
}


/** Retorna true si el circulo definido por el heap toca al kdtree */
bool collision(double cx, double cy, double r, double mx, double my, double Mx, double My)
{
  /*
  Esta funcion es para ayudarlos, no es importante entender su funcionamiento
  */

  // Caso 1: El centro del circulo esta dentro de la caja
  if (cx >= mx && cx <= Mx && cy >= my && cy <= My) return true;

  // Caso 2: El centro de la caja esta dentro del circulo
  // Calculo centro de la caja
  double bx = (Mx + mx) / 2;
  double by = (My + my) / 2;

  // Veo si esta en el circulo
  if (pow(bx - cx, 2) + pow(by - cy, 2) <= pow(r, 2)) return true;

  // Caso 3: Circunferencia choca con algun borde de la caja
  // Testeo con cada uno de los bordes
  if (collision_line(cx, cy, r, mx, my, My)) return true;
  if (collision_line(cx, cy, r, Mx, my, My)) return true;
  if (collision_line(cy, cx, r, my, mx, Mx)) return true;
  if (collision_line(cy, cx, r, My, mx, Mx)) return true;

  // Caso 5: No intersectan
  return false;
}


/** Se dejan los elementos menores que el pivote a la izquierda y mayores a la derecha */
static int partition(Vector** array, int inicio, int final, int cord) {
  // source: https://github.com/IIC2133-PUC/2019-1/blob/master/
  // Clases/3.%20Estadi%CC%81sticas%20de%20orden%20%26%20Quicksort.pdf
  int ret = inicio;
  Vector* aux;
  // posible mejora, escoger buen pivote como mediana del medio y no el último
  for (int k = inicio; k < final;  k++) {
    if (array[k]->pos[cord] < array[final]->pos[cord]){
      aux = array[ret];
      array[ret] = array[k];
      array[k] = aux; 
      ret ++;
    }
  }
  // devolvemos al pivote a su posición
  aux = array[ret];
  array[ret] = array[final];
  array[final] = aux; 
  // retornamos la posición del pivote
  return ret;
}


/** buscamos la mediana del subarray que empieza en i y termina en f */
static int median(Vector** array, int i, int f, int cord) {

  int current = partition(array, i, f, cord);
  int med = (f+i)/2;
  // ordenamos de tal forma que la mediana quede en la posición "med" del array
  while (current != med) {
    if (current < med) {
      i = current + 1;
    }   
    else {
      f = current - 1;
    }
    current = partition(array, i, f, cord);
  }
  return med;
}


/** creación recursiva del kdtree (k=2) */
KDTree* init_tree(Vector** vectors, int begin, int end) {
  
  // reservamos memoria
  KDTree* kdtree = malloc(sizeof(KDTree));
  
  // seteo inicial
  // posiciones array y puntero a array completo
  kdtree->begin = begin;
  kdtree->end = end;
  kdtree->vectors = vectors;  // si usamos data no es necesario
  // dimensiones en el plano
  kdtree->min_x = vectors[begin]->pos[0];
  kdtree->max_x = vectors[begin]->pos[0];
  kdtree->min_y = vectors[begin]->pos[1];
  kdtree->max_y = vectors[begin]->pos[1];

  // seteo tamaño caja
  for (int i = begin + 1; i <= end; i++){
    if (vectors[i]->pos[0] < kdtree->min_x) kdtree->min_x = vectors[i]->pos[0];
    else if (vectors[i]->pos[0] > kdtree->max_x) kdtree->max_x = vectors[i]->pos[0];
    if (vectors[i]->pos[1] < kdtree->min_y) kdtree->min_y = vectors[i]->pos[1];
    else if (vectors[i]->pos[1] > kdtree->max_y) kdtree->max_y = vectors[i]->pos[1];
  }

  // revisamos si es una hoja, cantidad de vectores es un valor completamente arbitrario
  if (end - begin < 9) kdtree->leaf = true;
  else kdtree->leaf = false;

  // si es una hoja retornamos sin hijos
  if (kdtree->leaf) {
    kdtree->left_son = NULL;
    kdtree->right_son = NULL; 
    kdtree->vertical_cut = NULL;
    return kdtree;
  }

  // definimos por donde nos conviene cortar para que la figura sea más cuadrada
  if (kdtree->max_y - kdtree->min_y < kdtree->max_x - kdtree->min_x) kdtree->vertical_cut = true;
  else kdtree->vertical_cut = false;

  // buscamos la mediana según el corte
  int med_pos;
  if (kdtree->vertical_cut) {
    med_pos = median(vectors, begin, end, 0);
  }
  else {
    med_pos = median(vectors, begin, end, 1);
  }

  kdtree->left_son = malloc(sizeof(KDTree));
  kdtree->right_son = malloc(sizeof(KDTree));
 
  kdtree->left_son = init_tree(vectors, begin, med_pos);
  kdtree->right_son = init_tree(vectors, med_pos + 1, end);

  return kdtree;
}


void insertar(KDTree* kd, Heap* heap, Vector* obj) {
  if (collision(obj->pos[0], obj->pos[1], heap->array[0]->key, kd->min_x, kd->min_y, kd->max_x, kd->max_y)) {
    // si no es una hoja revisamos con que hijos existe colisión
    if (!kd->leaf) {
      if (kd->left_son->max_x > obj->pos[0] && obj->pos[0] > kd->left_son->min_x
          && kd->left_son->max_y > obj->pos[1] && obj->pos[1] > kd->left_son->min_y) {
        insertar(kd->left_son, heap, obj);
        insertar(kd->right_son, heap, obj);
      }
      else if (kd->right_son->max_x > obj->pos[0] && obj->pos[0] > kd->right_son->min_x
          && kd->right_son->max_y > obj->pos[1] && obj->pos[1] > kd->right_son->min_y) {
        insertar(kd->right_son, heap, obj);
        insertar(kd->left_son, heap, obj);
      }
      else {
        // buscamos el más cercano
        if (kd->vertical_cut) { // left_son a la izquierda
          if (obj->pos[0] < kd->left_son->min_x) { // muy a la izquierda
            insertar(kd->left_son, heap, obj);
            insertar(kd->right_son, heap, obj);
          }
          else if (obj->pos[0] > kd->right_son->max_x) { // muy a la derecha
            insertar(kd->right_son, heap, obj);
            insertar(kd->left_son, heap, obj);
          }
          else { // entre las cajas
            if (kd->right_son->min_x - obj->pos[0] > obj->pos[0] - kd->left_son->max_x) { // caja izquierda + cerca
              insertar(kd->left_son, heap, obj);
              insertar(kd->right_son, heap, obj);
            }
            else { // derecha igual o + cerca
              insertar(kd->right_son, heap, obj);
              insertar(kd->left_son, heap, obj);
            }
          }
        }
        else { // left_son arriba
          if (obj->pos[1] < kd->left_son->min_y) { // muy arriba
            insertar(kd->left_son, heap, obj);
            insertar(kd->right_son, heap, obj);
          }
          else if (obj->pos[1] > kd->right_son->max_y) { // muy abajo
            insertar(kd->right_son, heap, obj);
            insertar(kd->left_son, heap, obj);
          }
          else { // entre las cajas
            if (kd->right_son->min_y - obj->pos[1] > obj->pos[1] - kd->left_son->max_y) { // caja izquierda + cerca
              insertar(kd->left_son, heap, obj);
              insertar(kd->right_son, heap, obj);
            }
            else { // derecha igual o + cerca
              insertar(kd->right_son, heap, obj);
              insertar(kd->left_son, heap, obj);
            }
          }
        }
      }
    }
    // si es una hoja comparamos con los vectores que contenga
    else {
      for (int i = kd->begin; i <= kd->end; i++) {
        // insertar al heap
        Object* actual = obj_init(distance(obj, kd->vectors[i]), kd->vectors[i]);

        heap_insert(heap, actual);
      }
    }
  }
}


/** Funcion que crea un kdtree a partir de los datos de entrenamiento */
KDTree* kd_init(Data* train) {
  KDTree* kdtree = init_tree(train->vectors, 0, train->count - 1);
  // wubbalubbadubdub
  return kdtree;
}


/** Busca los vecinos cercanos y los guarda en neighbours */
void knn(Vector** neighbours, KDTree* kd, Data* train_data, int k, Vector* objective) {

  // heap tamaño k
  Heap* heap = heap_init(k);
  // insertamos nodo infinito
  Object* actual = obj_init(INFINITY, NULL);
  heap_insert(heap, actual);

  // insertamos el resto al heap
  insertar(kd, heap, objective);
  
  /** como al realizar la inserción sabemos que quedarán los k vecinos más cercanos en el heap
      solo necesitamos traspasarlos al vector de neighbours  */
  for(int i = 0; i < k; i++) {
    neighbours[i] = heap_pop(heap)->value;
  }
  // liberamos memoria
  heap_destroy(heap);
}


/** Libera la memoria del kdtree */
void kd_destroy(KDTree* kd) {
  if (!(kd->leaf)) {
    kd_destroy(kd->left_son);
    kd_destroy(kd->right_son);
  }
  free(kd);
}
