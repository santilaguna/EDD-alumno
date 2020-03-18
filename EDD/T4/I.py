from collections import deque
while True:
    N = int(input())
    if N == 0: break

    alturas = tuple(tuple(int(x) for x in input().split()) for i in range(N))

    pos_inicial = tuple(int(x) for x in input().split())
    if pos_inicial == (N - 1, N - 1):
        print(0)
        continue

    distancias = {pos_inicial: 0}
    por_revisar = deque()
    por_revisar.append(pos_inicial)

    while por_revisar:
        punto = por_revisar.popleft()
        if punto == (N - 1, N - 1):
            break
        y, x = punto
        #print(punto, alturas[y][x])
        candidatos = ((y + 1, x), (y - 1, x), (y, x - 1), (y, x + 1))
        for y_, x_ in candidatos:
            if y_ < 0 or x_ < 0 or x_ >= N or y_ >= N:
                continue
            elif (y_, x_) in distancias:
                continue
            elif alturas[y][x] - alturas[y_][x_] > 5:
                continue
            elif alturas[y_][x_] - alturas[y][x] > 2:
                continue
            else:
                distancias[(y_, x_)] = distancias[punto] + 1
                por_revisar.append((y_, x_))

    print(distancias[punto])