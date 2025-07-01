
## Find if there exist a canonical coloring of K_n


using JuMP
using Graphs
using LinearAlgebra
using Combinatorics
using Colors
using GraphPlot
using Compose
# using GLPK
using IterTools
using Cairo, Fontconfig 
using Gurobi
using JLD2
using MosekTools

dir_prefix="UnorderedCanonicalRamsey-Table-2-4-5/Lower/"


if !isdir(dir_prefix*"LPmodels")
    mkdir(dir_prefix*"LPmodels")
end

if !isdir(dir_prefix*"feasible_graphs")
    mkdir(dir_prefix*"feasible_graphs")
end

if !isdir(dir_prefix*"feasible_graphs/plots")
    mkdir(dir_prefix*"feasible_graphs/plots")∏
end

if !isdir(dir_prefix*"feasible_graphs/adjacency_matrices")
    mkdir(dir_prefix*"feasible_graphs/adjacency_matrices")
end


##
function ord_edge(u,v)
    return Edge(min(u,v),max(u,v))
end

function color_from_difference(y, E, ind_edges, n)
    colors_draw = distinguishable_colors(100, [RGB(1,1,1), RGB(0,0,0)], dropseed=false)
    c = 1
    feasible_colors = [colors_draw[1] for e in E]
    colored_edges = []
    A = zeros(Int8, n, n)
    for e in E
        if e ∉ colored_edges
            feasible_colors[ind_edges[e]] = colors_draw[c+2]
            A[src(e), dst(e)] = c
            A[dst(e), src(e)] = c
            println("$e colored with color $c")
            push!(colored_edges,e)
            for f in E
                if f ∉ colored_edges
                    @error "double check whether it should be == 0 or == 1"
                    if ind_edges[e]>ind_edges[f] && y[e,f] == 0 || ind_edges[e]<ind_edges[f] && y[f,e] == 0
                        feasible_colors[ind_edges[f]] = colors_draw[c+2]
                        A[src(f), dst(f)] = c
                        A[dst(f), src(f)] = c
                        push!(colored_edges,f)
                        println("$f colored with color $c")
                    end
                end
            end
            c = c+1
        end
    end
    return A, feasible_colors, (c-1)
end



function get_graph(G)

    if G isa Number
        return complete_graph(G)

    elseif startswith(G, "P")
        t = parse(Int64,chop(G, head=1, tail=0))
        return path_graph(t)

    elseif startswith(G, "C")
        t = parse(Int64,chop(G, head=1, tail=0))
        return cycle_graph(t)
        
    elseif startswith(G, "K")
        if contains(G, ",")
            if endswith(G, "-")
                k,p = split(chop(G, head=1, tail=1), ",")
                k,p = parse(Int64,k), parse(Int64,p)
                H = complete_bipartite_graph(k,p)
                rem = rem_edge!(H, k,k+p)
                if rem==false
                    println("!!!! Problem with the K3,3- !!!")
                    return nothing
                end
                return H
            else
                k,p = split(chop(G, head=1, tail=0), ",")
                k,p = parse(Int64,k), parse(Int64,p)
                return complete_bipartite_graph(k,p)
            end
        else
            if endswith(G, "-")
                t = parse(Int64,chop(G, head=1, tail=1))
                H = complete_graph(t)
                rem_edge!(H, t-1,t)
                return H
            end
        end

    elseif startswith(G, "L")
        t=parse(Int64,chop(G, head=1, tail=0))
        return ladder_graph(3)

    else
        println("Other graphs not yet supported :'( ")
    end
end


##


