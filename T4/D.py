# Problema D contest EDD

from heapq import heappush, heappop

while True:
    cantidad_fuentes, cantidad_estanques, cantidad_tuberias = [int(x) for x in input().split()]
    if cantidad_fuentes == 0 and cantidad_estanques == 0 and cantidad_tuberias == 0:
        break
    flujo = 0
    MAX_CAPACITY = (2 ** 64) - 1
    nodo_abstracto = cantidad_fuentes + cantidad_estanques
    aristas = {i: {} for i in range(nodo_abstracto + 1)}
    for i in range(cantidad_tuberias):
        nodo_1, nodo_2, capacidad = [int(x) for x in input().split()]
        aristas[nodo_2][nodo_1] = capacidad
        aristas[nodo_1][nodo_2] = capacidad

    for estanque in range(cantidad_fuentes, cantidad_estanques + cantidad_fuentes):  # a, a + b
        aristas[nodo_abstracto][estanque] = MAX_CAPACITY

    distancias_mst = [(-MAX_CAPACITY, i, nodo_abstracto) for i in range(cantidad_fuentes,
                                                                        cantidad_fuentes + cantidad_estanques)]
    #distancias_dict = {1: -MAX_CAPACITY}

    current_mst = {nodo_abstracto}

    while distancias_mst and len(current_mst) < cantidad_estanques + cantidad_fuentes + 1:
        #print(distancias_mst)
        n_capacity, nodo, parent = heappop(distancias_mst)
        if nodo in current_mst:
            continue
        #print(current_mst, flujo)
        flujo -= n_capacity
        current_mst.add(nodo)
        #print("vecinos: ", nodo, aristas[nodo])
        #print()
        for vecino in aristas[nodo]:
            if vecino not in current_mst:
                heappush(distancias_mst, (-aristas[nodo][vecino], vecino, nodo))


    print(flujo - cantidad_estanques * MAX_CAPACITY)
