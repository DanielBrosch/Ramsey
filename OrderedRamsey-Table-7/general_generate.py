from itertools import product
from math import sqrt
import sys
# Read flag from command line
flag = sys.argv[1]

# Determine where the non-edges are in the flag
nonedges = list()
count = 0
for char in flag:
    if char == '0':
        nonedges.append(count)
        count += 1
    elif char == '1':
        count += 1

for word in product([2,3], repeat=len(nonedges)):
    num_vertices = int((1 + sqrt(1 + 8 * count))/2)
    line_a = f'{num_vertices} 0 '
    line_b = f'{num_vertices} 0 '
    curr = 0
    for i in range(count):
        if i in nonedges:
            line_a += f'{word[curr]} '
            line_b += f'{word[curr]} '
            curr += 1
        else:
            line_a += '2 '
            line_b += '3 '
    print(line_a)
    print(line_b)
