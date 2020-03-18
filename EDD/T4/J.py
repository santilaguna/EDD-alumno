

# def make_hashs(word, sub_size):
#     ret = set()
#     current_hash = sum(ord(word[i]) * (i + 1) for i in range(sub_size))
#     current_suma = sum(ord(word[i]) for i in range(sub_size))
#     ret.add(current_hash)
#     for initial_pos in range(1, len(word) - sub_size + 1):
#         current_hash -= current_suma
#         current_suma -= ord(word[initial_pos - 1])
#         current_suma += ord(word[initial_pos + sub_size - 1])
#         current_hash += (ord(word[initial_pos + sub_size - 1]) * sub_size)
#         ret.add(current_hash)
#     return ret


while True:
    N = int(input())
    if N == 0: break
    words = tuple(input() for _ in range(N))

    min_ = 0
    max_ = min(len(word) for word in words)
    while True:
        if min_ == max_:
            break
        sub_size = max(min_ + 1, min_ + (max_ - min_) // 2)
        #sets = tuple({word[i:i + sub_size] for i in range(len(word) - sub_size + 1)} for word in words)
        win = False
        #initial = make_hashs(words[0], sub_size)
        #sets = tuple(make_hashs(words[j], sub_size) for j in range(1, N))
        if any(
                all(sub in word for word in words[1:]) for sub in
                {words[0][i:i + sub_size] for i in range(len(words[0]) - sub_size + 1)}):
            win = True
        if win:
            min_ = sub_size
        else:
            max_ = sub_size - 1
    print(min_)