function colorblind_canonical_ordering(GOrd, GRain, n, relaxation=false, max_colors=false, max_time=1200, save=true)

    println("Finding a coloring of K$n with no orderable $GOrd and no rainbow $GRain \n\n")

    G = complete_graph(n)
    V = vertices(G)
    E = edges(G)

    ind_edges = Dict(E .=> collect(1:binomial(n,2)))
    for e in E
        ind_edges[(reverse(e))] = ind_edges[(e)]
    end

    m = Model(Gurobi.Optimizer)
    set_optimizer_attribute(m, "TimeLimit", 300.0)

    # m = Model(Mosek.Optimizer)
    #set_optimizer_attribute(m, "MIO_MAX_TIME", 300.0)

    # set_optimizer_attribute(m, "TIME_LIMIT", max_time)

    if relaxation
        @variable(m, 1>=y[e1=E, e2=E; ind_edges[e1]>ind_edges[e2]]>=0)
    else
        @variable(m, y[e1=E, e2=E; ind_edges[e1]>ind_edges[e2]], Bin)
    end

    if max_colors
        colors = 1:(binomial(n,2)-1)
        # @variable(m, x[E, colors], Bin) 
        @variable(m, 0<=z[colors]<=1)
        # @variable(m, w[e1=E, e2=E; ind_edges[e1]>ind_edges[e2]], Bin)

        @objective(m, Max, sum(z[c] for c in colors))

    else
        @objective(m, Min, 1)
    end

    # Transitivity
    for e1 in E
        for e2 in E
            if ind_edges[e1]>ind_edges[e2]
                for e3 in E
                    if ind_edges[e2]>ind_edges[e3]
                        @error "Transitivity constraints incorrect?"
                        @constraint(m, y[e2,e3] <= y[e1,e2] + y[e1,e3])
                        @constraint(m, y[e2,e3] >= y[e1,e2] - y[e1,e3])
                        @constraint(m, y[e2,e3] >= y[e1,e3] - y[e1,e2])
                    end
                end
            end
        end
    end


    # if max_colors
    #     for e in E
    #         @constraint(m, sum(x[e,c] for c in colors) == 1)
    #     end
    #     # Difference variables
    #     for a in E
    #         for b in E
    #             if ind_edges[a]>ind_edges[b]
    #                 for c in colors
    #                     @constraint(m, y[a,b] >= x[a,c] - x[b,c])
    #                     @constraint(m, y[a,b] >= x[b,c] - x[a,c])
    #                 end
    #                 # Forcing x[a,c] and x[b,c] to be different for some c if y[a,b] = 1
    #                 # @constraint(m, sum(c*(x[a,c]-x[b,c]) for c in colors) + (max_colors + 1)*w[a,b] >= y[a,b])
    #                 # @constraint(m, sum(-c*(x[a,c]-x[b,c]) for c in colors) + (max_colors + 1)*(1-w[a,b]) >= y[a,b])
    #             end
    #         end
    #     end
    #     # Colors used
    #     for c in colors
    #         @constraint(m, z[c] <= sum(x[e,c] for e in E))
    #     end
    # end

    if GOrd isa Number
        s = GOrd

        if s==3
        # No orderable triangles
         for v in V[1:end]
             adjacent_edges = []
             for u in neighbors(G,v)
                 push!(adjacent_edges, Edge(min(u,v),max(u,v)))
             end
             for e1 in adjacent_edges
                 for e2 in adjacent_edges 
                     if ind_edges[e1] > ind_edges[e2]
                         @constraint(m, y[e1,e2] >= 1)
                     end
                 end
             end
         end
     
        elseif s==4
            all_K4 = combinations(V,4)
            for V4 in all_K4
                # @show V4
                for u in V4
                    adjacent_edges_u_K4 = [Edge(min(u,v),max(u,v)) for v in V4 if u!=v]
                    # @show u
                    # @show adjacent_edges_u_K4
                    for v in V4
                        if v !=u
                            adjacent_edges_v_K4 = [Edge(min(v,w),max(v,w)) for w in V4 if (w!=v && w!=u)]
                            f1, f2 = adjacent_edges_v_K4
                            if ind_edges[f1]<ind_edges[f2]
                                f1,f2 = f2,f1
                            end
                            # @show v
                            # @show f1,f2
                            @constraint(m, sum(sum(y[e1,e2] for e1 in adjacent_edges_u_K4 if ind_edges[e1]>ind_edges[e2]) for e2 in adjacent_edges_u_K4) + y[f1,f2] >= 1)
                        end
                    end
                end
            end

        elseif s==5
            all_K5 = combinations(V,5)
            for V5 in all_K5
                # @show V4
                for u in V5
                    adjacent_edges_u_K5 = [Edge(min(u,v),max(u,v)) for v in V5 if u!=v]
                    # @show u
                    # @show adjacent_edges_u_K5
                    for v in V5
                        if v !=u
                            adjacent_edges_v_K5 = [Edge(min(v,w),max(v,w)) for w in V5 if (w!=v && w!=u)]
                            # @show adjacent_edges_v_K5
                            w = collect(setdiff(V5, [u,v]))
                            edges_w = [Edge(min(w[i], w[j]),max(w[i], w[j])) for i=1:3 for j=i+1:3 ]
                            # @show edges_w
                            @constraint(m, sum(sum(y[e1,e2] for e1 in adjacent_edges_u_K5 if ind_edges[e1]>ind_edges[e2]) for e2 in adjacent_edges_u_K5) 
                                            + 2*sum(sum(y[e1,e2] for e1 in adjacent_edges_v_K5 if ind_edges[e1]>ind_edges[e2]) for e2 in adjacent_edges_v_K5)
                                            + sum(sum(y[e1,e2] for e1 in edges_w if ind_edges[e1]>ind_edges[e2]) for e2 in edges_w)>= 3)
                        end
                    end
                end
            end
        elseif s==6
            all_K6 = combinations(V,6)
            for V6 in all_K6
                # @show V4
                for u in V6
                    adjacent_edges_u_K6 = [Edge(min(u,v),max(u,v)) for v in V6 if u!=v]
                    # @show u
                    # @show adjacent_edges_u_K5
                    for v in V6
                        if v !=u
                            adjacent_edges_v_K6 = [Edge(min(v,w),max(v,w)) for w in V6 if (w!=v && w!=u)]
                            # @show adjacent_edges_v_K5
                            for w in V6
                                if w ≠ u && w ≠ v
                                    adjacent_edges_w_K6 = [Edge(min(w,t),max(w,t)) for t in V6 if (t!=v && t!=u && t!=w)]
                                    t = collect(setdiff(V6, [u,v,w]))
                                    edges_t = [Edge(min(t[i], t[j]),max(t[i], t[j])) for i=1:3 for j=i+1:3 ]
                                    # @show edges_w
                                    @constraint(m, sum(sum(y[e1,e2] for e1 in adjacent_edges_u_K6 if ind_edges[e1]>ind_edges[e2]) for e2 in adjacent_edges_u_K6) 
                                                    + sum(sum(y[e1,e2] for e1 in adjacent_edges_v_K6 if ind_edges[e1]>ind_edges[e2]) for e2 in adjacent_edges_v_K6)
                                                    + 2*sum(sum(y[e1,e2] for e1 in adjacent_edges_w_K6 if ind_edges[e1]>ind_edges[e2]) for e2 in adjacent_edges_w_K6)
                                                    + sum(sum(y[e1,e2] for e1 in edges_t if ind_edges[e1]>ind_edges[e2]) for e2 in edges_t)>= 3)
                                end
                            end
                        end
                    end
                end
            end
        else
            println("s>=7 not yet supported :'( ")
        end

    elseif startswith(GOrd, "C")
       # No orderable Cycle
        for v in V
            adjacent_edges_v = [Edge(min(v,u),max(v,u)) for u in V if u!=v]
            for e1 in adjacent_edges_v
                for e2 in adjacent_edges_v
                    if ind_edges[e1]>ind_edges[e2]
                        @constraint(m, y[e1,e2] == 1)
                    end
                end
            end
        end
                    
    elseif startswith(GOrd, "L")
        #Ladder
        k=parse(Int64,chop(GOrd, head=1, tail=0))
        if k==3
            for v in V
                adjacent_edges_v = [Edge(min(v,u),max(v,u)) for u in V if u!=v]
                all_incident_triplet_edges = combinations(adjacent_edges_v,3)
                for triplet in all_incident_triplet_edges
                    # @show triplet
                    e,f,g = sort(triplet, by= x->ind_edges[x], rev=true)
                    @constraint(m, y[e,f] + y[e,g] + y[f,g] >= 1)
                end
                for u in V
                    if u ≠ v
                        uv = Edge(min(u,v),max(u,v))
                        other_vertices = setdiff(V, [u,v])
                        all_other_pairs = combinations(other_vertices,2)
                        for k in all_other_pairs
                            for l in all_other_pairs
                                if l ≠ k
                                    k1,k2 = sort(k)
                                    l1,l2 = sort(l)
                                    uk = [Edge(min(u,k1), max(u,k1)), Edge(min(u,k2), max(u,k2))]
                                    (uk1,uk2) = sort(uk, by= x->ind_edges[x], rev=true)
                                    vl = [Edge(min(v,l1), max(v,l1)), Edge(min(v,l2), max(v,l2))]
                                    (vl1,vl2) = sort(vl, by= x->ind_edges[x], rev=true)
                                    @constraint(m, 
                                        2*(y[uk1,uk2] + y[vl1,vl2])
                                        + sum(y[uv,e] for e in vcat(uk,vl) if ind_edges[uv]>ind_edges[e])
                                        + sum(y[e,uv] for e in vcat(uk,vl) if ind_edges[uv]<ind_edges[e]) 
                                        >= 6)
                                end
                            end
                        end
                    end
                end
            end
        else
            println("Only ladders of size 6!!")
            return 0
        end

    elseif startswith(GOrd, "K")
        if endswith(GOrd, "-")
            all_sextuplets = combinations(V,6)
            for sextuplet in all_sextuplets
                all_permutations = permutations(sextuplet)
                for perm in all_permutations
                    (i1, j1, k1, i2, j2, k2) = perm
                    edges_i1 = [Edge(min(i1,k), max(i1,k)) for k in ([i2,j2,k2])]
                    edges_i2 = [Edge(min(i2,k), max(i2,k)) for k in ([j1,k1])]
                    ei21, ei22 = sort(edges_i2, by= x->ind_edges[x], rev=true)
                    @constraint(m, sum(sum(y[e1,e2] for e1 in edges_i1 if ind_edges[e1]>ind_edges[e2]) for e2 in edges_i1)
                                +y[ei21,ei22] >= 1)

                    edges_k1 = [Edge(min(k1,k), max(k1,k)) for k in ([i2,j2])]
                    ek11, ek12 = sort(edges_k1, by= x->ind_edges[x], rev=true)
                    @constraint(m, sum(sum(y[e1,e2] for e1 in edges_i1 if ind_edges[e1]>ind_edges[e2]) for e2 in edges_i1)
                                +y[ek11, ek12] >= 1)

                    edges_i2 = [Edge(min(i2,k), max(i2,k)) for k in ([i1,j1])]
                    ei21, ei22 = sort(edges_i2, by= x->ind_edges[x], rev=true)
                    edges_i1 = [Edge(min(i1,k), max(i1,k)) for k in ([j2,k1])]
                    ei11, ei12 = sort(edges_i1, by= x->ind_edges[x], rev=true)
                    edges_j1 = [Edge(min(j1,k), max(j1,k)) for k in ([j2,k2])]
                    ej11, ej12 = sort(edges_j1, by= x->ind_edges[x], rev=true)
                    edges_k2 = [Edge(min(k2,k), max(k2,k)) for k in ([i1,j1])]
                    ek21, ek22 = sort(edges_k2, by= x->ind_edges[x], rev=true)
                    @constraint(m, 2*(y[ek11,ek12]+y[ei21,ei22]) + y[ei11, ei12] + y[ej11, ej12] + y[ek21, ek22] >= 3)
                end
            end
        else
            k,p = split(chop(GOrd, head=1, tail=0), ",")
            k,p = parse(Int64,k), parse(Int64,p)
            if k == 2 && p == 3
                for i in V
                    other_vertices = setdiff(V, [i])
                    all_other_triplets = combinations(other_vertices,3)
                    for triplet in all_other_triplets
                        edges_i = [Edge(min(i,j), max(i,j)) for j in triplet]
                        @constraint(m, sum(sum(y[e1,e2] for e1 in edges_i if ind_edges[e1]>ind_edges[e2]) for e2 in edges_i) >= 1)
                    end
                    for j in V
                        if j ≠ i
                            other_vertices = setdiff(V, [i,j])
                            all_other_pairs = combinations(other_vertices,2)
                            for pair in all_other_pairs
                                edges_i = [Edge(min(i,k), max(i,k)) for k in pair]
                                edges_j = [Edge(min(j,k), max(j,k)) for k in pair]
                                (ik,il) = sort(edges_i, by= x->ind_edges[x], rev=true)
                                (jk,jl) = sort(edges_j, by= x->ind_edges[x], rev=true)
                                @constraint(m, y[ik,il]+y[jk,jl]>=1)
                            end

                            ij = Edge(min(i,j),max(i,j))
                            all_other_triplets = combinations(other_vertices,3)
                            for triplet in all_other_triplets
                                for k in triplet
                                    l1,l2 = setdiff(triplet, [k])
                                    ik = Edge(min(i,k),max(i,k))
                                    jl1 = Edge(min(j,l1),max(j,l1))
                                    jl2 = Edge(min(j,l2),max(j,l2))
                                    ei1,ei2 = sort([ij,ik], by= x->ind_edges[x], rev=true)
                                    ej1,ej2 = sort([jl1,jl2], by= x->ind_edges[x], rev=true)
                                    @constraint(m, y[ei1,ei2]+y[ej1,ej2]>=1)
                                end
                            end
                        end
                    end
                end
            elseif k == 2 && p == 4
                for i1 in V
                    for i2 in V
                        if i2 ≠ i1
                            Vmini = setdiff(V, [i1,i2])                  
                            i1i2 =  ord_edge(i1,i2)
                            # Item 1
                            all_edges = [ord_edge(i1,k) for k in Vmini]
                            @constraint(m, sum(y[i1i2,e] for e in all_edges if ind_edges[i1i2]>ind_edges[e])
                                        + sum(y[e,i1i2] for e in all_edges if ind_edges[i1i2]<ind_edges[e]) >= n-4)
                            # Item 3
                            all_edges_ij = [(sort([ord_edge(j,i1),ord_edge(j,i2)], by= x->ind_edges[x], rev=true)) for j in Vmini]
                            @constraint(m, sum(y[e1,e2] for (e1,e2) in all_edges_ij) >= n-4)
                            for j in Vmini
                                for k in Vmini
                                    if k ≠ j
                                        # Item 2
                                        Vminijk = setdiff(Vmini, [j,k])
                                        ik, ji, ji2 = ord_edge(i1,k), ord_edge(i1,j), ord_edge(i2,j)
                                        ji, ji2 = sort([ji, ji2], by= x->ind_edges[x], rev=true)
                                        all_edges_i = [ord_edge(i1,l) for l in Vminijk]
                                        @constraint(m, y[ji,ji2] + sum(y[ik,e] for e in all_edges_i if ind_edges[ik]>ind_edges[e])
                                        + sum(y[e,ik] for e in all_edges_i if ind_edges[ik]<ind_edges[e]) >= n-5)
                                        # Item 4
                                        j1=j
                                        j2=k
                                        i1j1,i1j2 = sort([ord_edge(i1,j1),ord_edge(i1,j2)], by= x->ind_edges[x], rev=true)
                                        i2j1,i2j2 = sort([ord_edge(i2,j1),ord_edge(i2,j2)], by= x->ind_edges[x], rev=true)
                                        for k1 in Vminijk
                                            for k2 in Vminijk
                                                if k2 > k1
                                                    j1k1, j1k2 = sort([ord_edge(j1,k1),ord_edge(j1,k2)], by= x->ind_edges[x], rev=true)
                                                    @constraint(m, (y[i1j1,i1j2] + y[i2j1,i2j2]) +y[j1k1, j1k2] >= 1)
                                                end
                                            end
                                        end
                                    end
                                end
                            end
                        end
                    end
                    
                end
            elseif k == 3 && p == 3
                all_sextuplets = permutations(V,6)
                for (i1,i2,i3,j1,j2,j3) in all_sextuplets
                    # First item
                    all_edges_i1j = [ord_edge(i1,j) for j in [j1,j2,j3]]
                    all_edges_i2j = [ord_edge(i2,j) for j in [j1,j2,j3]]
                    i1j1,i1j2,i1j3 = sort(all_edges_i1j, by= x->ind_edges[x], rev=true)
                    i2j1,i2j2,i2j3 = sort(all_edges_i2j, by= x->ind_edges[x], rev=true)
                    @constraint(m, y[i1j1,i1j2] + y[i1j1,i1j3] + y[i1j2,i1j3]+y[i2j1,i2j2] + y[i2j1,i2j3] + y[i2j2,i2j3] >= 1)
                    # Second item
                    j2i2,j2i3 = sort([ord_edge(j2,i2), ord_edge(j2,i3)], by= x->ind_edges[x], rev=true)
                    @constraint(m, y[i1j1,i1j2] + y[i1j1,i1j3] + y[i1j2,i1j3] + y[i2j2,i2j3] + y[j2i2,j2i3] >= 1)
                    # Third Item
                    j1i2,j1i3 = sort([ord_edge(j1,i2), ord_edge(j1,i3)], by= x->ind_edges[x], rev=true)
                    @constraint(m, y[i1j1,i1j2] + y[i1j1,i1j3] + y[i1j2,i1j3] + y[j1i2,j1i3] + y[j2i2,j2i3] >= 1)
                end

                
                # for (u,v) in all_uv
                #     other_vertices = setdiff(V, [u,v])
                #     # No two non-adjacent vertices with 3 incident edges of same color
                #     all_other_triplets = combinations(other_vertices,3)
                #     for (w1, w2, w3) in all_other_triplets
                #         edges_u = [Edge(min(u,w), max(u,w)) for w in ([w1,w2,w3])]
                #         edges_v = [Edge(min(v,w), max(v,w)) for w in ([w1,w2,w3])]
                #         @constraint(m, sum(sum(y[e1,e2] for e1 in edges_u if ind_edges[e1]>ind_edges[e2]) for e2 in edges_u)
                #                     +sum(sum(y[e1,e2] for e1 in edges_v if ind_edges[e1]>ind_edges[e2]) for e2 in edges_v) >= 1)
                #     end
                #     # No vertex with 3 IEoSC + 2 other vertices with 2 IEoSC (one adjacent to the one with 3)
                #     uv = Edge(min(u,v),max(u,v))
                #     all_other_quadruplets = combinations(other_vertices,4)
                #     for quadruplet in all_other_quadruplets
                #         all_pairs = combinations(quadruplet,2)
                #         for (w1,w2) in all_pairs
                #             (t1,t2) = setdiff(quadruplet, [w1,w2])
                #             uw1, uw2 = Edge(min(u,w1), max(u,w1)), Edge(min(u,w2), max(u,w2))
                #             vw1, vw2 = Edge(min(v,w1), max(v,w1)), Edge(min(v,w2), max(v,w2))
                #             ut1, ut2 = Edge(min(u,t1), max(u,t1)), Edge(min(u,t2), max(u,t2))
                #             vt1, vt2 = Edge(min(v,t1), max(v,t1)), Edge(min(v,t2), max(v,t2))
                #             edges_tw = [Edge(min(t,w), max(t,w)) for t ∈ (t1,t2) for w ∈ (w1,w2)]
                #             # t1w1, t1w2 = Edge(min(t1,w1), max(t1,w1)), Edge(min(t1,w2), max(t1,w2))
                #             # t2w1, t2w2 = Edge(min(t2,w1), max(t2,w1)), Edge(min(t2,w2), max(t2,w2))
                #             pairs_edges_u = [(uv, uw1), (uv,uw2), (vt1, vt2)]
                #             pairs_edges_u = [ind_edges[e1]>ind_edges[e2] ? (e1,e2) : (e2,e1) for (e1,e2) in pairs_edges_u]
                #             pairs_edges_v = [(uv, vw1), (uv,vw2), (ut1, ut2)]
                #             pairs_edges_v = [ind_edges[e1]>ind_edges[e2] ? (e1,e2) : (e2,e1) for (e1,e2) in pairs_edges_v]
                #             @constraint(m, 4*sum(y[e1,e2] for (e1,e2) in pairs_edges_u)
                #                             +sum(sum(y[e1,e2] for e1 in edges_tw if ind_edges[e1]>ind_edges[e2]) for e2 in edges_tw) >= 4)
                #         end
                #     end
                # end
            end
        end

    else
        println("\n Other graphs not yet supported :'( \n")
    end

    # if GRain isa Number
    #     t = GRain
    #      # No rainbow Kt    
    #      V_t = combinations(V,t)
    #      for V_subset in V_t
    #      #     @show V_subset
    #          E_Kn = [Edge(i,j) for j in V_subset for i in V_subset if i<j]
    #          if GOrd==3 && t==4
    #              @constraint(m,  sum(sum(y[e1,e2] for e1=E_Kn if (ind_edges[e1]>ind_edges[e2] &&
    #                                                          e1.src != e2.src && e1.src != e2.dst &&
    #                                                          e1.dst != e2.src && e1.dst != e2.dst)) for e2=E_Kn)<=2)
    #          else
    #              nb_pairs_edges_kt = binomial(binomial(t,2),2)
    #              # @show nb_pairs_edges_kt
    #              @constraint(m, sum(sum(y[e1,e2] for e1 in E_Kn if ind_edges[e1]>ind_edges[e2]) for e2 in E_Kn) <= nb_pairs_edges_kt- 1)
    #          end      
    #      end

    #  elseif startswith(GRain, "P") || startswith(GRain, "C")
    #     t = parse(Int64,chop(GRain, head=1, tail=0))
    #     nb_pairs_edges = binomial(t,2)
    #     @show nb_pairs_edges
    #     if startswith(GRain, "P")
    #         t = t+1
    #     end
    #     all_paths_t = permutations(V, t)
    #     for path_t in all_paths_t
    #         # if path_t[1]<path_t[t] # Each cycle in only one direction
    #         edges_Pt = [Edge(min(path_t[i], path_t[i+1]), max(path_t[i], path_t[i+1])) for i=1:t-1]
    #         if startswith(GRain, "P")
    #             @constraint(m, sum(sum(y[e1,e2] for e1 in edges_Pt if ind_edges[e1]>ind_edges[e2]) for e2 in edges_Pt) <= nb_pairs_edges-1)
    #         elseif startswith(GRain, "C")
    #             edges_Ct = vcat(edges_Pt, [Edge(min(path_t[t], path_t[1]), max(path_t[t], path_t[1]))])
    #             # @show edges_Ct
    #             # @show length(edges_Ct)
    #             # @show sum(sum(1 for e1 in edges_Ct if ind_edges[e1]>ind_edges[e2]; init=0) for e2 in edges_Ct; init=0)
    #             @constraint(m, sum(sum(y[e1,e2] for e1 in edges_Ct if ind_edges[e1]>ind_edges[e2]) for e2 in edges_Ct) <= nb_pairs_edges-1)
    #         end
    #         # end
    #     end

    # elseif startswith(GRain, "K")
    #     if contains(GRain, ",")
    #         if endswith(GRain, "-")
    #             k,p = split(chop(GRain, head=1, tail=1), ",")
    #             k,p = parse(Int64,k), parse(Int64,p)
    #             if (k,p) == (3,3)
    #                 all_sextuplets = combinations(V,6)
    #                 for sextuplet in all_sextuplets
    #                     all_permutations = permutations(sextuplet)
    #                     for copy_K33min in all_permutations
    #                         # copy_K33min = sextuplet[perm]
    #                         L,R = copy_K33min[1:3], copy_K33min[4:6]
    #                         edgesLR = [Edge(min(u,v), max(u,v)) for u in L for v ∈ R]
    #                         pop!(edgesLR)
    #                         @constraint(m, sum(sum(y[e1,e2] for e1 in edgesLR if ind_edges[e1]>ind_edges[e2]) for e2 in edgesLR) <= binomial(k*p-1,2)-1)
    #                     end
    #                 end
    #             end
    #         else
    #             k,p = split(chop(GRain, head=1, tail=0), ",")
    #             k,p = parse(Int64,k), parse(Int64,p)
    #             V_kp = combinations(V,k+p)
    #             for V_subset in V_kp
    #                 V_k = combinations(V_subset, k)
    #                 for L in V_k
    #                     edgesLR = [Edge(min(u,v), max(u,v)) for u in L for v in V_subset if v ∉ L]
    #                     @show sum(sum(1 for e1 in edgesLR if ind_edges[e1]>ind_edges[e2]; init=0) for e2 in edgesLR; init=0) <= binomial(k*p,2)-1)
    #                     @constraint(m, sum(sum(y[e1,e2] for e1 in edgesLR if ind_edges[e1]>ind_edges[e2]) for e2 in edgesLR) <= binomial(k*p,2)-1)
    #                 end
    #             end
    #         end
    #     elseif endswith(GRain, "-")
    #         t = parse(Int64,chop(GRain, head=1, tail=1))
    #         # No rainbow Kt-   
    #         V_t = combinations(V,t)
    #         for V_subset in V_t
    #         #     @show V_subset
    #             E_Kn = [Edge(i,j) for j in V_subset for i in V_subset if i<j]
    #             # @show E_Kn
    #             for i in V_subset
    #                 for j in V_subset
    #                     if j>i
    #                         # @show(i,j)
    #                         # E_Kn_minus = setdiff(collect(E_Kn), Edge(i,j))
    #                         nb_pairs_edges_kt_minus = binomial(binomial(t,2)-1,2)
    #                         @constraint(m, sum(sum(y[e1,e2] for e1 in E_Kn if (ind_edges[e1]>ind_edges[e2] && e1 ≠  Edge(i,j))) for e2 in E_Kn if e2 ≠  Edge(i,j)) <= nb_pairs_edges_kt_minus- 1)
    #                     end
    #                 end
    #             end
    #         end     
    #     end 
    #     # No monochromatic Kt
    #     # @constraint(m, sum(sum(y[e1,e2] for e1 in E_Kn if ind_edges[e1]>ind_edges[e2]) for e2 in E_Kn) >= 2)
    #     # end


    # elseif startswith(GRain, "L")
    #     t=parse(Int64,chop(GRain, head=1, tail=0))
    #     if t==3
    #         V_6 = permutations(V,6)
    #         Edges_L3 = [[1,2],[1,3],[2,4],[3,4],[2,5],[4,6],[5,6]]
    #         for V_subset in V_6
    #             E_L3 =[Edge(min(V_subset[i],V_subset[j]),max(V_subset[i],V_subset[j])) for (i,j) in Edges_L3]
    #             @show length(E_L3)
    #             @show sum(sum(1 for e1 in E_L3 if ind_edges[e1]>ind_edges[e2]; init=0) for e2 in E_L3; init=0)
    #             @constraint(m, sum(sum(y[e1,e2] for e1 in E_L3 if ind_edges[e1]>ind_edges[e2]) for e2 in E_L3) <= 20)
    #         end
    #     end
    # else
    #     println("Other graphs not yet supported :'( ")
    # end

    if GRain isa Number
        t = GRain
         # No rainbow Kt    
         V_t = combinations(V,t)
         for V_subset in V_t
         #     @show V_subset
             E_Kn = [Edge(i,j) for j in V_subset for i in V_subset if i<j]
             if GOrd==3 && t==4
                 @constraint(m,  sum(sum(y[e1,e2] for e1=E_Kn if (ind_edges[e1]>ind_edges[e2] &&
                                                             e1.src != e2.src && e1.src != e2.dst &&
                                                             e1.dst != e2.src && e1.dst != e2.dst)) for e2=E_Kn)<=2)
             else
                 nb_pairs_edges_kt = binomial(binomial(t,2),2)
                 # @show nb_pairs_edges_kt
                 @constraint(m, sum(sum(y[e1,e2] for e1 in E_Kn if ind_edges[e1]>ind_edges[e2]) for e2 in E_Kn) <= nb_pairs_edges_kt- 1)
             end      
         end

     elseif startswith(GRain, "P") || startswith(GRain, "C")
        t = parse(Int64,chop(GRain, head=1, tail=0))
        nb_pairs_edges = binomial(t,2)
        @show nb_pairs_edges
        if startswith(GRain, "P")
            t = t+1
        end
        all_paths_t = permutations(V, t)
        for path_t in all_paths_t
            # if path_t[1]<path_t[t] # Each cycle in only one direction
            edges_Pt = [Edge(min(path_t[i], path_t[i+1]), max(path_t[i], path_t[i+1])) for i=1:t-1]
            if startswith(GRain, "P")
                @constraint(m, sum(sum(y[e1,e2] for e1 in edges_Pt if ind_edges[e1]>ind_edges[e2]) for e2 in edges_Pt) <= nb_pairs_edges-1)
            elseif startswith(GRain, "C")
                edges_Ct = vcat(edges_Pt, [Edge(min(path_t[t], path_t[1]), max(path_t[t], path_t[1]))])
                # @show edges_Ct
                # @show length(edges_Ct)
                # @show sum(sum(1 for e1 in edges_Ct if ind_edges[e1]>ind_edges[e2]; init=0) for e2 in edges_Ct; init=0)
                @constraint(m, sum(sum(y[e1,e2] for e1 in edges_Ct if ind_edges[e1]>ind_edges[e2]) for e2 in edges_Ct) <= nb_pairs_edges-1)
            end
            # end
        end

    elseif startswith(GRain, "K")
        if contains(GRain, ",")
            if endswith(GRain, "-")
                k,p = split(chop(GRain, head=1, tail=1), ",")
                k,p = parse(Int64,k), parse(Int64,p)
                if (k,p) == (3,3)
                    all_sextuplets = combinations(V,6)
                    for sextuplet in all_sextuplets
                        all_permutations = permutations(sextuplet)
                        for copy_K33min in all_permutations
                            # copy_K33min = sextuplet[perm]
                            L,R = copy_K33min[1:3], copy_K33min[4:6]
                            edgesLR = [Edge(min(u,v), max(u,v)) for u in L for v ∈ R]
                            pop!(edgesLR)
                            @constraint(m, sum(sum(y[e1,e2] for e1 in edgesLR if ind_edges[e1]>ind_edges[e2]) for e2 in edgesLR) <= binomial(k*p-1,2)-1)
                        end
                    end
                end
            else
                k,p = split(chop(GRain, head=1, tail=0), ",")
                k,p = parse(Int64,k), parse(Int64,p)
                V_kp = combinations(V,k+p)
                for V_subset in V_kp
                    V_k = combinations(V_subset, k)
                    for L in V_k
                        edgesLR = [Edge(min(u,v), max(u,v)) for u in L for v in V_subset if v ∉ L]
                        @constraint(m, sum(sum(y[e1,e2] for e1 in edgesLR if ind_edges[e1]>ind_edges[e2]) for e2 in edgesLR) <= binomial(k*p,2)-1)
                    end
                end
            end
        elseif endswith(GRain, "-")
            t = parse(Int64,chop(GRain, head=1, tail=1))
            # No rainbow Kt-   
            V_t = combinations(V,t)
            for V_subset in V_t
            #     @show V_subset
                E_Kn = [Edge(i,j) for j in V_subset for i in V_subset if i<j]
                # @show E_Kn
                for i in V_subset
                    for j in V_subset
                        if j>i
                            # @show(i,j)
                            # E_Kn_minus = setdiff(collect(E_Kn), Edge(i,j))
                            nb_pairs_edges_kt_minus = binomial(binomial(t,2)-1,2)
                            @constraint(m, sum(sum(y[e1,e2] for e1 in E_Kn if (ind_edges[e1]>ind_edges[e2] && e1 ≠  Edge(i,j))) for e2 in E_Kn if e2 ≠  Edge(i,j)) <= nb_pairs_edges_kt_minus- 1)
                        end
                    end
                end
            end     
        end 
        # No monochromatic Kt
        # @constraint(m, sum(sum(y[e1,e2] for e1 in E_Kn if ind_edges[e1]>ind_edges[e2]) for e2 in E_Kn) >= 2)
        # end


    elseif startswith(GRain, "L")
        t=parse(Int64,chop(GRain, head=1, tail=0))
        if t==3
            V_6 = permutations(V,6)
            Edges_L3 = [[1,2],[1,3],[2,4],[3,4],[2,5],[4,6],[5,6]]
            for V_subset in V_6
                E_L3 =[Edge(min(V_subset[i],V_subset[j]),max(V_subset[i],V_subset[j])) for (i,j) in Edges_L3]
                @show length(E_L3)
                @show sum(sum(1 for e1 in E_L3 if ind_edges[e1]>ind_edges[e2]; init=0) for e2 in E_L3; init=0)
                @constraint(m, sum(sum(y[e1,e2] for e1 in E_L3 if ind_edges[e1]>ind_edges[e2]) for e2 in E_L3) <= 20)
            end
        end
    else
        println("Other graphs not yet supported :'( ")
    end


    if GOrd isa Number
        graph_name = "s$(GOrd)"
    else
        graph_name = "ord$GOrd"
    end
    if GRain isa Number
        graph_name = graph_name * "_t$(GRain)"
    else
        graph_name = graph_name * "_rain$GRain"
    end
    graph_name = graph_name * "_K$n"

    file_model = "model_" * graph_name * ".lp"
    path_file=joinpath(dir_prefix*"LPmodels/", file_model)

    open(path_file, "w") do f
        print(f, m)
    end

    # print(m)
    println("Model Written \n Optimizing...")

    optimize!(m)

    @show termination_status(m)

    if is_solved_and_feasible(m)
        @show objective_value(m)
        # @show(value.(x))
        # @show(value.(y))
        # @show(value.(z))
        
        A, feasible_colors, nb_c = color_from_difference(value.(y), E, ind_edges, n)

        file_adj = graph_name * ".txt"
        path_adj=joinpath(dir_prefix*"feasible_graphs/adjacency_matrices", graph_name*".txt")

        open(path_adj, "w") do f
            for i=1:n
                for j=1:n
                    print(f, "$(A[i,j]) ")
                end
                print(f, "\n")
            end
        end

        if save
            if isfile(dir_prefix*"newfeasibleGraphsK24.jld2")
                @load dir_prefix*"newfeasibleGraphsK24.jld2" results
            else
                results = Any[]
            end
            H1, H2 = get_graph(GOrd), get_graph(GRain)
            @show H1, H2
            push!(results, (adjacency_matrix(H1), adjacency_matrix(H2), A))
            @save dir_prefix*"newfeasibleGraphsK24.jld2" results
        end

        draw(PDF(joinpath(dir_prefix*"feasible_graphs/plots", graph_name*".pdf"), 16cm, 16cm), gplot(G,  layout=circular_layout, nodelabel = V, edgestrokec=feasible_colors))
        draw(SVG(joinpath(dir_prefix*"feasible_graphs/plots", graph_name*".svg"), 16cm, 16cm), gplot(G,  layout=circular_layout, nodelabel = V, edgestrokec=feasible_colors))
    
        println("\n K$n colored with no orderable $GOrd and no rainbow $GRain with $(nb_c) colors ")
        
    elseif termination_status(m) == TIME_LIMIT
        println("Time limit reached for orderable $GOrd and rainbow $GRain in K$n...")
    else
        println("Unfeasible!!")
        println("\n It is impossible to color K$n with no orderable $GOrd and no rainbow $GRain !")
    end
