from collections import deque
while True:
    M, N, p, q, x1, y1, x2, y2 = [int(x) for x in input().split()]
    if x1 == 0: break
    if x1 == x2 and y1 == y2:
        print(0)
        continue

    distancias = {(x1, y1): 0}
    padres = {(x1, y1): None}
    por_revisar = deque()
    por_revisar.append((x1, y1))

    win = False
    while por_revisar:
        punto = por_revisar.popleft()
        if punto == (x2, y2):
            win = True
            break
        # agregamos nuevas posibilidades
        x, y = punto
        candidatos = [(x + p, y + q), (x + p, y - q), (x - p, y + q), (x - p, y - q),
                      (x + q, y + p), (x + q, y - p), (x - q, y + p), (x - q, y - p)]
        for x_, y_ in candidatos:
            if x_ < 0 or x_ > M or y_ < 0 or y_ > N:
                continue
            elif (x_, y_) in padres:
                continue
            else:
                padres[(x_, y_)] = punto
                distancias[(x_, y_)] = distancias[padres[(x_, y_)]] + 1
                por_revisar.append((x_, y_))
    if win:
        print(distancias[punto])
    else:
        print(-1)
