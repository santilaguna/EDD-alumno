from math import inf

# As seen at: https://es.wikibooks.org/wiki/Optimizaci%C3%B3n_del_Producto_de_Matrices

N = int(input())
sizes = tuple(int(input()) for _ in range(N))
memo = {(i, i): 0 for i in range(1, N + 1)}

min_ = inf
for diagonal in range(1, N):
    for row in range(1, N - diagonal):
        ## mínimo dp
        # col >= row, row es el puesto de la matriz de la izquierda y col de la derecha (row + diagonal)
        current_min = inf
        for k in range(row, row + diagonal):
            #print(row, k, row + diagonal, memo)
            val = memo[(row, k)] + memo[(k + 1, row + diagonal)] + (sizes[row - 1] * sizes[k] * sizes[row + diagonal])
            if val < current_min:
                current_min = val
        memo[(row, row + diagonal)] = current_min
#print(memo)
print(memo[(1, N - 1)])