end



relaxation = false
max_colors = false
max_time = 60*5
save=false

# GOrd = "K3,3-"
# # GRain = "K3,3"
GOrd = "K2,4"
GRain = "K2,3"
n=6
colorblind_canonical_ordering(GOrd, GRain, n, relaxation, max_colors, max_time, save)

# s = 6
# t = 3
# n = 15

# colorblind_canonical_ordering(s, t, n, relaxation, max_colors)

##
# GOrd = "K4-"
# GRain = "C4"
# n=4
# colorblind_canonical_ordering(GOrd, GRain, n, relaxation, max_colors, max_time, true)

##
save=true
GOrd = "K2,4"
##
GRain = "C4"
n=8
colorblind_canonical_ordering(GOrd, GRain, n, relaxation, max_colors, max_time, save)

##
GRain = "C5"
n=8
colorblind_canonical_ordering(GOrd, GRain, n, relaxation, max_colors, max_time, save)


##
GRain = "K2,3"
n=8
colorblind_canonical_ordering(GOrd, GRain, n, relaxation, max_colors, max_time, save)

##
GRain = "C6"
n=8
colorblind_canonical_ordering(GOrd, GRain, n, relaxation, max_colors, max_time, save)

##
# max_time = 60*15
GRain = "L3"
n=8
colorblind_canonical_ordering(GOrd, GRain, n, relaxation, max_colors, max_time, save)

