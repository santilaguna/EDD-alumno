#include <stdio.h>
#include <stdlib.h>
#include "kdtree/kdtree.h"
#include <time.h> 

int main(int argc, char *argv[])
{
  if (argc != 5)
  {
    printf("Modo de uso: ./knn <train.txt> <test.txt> <k> <l>\n");
    printf("Donde <test.txt> tiene los puntos a clasificar y sus labels\n");
    printf("<train.txt> tiene los puntos de entrenamiento y sus labels\n");
    printf("<k> es el numero de vecinos a revisar\n");
    printf("<l> es el numero de labels distintos en los archivos\n");
    return 0;
  }

  //////////////////////////////////////////////////////////////////////////////
  //                            Lectura de input                              //
  //////////////////////////////////////////////////////////////////////////////

  // Numero de vecinos a revisar
  int k = atoi(argv[3]);
  // Numero de labels
  N_LABELS = atoi(argv[4]);

  // Abro el archivo de train
  FILE* train_f = fopen(argv[1], "r");
  // Leo el archivo
  Data* train_data = data_init(train_f);
  // Cierro el archivo
  fclose(train_f);

  // Abro el archivo de test
  FILE* test_f = fopen(argv[2], "r");
  // Leo el archivo
  Data* test_data = data_init(test_f);
  // Cierro el archivo
  fclose(test_f);

  //////////////////////////////////////////////////////////////////////////////
  //                      Busqueda de vecinos cercanos                        //
  //////////////////////////////////////////////////////////////////////////////
  
  // Inicializo el kdtree
  
  KDTree* kd = kd_init(train_data);
  int initial = test_data->count;
  for(int c = 100; c <= 10000; c+=100){
  test_data->count = c;
  clock_t t; 
	t = clock();
  // Arreglo de vecinos mas cercanos
  Vector** neighbours = malloc(sizeof(Vector*) * k);
  // Itero por los vectores a clasificar
  for (int o = 0; o < test_data -> count; o++)
   {
    // Vector a clasificar
    Vector* objective = test_data -> vectors[o];

    
    // Obtengo los vecinos cercanos y los guardo en el arreglo neighbours
    knn(neighbours, kd, train_data, k, objective);
    
    // Imprimo distancias a vecinos cercanos
	//for (int v = 0; v < k; v++)
	//{
	//	printf("%lf ", distance(neighbours[v], objective));
	//}
	//printf("\n");
   }
  t = clock() - t; 
  double time = ((double)t)/CLOCKS_PER_SEC;
  printf("%f\n", time);
  // Libero el arreglo de vecinos
  free(neighbours);
  }
  // Libero kdtree
  kd_destroy(kd);
  printf("\n");
  //////////////////////////////////////////////////////////////////////////////
  //                        Termino del programa                              //
  //////////////////////////////////////////////////////////////////////////////

  // Libero los datos
  test_data->count = initial;
  data_destroy(train_data);
  data_destroy(test_data);

  return 0;
}
