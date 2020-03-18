# Problema D contest EDD

from heapq import heappop, heapify

def get_representante(nodo, representantes):
    if representantes[nodo] == nodo:
        return nodo
    representantes[nodo] = get_representante(representantes[nodo], representantes)
    return representantes[nodo]


while True:
    cantidad_fuentes, cantidad_estanques, cantidad_tuberias = [int(x) for x in input().split()]
    if cantidad_fuentes == 0 and cantidad_estanques == 0 and cantidad_tuberias == 0:
        break
    flujo = 0

    aristas = [0 for _ in range(cantidad_tuberias)]
    for i in range(cantidad_tuberias):
        nodo_1, nodo_2, capacidad = [int(x) for x in input().split()]
        aristas[i] = (-capacidad, nodo_1, nodo_2)

    heapify(aristas)

    representantes = {i:i for i in range(cantidad_estanques + cantidad_fuentes)}
    estanques = {i for i in range(cantidad_fuentes, cantidad_fuentes + cantidad_estanques)}
    fuentes_en_mst = set()

    while aristas and len(fuentes_en_mst) < cantidad_fuentes:
        n_capacidad, nodo_1, nodo_2 = heappop(aristas)
        if nodo_1 in estanques and nodo_2 in estanques:
            continue
        if nodo_1 in estanques:
            rep_2 = get_representante(nodo_2, representantes)
            if rep_2 in estanques:
                continue
            else:
                representantes[rep_2] = nodo_1
                flujo -= n_capacidad
        elif nodo_2 in estanques:
            rep_1 =  get_representante(nodo_1, representantes)
            if rep_1 in estanques:
                continue
            else:
                representantes[rep_1] = nodo_2
                flujo -= n_capacidad
        else:
            rep_1 = get_representante(nodo_1, representantes)
            rep_2 = get_representante(nodo_2, representantes)
            if rep_1 != rep_2:
                if rep_1 in estanques:
                    if rep_2 in estanques:
                        continue
                    representantes[rep_2] = rep_1
                else:
                    representantes[rep_1] = rep_2
                flujo -= n_capacidad



    print(flujo)