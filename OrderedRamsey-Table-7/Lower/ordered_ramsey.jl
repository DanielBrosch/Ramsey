## Find if there exist a canonical coloring of K_n

using JuMP
using Graphs
using LinearAlgebra
using Combinatorics
# using Colors
# using GraphPlot
using Compose
# using GLPK
using IterTools
# using Cairo, Fontconfig
using Gurobi
# using Plots
using JLD2


# Otherwise this makes a lot of mess in the top level directory
dir_prefix="./"

## Permuting graph G according to permutation perm
function permute_graph(G, perm)
    n = nv(G)
    G_permuted = SimpleGraph(n)
    
    for e in edges(G)
        add_edge!(G_permuted, min(perm[src(e)], perm[dst(e)]), max(perm[src(e)], perm[dst(e)]))
    end
    return G_permuted
end


"""
## Plotting the graph
function plot_graph_linear(G, path_file=nothing) 
    n = nv(G)
    x_positions = [i for i in 1:n]
    y_positions = zeros(Int, n)  # All vertices are on a straight line (y=0)
    if !isnothing(path_file)
        if !isempty(edges(G))
            graph_plot = gplot(G, x_positions, y_positions, nodelabel=1:n, nodesize=0.7, linetype="curve", outangle=-π/3) 
            draw(PDF(joinpath(path_file*".pdf"), 16cm, 16cm), graph_plot) 
            draw(SVG(joinpath(path_file*".svg"), 16cm, 16cm), graph_plot) 
        else
            savefig(plot(x_positions, y_positions, marker=(:circle, 20)), joinpath(path_file*".pdf"))
            Plots.svg(plot(x_positions, y_positions, marker=(:circle, 20)), joinpath(path_file*".pdf"))
        end
    else
        gplot(G, x_positions, y_positions, nodelabel=1:n, linetype="curve", outangle=-π/3)
    end
end
"""

# Creates an edge with ends u and v
function edge(u,v)
    return Edge(min(u,v), max(u,v))
end


