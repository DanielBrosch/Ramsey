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
using Plots

## Permuting a graph according to a permutation
function permute_graph(G, perm)
    n = nv(G)
    G_permuted = SimpleGraph(n)
    
    for e in edges(G)
        add_edge!(G_permuted, min(perm[src(e)], perm[dst(e)]), max(perm[src(e)], perm[dst(e)]))
    end
    return G_permuted
end


## Plotting the graph
# import Cairo, Fontconfig
function plot_graph_linear(G, path_file=nothing) 
    n = nv(G)
    x_positions = [i for i in 1:n]
    y_positions = zeros(Int, n)  # All vertices are on a straight line (y=0)
    if !isnothing(path_file)
        if !isempty(edges(G))
            draw(PDF(joinpath(path_file*".pdf"), 16cm, 16cm), gplot(G, x_positions, y_positions, nodelabel=1:n, nodesize=0.7, linetype="curve", outangle=-π/3))
            draw(SVG(joinpath(path_file*".svg"), 16cm, 16cm), gplot(G, x_positions, y_positions, nodelabel=1:n, nodesize=0.7, linetype="curve", outangle=-π/3))
        else
            savefig(plot(x_positions, y_positions, marker=(:circle, 20)), joinpath(path_file*".pdf"))
            Plots.svg(plot(x_positions, y_positions, marker=(:circle, 20)), joinpath(path_file*".pdf"))
        end
    else
        gplot(G, x_positions, y_positions, nodelabel=1:n, linetype="curve", outangle=-π/3)
    end
end

##

# function color_from_difference(y, E, ind_edges, n)
#     colors_draw = distinguishable_colors(100, [RGB(1,1,1), RGB(0,0,0)], dropseed=false)
#     c = 1
#     feasible_colors = [colors_draw[1] for e in E]
#     colored_edges = []
#     A = zeros(Int8, n, n)
#     for e in E
#         if e ∉ colored_edges
#             feasible_colors[ind_edges[e]] = colors_draw[c+2]
#             A[src(e), dst(e)] = c
#             A[dst(e), src(e)] = c
#             println("$e colored with color $c")
#             push!(colored_edges,e)
#             for f in E
#                 if f ∉ colored_edges
#                     if ind_edges[e]<ind_edges[f] && y[e,f] == 0 || ind_edges[e]>ind_edges[f] && y[f,e] == 0
#                         feasible_colors[ind_edges[f]] = colors_draw[c+2]
#                         A[src(f), dst(f)] = c
#                         A[dst(f), src(f)] = c
#                         push!(colored_edges,f)
#                         println("$f colored with color $c")
#                     end
#                 end
#             end
#             c = c+1
#         end
#     end
#     return A, feasible_colors, (c-1)
# end




