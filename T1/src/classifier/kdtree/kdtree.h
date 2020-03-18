#pragma once
#include <stdbool.h>
#include "../data/data.h"
#include "../heap/heap.h"

/** Estructura de un kdtree */
typedef struct kdtree
{
  Vector** vectors;
  double max_x;
  double max_y;
  double min_x;
  double min_y;
  bool vertical_cut;
  bool leaf;
  int begin;
  int end;
  struct kdtree* left_son;
  struct kdtree* right_son;

} KDTree;

/** Determina si el circulo definido por cx, cy, r choca con la caja */
bool collision(double cx, double cy, double r, double mx, double my, double Mx, double My);

/** Funcion que crea un kdtree a partir de los datos de entrenamiento */
KDTree* kd_init(Data* train);

/** Funcion que busca los k vecinos mas cercanos a objective
en el kd y los guarda en neighbours */
void knn(Vector** neighbours, KDTree* kd, Data* train_data, int k, Vector* objective);

/** Libera la memoria del kdtree */
void kd_destroy(KDTree* kd);