# Writes and solves ILP for Ordered Graph on n vertices with no monochromatic G_perm.
# Writes results to various files and draws a picture of the solution, if found
function colorblind_ordered_canonical_ordering(G_init, perm, n, graph_name, max_time=1200, save=false, results_file=nothing)

    println("Finding a coloring of K$n with no canonically colored $graph_name with permutation $perm\n\n")

    feasible=false

    # Necessary graph structures
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

    # All n-vertex subsets of Kn, though of a vertex sets for copies of G 
    all_G_copies_vertices = powerset(V, nv(G), nv(G))
    # List of edge sets, one for each copy G_copy of G in K_n
    all_G_copies_edges = [ [ edge(G_copy[src(e)], G_copy[dst(e)]) for e in EG ] for G_copy in all_G_copies_vertices ]

    # Creating ILP
    m = Model(Gurobi.Optimizer)
    # set_optimizer_attribute(m, "TIME_LIMIT", max_time)

    # x represents the 2 possible colors of an edge: 0 or 1
    @variable(m, x[E], Bin) 

    for G_copy_edges in all_G_copies_edges
        # No monochromatic G
        @constraint(m, sum(x[e] for e=G_copy_edges) >= 1)
        @constraint(m, sum(x[e] for e=G_copy_edges) <= ne(G) -1)
    end

    ## Output
    # Create necessary directories
    if !isdir(dir_prefix*"LPmodels")
        mkdir(dir_prefix*"LPmodels")
    end

    if !isdir(dir_prefix*"ordered_graphs")
        mkdir(dir_prefix*"ordered_graphs")
    end

    perm_string= join(string.(perm))
    path_name = graph_name*"_$(perm_string)_K$n"

    # Is it really worth doing this for every instance?
    # Write the LP to a file
    path_file = joinpath(dir_prefix*"LPmodels/", "model_" * path_name * ".lp")
    open(path_file, "w") do f
        print(f, m)
    end

    # Draw the forbidden graph in specified file?
    path_image = joinpath(dir_prefix*"ordered_graphs/", graph_name*"_$(perm_string)")
    #plot_graph_linear(G, path_image)

    # Solve the LP
    println("Model Written \n Optimizing...")

    optimize!(m)

    @show termination_status(m)
    @show typeof(termination_status(m))

    if is_solved_and_feasible(m)
        # Construct and display the adjacency matrix
        A = zeros(Int8, n, n)
        feasible_colors = []
        for e in E
            A[src(e),dst(e)] = value.(x)[e]
            A[dst(e),src(e)] = value.(x)[e]
	   """
            if value.(x)[e] == 0
                push!(feasible_colors, colorant"red")
            else
                push!(feasible_colors, colorant"blue")
            end
           """
        end
        @show A

        ## Output
        if !isdir(dir_prefix*"feasible_graphs")
            mkdir(dir_prefix*"feasible_graphs")
        end

        if !isdir(dir_prefix*"feasible_graphs/adjacency_matrices")
            mkdir(dir_prefix*"feasible_graphs/adjacency_matrices")
        end

        path_adj=joinpath(dir_prefix*"feasible_graphs/adjacency_matrices", path_name*".txt")

        # Print the adjacency matrix
        open(path_adj, "w") do f
            for i=1:n
                for j=1:n
                    print(f, "$(A[i,j]) ")
                end
                print(f, "\n")
            end
        end

        # Save the results
        if save
            if isfile(dir_prefix*"feasibleGraphsOrderedRN.jld2")
                @load dir_prefix*"feasibleGraphsOrderedRN.jld2" results
            else
                results = Any[]
            end
            push!(results, (adjacency_matrix(G), A))
            @save dir_prefix*"feasibleGraphsOrderedRN.jld2" results
        end

        if !isdir(dir_prefix*"feasible_graphs/plots")
            mkdir(dir_prefix*"feasible_graphs/plots")
        end

        # Draw the Ramsey graph
        # plot_path = dir_prefix*"feasible_graphs/plots"
        # graph_plot = gplot(Kn,  layout=circular_layout, nodelabel = V, edgestrokec=feasible_colors)
        # draw(PDF(joinpath(plot_path, path_name*".pdf"), 16cm, 16cm), graph_plot)
        # draw(SVG(joinpath(plot_path, path_name*".svg"), 16cm, 16cm), graph_plot)
    
        println("\n K$n feasibly colored with no $graph_name with permutation $perm\n")

        # Write the corresponding bound to the results file
        if !isnothing(results_file)
            ramBound = n+1
            open(results_file, "a") do f
                println(f, "R("*graph_name*"_$(perm_string)"*") >= $ramBound")
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
        println("\n It is impossible to color K$n with no monochrome $graph_name with permutation $perm !")
        if !isnothing(results_file)
            open(results_file, "a") do f
                println(f, "R("*graph_name*"_$(perm_string)"*") <= $n")
            end
        end
    end
    return feasible
end


## Determines if there exists an n-vertex Ordered Ramsey graph for G_\pi for each ordering G_\pi of G
#  and each n in range_n.

function bounds_all_permutations(G, name_graph, results_file, range_n, max_time=180, save=false)

    all_permutations = permutations(1:nv(G))
    ind_perm=1
    for perm in all_permutations
        # We may assume perm[1] < perm[end] by reflection symmetry
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

    for (G, name_G) in graphs
        bounds_all_permutations(G, name_G, results_file, range_n, max_time, save)
    end
end

num_verts = parse(Int,ARGS[1])
println(" START RAMSEY BOUND SEARCH ")
flush(stdout)
if num_verts == 3
    ## 3 vertices
    threeK1 = SimpleGraph(3)
    P2UK1 = SimpleGraph(3,1)
    P3 = path_graph(3)
    K3 = complete_graph(3)
    graphs3 = [(threeK1, "3K1"),
                (P2UK1, "P2UK1"),
                (P3, "P3"),
                (K3, "K3")]
    range_n=5:6
    results_file = dir_prefix*"allBounds_3_vertices.txt"
    bounds_all_graphs_of_order_n(graphs3,range_n, results_file)