function colorblind_ordered_canonical_ordering(G_init, perm, n, graph_name, max_time=1200, save=false, results_file=nothing)

    println("Finding a coloring of K$n with no canonically colored $graph_name with permutation $perm\n\n")

    feasible=false

    Kn = complete_graph(n)
    V = vertices(Kn)
    E = edges(Kn)


    ind_edges = Dict(E .=> collect(1:binomial(n,2)))
    for e in E
        ind_edges[(reverse(e))] = ind_edges[(e)]
    end

    G = permute_graph(G_init, perm)

    VG = vertices(G)
    EG = edges(G)
    @show collect(EG)

    # all_G_copies_vertices = permutations(V, nv(G))
    all_G_copies_vertices = powerset(V, nv(G), nv(G))
    # @show collect(all_G_copies_vertices)
    all_G_copies_edges = [[Edge(min(G_copy[src(e)], G_copy[dst(e)]),max(G_copy[src(e)], G_copy[dst(e)])) for e in EG] for G_copy in all_G_copies_vertices]
    # @show collect(all_G_copies_edges)
    # all_G_copies = Dict(zip(collect(all_G_copies_vertices), [[Edge(min(G_copy[src(e)], G_copy[dst(e)]),max(G_copy[src(e)], G_copy[dst(e)])) for e in EG] for G_copy in all_G_copies_vertices]))
    
    m = Model(Gurobi.Optimizer)

    set_optimizer_attribute(m, "TIME_LIMIT", max_time)

    # if relaxation
    #     @variable(m, 1>=y[e1=E, e2=E; ind_edges[e1]<ind_edges[e2]]>=0)
    # else
    #     @variable(m, y[e1=E, e2=E; ind_edges[e1]<ind_edges[e2]], Bin)
    # end

    # if !isnothing(max_colors)
    #     colors = 1:(binomial(n,2)-1)
    #     @variable(m, x[E, colors], Bin) 
    #     @variable(m, 0<=z[colors]<=1)
    #     @variable(m, w[e1=E, e2=E; ind_edges[e1]<ind_edges[e2]], Bin)
    # end

    # x represents the 2 possible colors of an edge: 0 or 1
    @variable(m, x[E], Bin) 

    # @objective(m, Min, 1)

    # Transitivity
    # for e1 in E
    #     for e2 in E
    #         if ind_edges[e1]<ind_edges[e2]
    #             for e3 in E
    #                 if ind_edges[e2]<ind_edges[e3]
    #                     @constraint(m, y[e2,e3] <= y[e1,e2] + y[e1,e3])
    #                     @constraint(m, y[e2,e3] >= y[e1,e2] - y[e1,e3])
    #                     @constraint(m, y[e2,e3] >= y[e1,e3] - y[e1,e2])
    #                 end
    #             end
    #         end
    #     end
    # end

    #Less than max_colors colors
    # if !isnothing(max_colors)
    #     for e in E
    #         @constraint(m, sum(x[e,c] for c in colors) == 1)
    #     end
    #     # Difference variables
    #     for a in E
    #         for b in E
    #             if ind_edges[a]<ind_edges[b]
    #                 for c in colors
    #                     @constraint(m, y[a,b] >= x[a,c] - x[b,c])
    #                     @constraint(m, y[a,b] >= x[b,c] - x[a,c])
    #                 end
    #                 # Forcing x[a,c] and x[b,c] to be different for some c if y[a,b] = 1
    #                 @constraint(m, sum(c*(x[a,c]-x[b,c]) for c in colors) + (max_colors + 1)*w[a,b] >= y[a,b])
    #                 @constraint(m, sum(-c*(x[a,c]-x[b,c]) for c in colors) + (max_colors + 1)*(1-w[a,b]) >= y[a,b])
    #             end
    #         end
    #     end
    #     # Colors used
    #     for c in colors
    #         @constraint(m, z[c] <= sum(x[e,c] for e in E))
    #         for e in E
    #             @constraint(m, z[c] >= x[e,c])
    #         end
    #     end

    #     @constraint(m, sum(z[c] for c in colors) <= max_colors)
    # end
        

    for G_copy_edges in all_G_copies_edges

        # G_copy_edges = all_G_copies[G_copy_vertices]
        # @show G_copy_vertices
        # @show G_copy_edges

        # No monochromatic G
        @constraint(m, sum(x[e] for e=G_copy_edges) >= 1)
        @constraint(m, sum(x[e] for e=G_copy_edges) <= ne(G) -1)
        # No rainbow G
        # if isnothing(max_colors) || max_colors >= ne(G)
        #     @constraint(m, sum(sum(y[e1,e2] for e1=G_copy_edges if ind_edges[e1]<ind_edges[e2]) for e2=G_copy_edges) <= binomial(ne(G),2) - 1)
        # end

        # # No lexicographic G
        # for u in G_copy_vertices
        #     # edges_right_u = [Edge(u,v) for v in G_copy_vertices if v>u]
        #     edges_right_u = [e for e in G_copy_edges if u==src(e)]

        #     if length(edges_right_u) ≥ 2
        #         # @show u 
        #         # @show edges_right_u
        #         @constraint(m, sum(sum(y[e1,e2] for e1=edges_right_u if ind_edges[e1]<ind_edges[e2]) for e2=G_copy_edges) >= 1)
        #     end
        # end

    end

    if !isdir("LPmodels")
        mkdir("LPmodels")
    end

    if !isdir("ordered_graphs")
        mkdir("ordered_graphs")
    end

    perm_string= join(string.(perm))

    path_name = graph_name*"_$(perm_string)_K$n"

    file_model = "model_" * path_name * ".lp"
    path_file=joinpath("LPmodels/", file_model)
    path_image=joinpath("ordered_graphs/", graph_name*"_$(perm_string)")

    open(path_file, "w") do f
        print(f, m)
    end

    plot_graph_linear(G, path_image)

    # print(m)
    println("Model Written \n Optimizing...")

    optimize!(m)

    @show termination_status(m)
    @show typeof(termination_status(m))

    if is_solved_and_feasible(m)
        @show objective_value(m)
        @show(value.(x))
        # @show(value.(y))
        # @show(value.(z))
        
        # A, feasible_colors, nb_c = color_from_difference(value.(y), E, ind_edges, n)
        A = zeros(Int8, n, n)
        feasible_colors = []
        for e in E
            A[src(e),dst(e)] = value.(x)[e]
            A[dst(e),src(e)] = value.(x)[e]
            if value.(x)[e] == 0
                push!(feasible_colors, colorant"red")
            else
                push!(feasible_colors, colorant"blue")
            end
        end
        @show A

        if !isdir("feasible_graphs")
            mkdir("feasible_graphs")
        end

        if !isdir("feasible_graphs/adjacency_matrices")
            mkdir("feasible_graphs/adjacency_matrices")
        end

        path_adj=joinpath("feasible_graphs/adjacency_matrices", path_name*".txt")

        open(path_adj, "w") do f
            for i=1:n
                for j=1:n
                    print(f, "$(A[i,j]) ")
                end
                print(f, "\n")
            end
        end

        if save
            if isfile("feasibleGraphsOrderedRN.jld2")
                @load "feasibleGraphsOrderedRN.jld2" results
            else
                results = Any[]
            end
            push!(results, (adjacency_matrix(G), A))
            @save "feasibleGraphsOrderedRN.jld2" results
        end

        if !isdir("feasible_graphs/plots")
            mkdir("feasible_graphs/plots")
        end

        draw(PDF(joinpath("feasible_graphs/plots", path_name*".pdf"), 16cm, 16cm), gplot(Kn,  layout=circular_layout, nodelabel = V, edgestrokec=feasible_colors))
        draw(SVG(joinpath("feasible_graphs/plots", path_name*".svg"), 16cm, 16cm), gplot(Kn,  layout=circular_layout, nodelabel = V, edgestrokec=feasible_colors))
    
        println("\n K$n feasibly colored!\n")

        if !isnothing(results_file)
            ramBound = n+1
            open(results_file, "a") do f
                println(f, "R($graph_name) >= $ramBound")
            end
        end

        feasible=true

        
    elseif termination_status(m) == TIME_LIMIT
        println("Time limit reached for K$n...")
        if !isnothing(results_file)
            ramBound = n+1
            open(results_file, "a") do f
                println(f, "Time limit reached for R($graph_name)in K$n...")
            end
        end
    else
        println("Unfeasible!!")
        println("\n It is impossible to color K$n with no canonically colored $graph_name !")
        if !isnothing(results_file)
            open(results_file, "a") do f
                println(f, "R($graph_name) <= $n")
            end
        end
    end
    return feasible
