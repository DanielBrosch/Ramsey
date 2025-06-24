import itertools

template = "5 0  2 2 2 2    2 2 2  2 2   2"
template = "6 0  2 2 2 2 2   2 2 2 2    2 2 2  2 2   2"


split = [ int(x) for x in template.split() ]

#print(split)

# number of vertices
v = split[0]

# empty matrix
AM = [ [0]*v for _ in range(v) ]

#print(AM)

splitID = 2
for x in range(v):
    for y in range(x+1,v):
        AM[x][y] =split[splitID]
        AM[y][x] =split[splitID]
        splitID += 1
#print(AM)



def try_colorings(v, AM, v_order, colors):

    if len(colors) == v:
        graph = [v,0]
        for x in range(v):
            for y in range(x+1,v):
                new_color = AM[v_order[x]][v_order[y]]
                if new_color == 2:
                    new_color = colors[x]
                graph.append(new_color)
        print(' '.join(map(str, graph)))
        return


    max_color = max(colors)+1
    for color in range(2,max_color+1):
        try_colorings(v, AM, v_order, colors + [color])

for v_order in itertools.permutations(list(range(v))):
    #print(v_order)
    
    try_colorings(v, AM, v_order, [2])
    
