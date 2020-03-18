from collections import deque

while True:
    try:
        a = [int(x) for x in input().split()]
        if len(a) == 1:
            break
        N, F = a
        if F == 1 or N == 1:
            for i in range(N - 1):
                input()
            input()
            print(0)
            continue
        subir = {i: (0, 0) for i in range(1, N + 2)}
        hijos_directos = {i: set() for i in range(1, N + 2)}
        hijo_que_quiere_subir = {i: False for i in range(1, N + 2)}
        recursion_levels = {i: 0 for i in range(1, N + 2)}

        for _ in range(N - 1):
            a, b, c = (int(x) for x in input().split())
            subir[b] = (a, c)
            hijos_directos[a].add(b)

        # set rec_levels
        fila = deque([1, 0])
        max_rec_level = 1
        last_cero = False
        while fila:
            #print(fila)
            nodo = fila.popleft()
            if nodo == 0:
                if last_cero:
                    break
                last_cero = True
                max_rec_level += 1
                fila.append(0)
            else:
                last_cero = False
                fila.extend(list(hijos_directos[nodo]))
                recursion_levels[nodo] = max_rec_level

        amigos_rec_levels = {i:  set() for i in range(1, max_rec_level + 1)}
        for amigo in (int(x) for x in input().split()):
            try :
                amigos_rec_levels[recursion_levels[amigo]].add(amigo)
            except:
                print(amigo)
                print(recursion_levels)
                print(recursion_levels[amigo])



        suma = 0

        while max_rec_level > 1:

            for nodo in amigos_rec_levels[max_rec_level]:
                padre, min_subir = subir[nodo]
                amigos_rec_levels[max_rec_level - 1].discard(padre)
                ## lo mismo que arriba
                nodo_2 = hijo_que_quiere_subir[padre]
                if nodo_2:
                    min_subir_2 = subir[nodo_2][1]
                    if min_subir < min_subir_2:
                        suma += min_subir
                    else:
                        suma += min_subir_2
                        hijo_que_quiere_subir[padre] = nodo
                        amigos_rec_levels[max_rec_level - 1].discard(nodo_2)
                        amigos_rec_levels[max_rec_level - 1].add(nodo)
                else:
                    hijo_que_quiere_subir[padre] = nodo
                    amigos_rec_levels[max_rec_level - 1].add(nodo)

            for nodo in amigos_rec_levels[max_rec_level]:
                if nodo in amigos_rec_levels[max_rec_level -1]:
                    padre, min_subir = subir[nodo]
                    subir[nodo] = (subir[padre][0], subir[padre][1] + min_subir)

            max_rec_level -= 1

        print(suma)
    except:
        break

# with open("input.txt", "w") as file:
#     max_N = 10 ** 5
#     file.write(f"{max_N} {max_N}\n")
#     for i in range(1, max_N):
#         file.write(f"{i} {i + 1} {1}\n")
#     for i in range(1, max_N):
#         file.write(f"{i} ")
#     file.write(f"{max_N}\n")
#
#     file.write(f"{max_N} {max_N}\n")
#     for i in range(1, max_N):
#         file.write(f"{1} {i + 1} {1}\n")
#     for i in range(1, max_N):
#         file.write(f"{i} ")
#     file.write(f"{max_N}\n")
#     file.write(f"0\n")