##
GRain = "K3,3-"
n=9
colorblind_canonical_ordering(GOrd, GRain, n, relaxation, max_colors, max_time, save)

##
GRain = "K2,4"
n=10
colorblind_canonical_ordering(GOrd, GRain, n, relaxation, max_colors, max_time, save)

##
GRain = "K3,3"
n=10
colorblind_canonical_ordering(GOrd, GRain, n, relaxation, max_colors, max_time, save)


##
GOrd = "K2,4"

GRain = "C4"
n=9
colorblind_canonical_ordering(GOrd, GRain, n, relaxation, max_colors, max_time, save)

##
GRain = "C5"
n=9
colorblind_canonical_ordering(GOrd, GRain, n, relaxation, max_colors, max_time, save)


##
GRain = "K2,3"
n=10
colorblind_canonical_ordering(GOrd, GRain, n, relaxation, max_colors, max_time, save)

##
GRain = "C6"
n=9
colorblind_canonical_ordering(GOrd, GRain, n, relaxation, max_colors, max_time, save)

##
# max_time = 60*15
GRain = "L3"
n=10
colorblind_canonical_ordering(GOrd, GRain, n, relaxation, max_colors, max_time, save)

##
GRain = "K3,3-"
n=11
colorblind_canonical_ordering(GOrd, GRain, n, relaxation, max_colors, max_time, save)

