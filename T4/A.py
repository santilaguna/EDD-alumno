while True:
    N = int(input())
    if N == 0: break
    if N == 1:
        print(N)
        continue
    fila_abuela = [int(x) for x in input().split()]
    fila_arriba = [int(x) for x in input().split()]
    fila_arriba[0] += fila_abuela[0]
    fila_arriba[1] += fila_abuela[0]
    if N == 2:
        print(max(fila_arriba))
        continue
    for largo_fila in range(3, N + 1):
        nuevos = [int(x) for x in input().split()]
        for i in range(largo_fila):
            if i == 0:
                nuevos[0] += fila_arriba[0]
            elif i == largo_fila - 1:
                nuevos[i] += fila_arriba[i - 1]
            else:
                mejor = max(fila_arriba[i], fila_arriba[i-1], fila_abuela[i-1])
                nuevos[i] += mejor
        fila_abuela = tuple(x for x in fila_arriba)
        fila_arriba = tuple(x for x in nuevos)
    print(max(nuevos))