elseif num_verts == 4
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

    graphs3 = [(fourK1, "4K1"),
                (P2U2K1, "P2U2K1"),
                (twoK2, "2K2"),
                (P3UK1, "P3UK1"),
                (K3UK1, "K3UK1"),
                (P4, "P4"),
                (S4, "S4"),
                (Paw4, "Paw4"),
                (C4, "C4"),
                (diamond4, "diamond4"),
                (K4, "K4")]

    graphs3 = [(Paw4, "Paw4")]

    range_n=5:11
    results_file = dir_prefix*"allBounds_4_vertices.txt"
    max_time = 60*15
    bounds_all_graphs_of_order_n(graphs3,range_n, results_file)
    ## K4

    K4 = complete_graph(4)
    perm = [1,2,3,4,5]
    max_time = 60*30
    n=18
    results_file = dir_prefix*"allBounds_4_vertices.txt"
    colorblind_ordered_canonical_ordering(K4, perm, n, "K4", max_time, false, results_file )
elseif num_verts == 5

    # Path on five vertices
    P = path_graph(5)
    # Orderings of the path
    permP1 = [5,3,1,2,4] # Row 1
    permP2 = [5,4,2,1,3] # Row 2
    permP3 = [5,2,1,3,4] # Row 3
    # Solve
    println("Row 1\n")
    flush(stdout)
 #   colorblind_ordered_canonical_ordering(P, permP1, 13, "row_1")
    flush(stdout)

#=
    println("Row 2\n")
    flush(stdout)
    colorblind_ordered_canonical_ordering(P, permP2, 15, "row_2")
    flush(stdout)

    println("Row 3\n")
    flush(stdout)
    colorblind_ordered_canonical_ordering(P, permP3, 14, "row_3")
    flush(stdout)
 =#   

    # Pentagon
    C = cycle_graph(5)
    # Orderings of the pentagon
    permC1 = [1,3,4,2,5] # Row 4
    permC2 = [1,3,5,4,2] # Row 5
    # Solve
#=
    println("Row 4\n")
    flush(stdout)
    colorblind_ordered_canonical_ordering(C, permC1, 13, "row_4")
    flush(stdout)
    println("Row 5\n")
    flush(stdout)
    colorblind_ordered_canonical_ordering(C, permC2, 18, "row_5")
    flush(stdout)
=#
    
    # House Graph
    H = cycle_graph(5)
    add_edge!(H,2,5)
    permH = [1,2,5,4,3] # Row 6
    # Solve
    println("Row 6\n")
    flush(stdout)
 #   colorblind_ordered_canonical_ordering(H, permH, 23, "row_6")
    flush(stdout)

    # Star with four points
    S = complete_bipartite_graph(1,4)
    # Orderings of the star
    permS1 = [2,1,3,4,5] # Row 7
    permS2 = [3,1,2,4,5] # Row 8
    # Solve 
 #   println("Row 7\n")
 #   flush(stdout)
 #   colorblind_ordered_canonical_ordering(S, permS1, 12, "row_7")
 #   flush(stdout)
    println("Row 8\n")
    flush(stdout)
    colorblind_ordered_canonical_ordering(S, permS1, 12, "row_8")
    flush(stdout)
#=
    # Broom with two bristles and five vertices
    B = path_graph(4)
    add_vertices!(B,1)
    add_edge!(B,3,5)
    # Orderings of the broom
    permB1 = [5,4,1,2,3] # Row 9
    permB2 = [5,3,1,2,4] # Row 10
    # Solve
    println("Row 9\n")
    flush(stdout)
    colorblind_ordered_canonical_ordering(B, permB1, 12, "row_9")
    flush(stdout)
    println("Row 10\n")
    flush(stdout)
    colorblind_ordered_canonical_ordering(B, permB1, 11, "row_10")
    flush(stdout)
=#
end