##
GRain = "K2,4"
n=11
colorblind_canonical_ordering(GOrd, GRain, n, relaxation, max_colors, max_time, save)

##
GRain = "K3,3"
n=11
colorblind_canonical_ordering(GOrd, GRain, n, relaxation, max_colors, max_time, save)




##
GOrd = "K3,3"

GRain = "C4"
n=9
colorblind_canonical_ordering(GOrd, GRain, n, relaxation, max_colors, max_time, save)

##
GRain = "C5"
n=10
colorblind_canonical_ordering(GOrd, GRain, n, relaxation, max_colors, max_time, save)
C
##
GRain = "K2,3"
n=10
colorblind_canonical_ordering(GOrd, GRain, n, relaxation, max_colors, max_time, save)

##
GRain = "C6"
n=11
colorblind_canonical_ordering(GOrd, GRain, n, relaxation, max_colors, max_time, save)

##
# max_time = 60*15
GRain = "L3"
n=11
colorblind_canonical_ordering(GOrd, GRain, n, relaxation, max_colors, max_time, save)

##
GRain = "K3,3-"
n=12
colorblind_canonical_ordering(GOrd, GRain, n, relaxation, max_colors, max_time, save)

##
GRain = "K2,4"
n=11
colorblind_canonical_ordering(GOrd, GRain, n, relaxation, max_colors, max_time, save)

##
GRain = "K3,3"
n=13
colorblind_canonical_ordering(GOrd, GRain, n, relaxation, max_colors, max_time, save)



##
# GRain = "C7"
# n=8
# colorblind_canonical_ordering(GOrd, GRain, n, relaxation, max_colors, max_time, save)

# ##
# GRain = "C8"
# n=8
# colorblind_canonical_ordering(GOrd, GRain, n, relaxation, max_colors, max_time, save)