end


##

function bounds_all_permutations(G, name_graph, results_file, range_n, max_time=180, save=false)

    all_permutations = permutations(1:nv(G))
    ind_perm=1
    for perm in all_permutations
        if perm[1]>perm[end]
            continue
        end
        graph_name_perm = name_graph*"_$ind_perm"
        for n in range_n
            feasible = colorblind_ordered_canonical_ordering(G, perm, n, graph_name_perm, max_time, save, results_file)
            if !feasible
                break
            end
        end
        open(results_file, "a") do f
            println(f, " ")
        end
        ind_perm+=1
    end
end


function bounds_all_graphs_of_order_n(graphs, range_n, results_file, max_time=180, save=false)

    ind_graph=1
    for (G, name_G) in graphs
        bounds_all_permutations(G, name_G, results_file, range_n, max_time, save)
        ind_graph+=1
    end
end

## 3 vertices

threeK1 = SimpleGraph(3)
P2UK1 = SimpleGraph(3,1)
P3 = path_graph(3)
K3 = complete_graph(3)

graphs3 = [(threeK1, "3K1"),
            (P2UK1, "P2UK1"),
            (P3, "P3"),
            (K3, "K3")]
range_n=2:6
results_file = "allBounds_3_vertices.txt"
bounds_all_graphs_of_order_n(graphs3,range_n, results_file)



