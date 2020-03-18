from collections import deque

def direccion(estado_incial, estado_final):
    if estado_incial[1] < estado_final[1]:
        return "L"
    elif estado_incial[1] > estado_final[1]:
        return "R"
    if estado_incial[0] < estado_final[0]:
        return "U"
    else:
        return "D"


while True:
    N, M = [int(x) for x in input().split()]
    if N == 0 and M == 0:
        break
    tablero = tuple(input() for f in range(N))
    for fila in range(N):
        for columna in range(M):
            if tablero[fila][columna] == "C":
                estado_caja = (fila, columna, "P")
                # P parado, L su otra parte está a la izquierda, D su otra parte está hacia abajo, etc.
            elif tablero[fila][columna] == "E":
                pos_final = (fila, columna, "P")

    distancias = {estado_caja: 0}
    padres = {estado_caja: None}
    por_revisar = deque()
    por_revisar.append(estado_caja)


    win = False
    #print(tablero)
    while por_revisar:
        #print(por_revisar)
        punto = por_revisar.popleft()
        if punto == pos_final:
            win = True
            break
        # agregamos nuevas posibilidades
        fila, columna, estado = punto
        if estado == "P":
            candidatos = [(fila - 2, columna, "D"), (fila + 2, columna, "U"),
                          (fila, columna + 2, "L"), (fila, columna - 2, "R")]
        elif estado == "L":
            candidatos = [(fila - 1, columna, "L"), (fila + 1, columna, "L"),
                          (fila, columna - 2, "P"), (fila, columna + 1, "P")]
        elif estado == "R":
            candidatos = [(fila - 1, columna, "R"), (fila + 1, columna, "R"),
                          (fila, columna + 2, "P"), (fila, columna - 1, "P")]
        elif estado == "U":
            candidatos = [(fila, columna - 1, "U"), (fila, columna + 1, "U"),
                          (fila + 1, columna, "P"), (fila - 2, columna, "P")]
        else:  # down
            candidatos = [(fila, columna - 1, "D"), (fila, columna + 1, "D"),
                          (fila - 1, columna, "P"), (fila + 2, columna, "P")]

        for cand in candidatos:
            if cand in padres:
                continue
            padres[cand] = punto
            y_, x_, p_ = cand
            if x_ < 0 or x_ >= M or y_ < 0 or y_ >= N:
                #print(cand, "A")
                continue
            elif tablero[y_][x_] == "#":
                #print(cand, "B")
                continue
            elif p_ != "P":
                #print(cand, "C")
                if p_ == "U":
                    if tablero[y_ - 1][x_] == "#":
                        continue
                elif p_ == "D":
                    if tablero[y_ + 1][x_] == "#":
                        continue
                elif p_ == "L":
                    if tablero[y_][x_ - 1] == "#":
                        continue
                else:  # right
                    if tablero[y_][x_ + 1] == "#":
                        continue
            #print(cand, "D")
            padres[cand] = punto
            distancias[cand] = distancias[punto] + 1
            por_revisar.append(cand)
    if win:
        print(distancias[punto])
        por_revisar = []
        parent = padres[punto]
        por_revisar.append(direccion(punto, parent))
        while parent != None:
            #print(por_revisar)
            if padres[parent]:
                por_revisar.append(direccion(parent, padres[parent]))
            parent = padres[parent]
        print("".join(reversed(por_revisar)))
    else:
        print(-1)
