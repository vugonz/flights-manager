from random import shuffle as r
from itertools import permutations

abc = ['A', 'B', 'C', 'D', 'F']

lis = list(permutations(abc))
lis = [''.join(permutation) for permutation in lis]

r(lis)

with open('test.in', 'w') as f:
    for i in range(25):
        print(f"a {lis[i]} Country City", file=f)
    
    print("l ", end = "", file=f)
    for i in range(25):
        print(f"{lis[i]}", end = "", file=f) 
        if i != 25:
            print(" ", file=f, end = "")
        else:
            print("\nq", file=f)