## 4 vertices

fourK1 = SimpleGraph(4)

P2U2K1 = SimpleGraph(4,1)

twoK2 = SimpleGraph(4)
add_edge!(twoK2, 1,2)
add_edge!(twoK2, 3,4)

P3UK1 = path_graph(3)
add_vertex!(P3UK1)

K3UK1 = cycle_graph(3)
add_vertex!(K3UK1)

P4 = path_graph(4)

S4 = deepcopy(P3UK1)
add_edge!(S4, 2,4)

Paw4 = deepcopy(S4)
add_edge!(Paw4, 1,3)

C4 = cycle_graph(4)

diamond4 = cycle_graph(4)
add_edge!(diamond4, 1,3)

K4 = complete_graph(4)

# graphs3 = [(fourK1, "4K1"),
#             (P2U2K1, "P2U2K1"),
#             (twoK2, "2K2"),
#             (P3UK1, "P3UK1"),
#             (K3UK1, "K3UK1"),
#             (P4, "P4"),
#             (S4, "S4"),
#             (Paw4, "Paw4"),
#             (C4, "C4"),
#             (diamond4, "diamond4"),
#             (K4, "K4)"]

graphs3 = [(Paw4, "Paw4")]

range_n=5:11
results_file = "allBounds_4_vertices.txt"
max_time = 60*15
bounds_all_graphs_of_order_n(graphs3,range_n, results_file)


##
graphsP5 = [(path_graph(5), "P5")]
range_n =8:30
results_file = "allBounds_P5.txt"
max_time = 60*30
bounds_all_graphs_of_order_n(graphsP5, range_n, results_file, max_time)


## K4

K4 = complete_graph(4)
perm = [1,2,3,4,5]
max_time = 60*30
n=18
results_file = "allBounds_4_vertices.txt"
colorblind_ordered_canonical_ordering(K4, perm, n, "K4", max_time, false, results_file )



#
# GOrd = "K3,3"
# GRain = "C4"
# G = complete_graph(3)

## Double cycle 5
G = path_graph(5)
add_edge!(G,1,3)
add_edge!(G,2,5)
# G = cycle_graph(5)
name_G = "double_cycle_5"
perm = [1,2,3,4,5]
# add_vertex!(G)
# add_edge!(G,3,5)
n =24
colorblind_ordered_canonical_ordering(G, perm, n, name_G)

## Broom5
G = path_graph(3)
add_vertices!(G,2)
add_edge!(G,2,4)
add_edge!(G,2,5)
# G = cycle_graph(5)
name_G = "broom_5"
perm = [1,2,3,4,5]
# add_vertex!(G)
# add_edge!(G,3,5)
n =12
colorblind_ordered_canonical_ordering(G, perm, n, name_G)

## Cross5
G = SimpleGraph(5)
add_edge!(G,1,3)
add_edge!(G,2,3)
add_edge!(G,3,4)
add_edge!(G,3,5)
name_G = "cross_5"
perm = [1,2,3,4,5]
# add_vertex!(G)
# add_edge!(G,3,5)
n =114
colorblind_ordered_canonical_ordering(G, perm, n, name_G)

## tree5_1
G = SimpleGraph(5)
add_edge!(G,1,2)
add_edge!(G,1,3)
add_edge!(G,1,4)
add_edge!(G,4,5)
perm = [1,2,3,4,5]
# add_vertex!(G)
# add_edge!(G,3,5)
n =19
colorblind_ordered_canonical_ordering(G, perm, n, name_G)


## tree5_1
G = SimpleGraph(5)
add_edge!(G,1,2)
add_edge!(G,1,3)
add_edge!(G,1,4)
add_edge!(G,3,5)
# G = cycle_graph(5)
name_G = "tree_5_2"
perm = [1,2,3,4,5]
# add_vertex!(G)
# add_edge!(G,3,5)
n =19
colorblind_ordered_canonical_ordering(G, perm, n, name_G)



