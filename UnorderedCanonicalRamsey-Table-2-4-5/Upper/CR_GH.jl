using FlagSOS
using FlagSOS: EdgeColoredGraph, isSubFlag
using Combinatorics, AbstractAlgebra


# CanGraph = EdgeColoredGraph{-1, true}
CanGraph = InducedFlag{EdgeColoredGraph{-1,true}}

# generates all partitions of the set [n]. Used to generate all combinations of colors for ordered colorings
function gen_partitions(n)
    if n == 0
        return Vector{Int}[Int[]]
    end
    tmp = gen_partitions(n - 1)
    res = Vector{Int}[]
    for v in tmp
        k = maximum(v; init=0)
        if k == 0
            push!(res, vcat(v, 1))
        else
            for l = 1:k+1
                push!(res, vcat(v, l))
            end
        end
    end
    return res
end

# to forbid non-induced edge colored graph in a blow up, forbid all complete supgraphs (not incomplete supgraphs, as these may be blow-ups!)
function make_induced(G::EdgeColoredGraph{-1,true})
    supGraphs = unique(labelCanonically.(collect(keys(FlagSOS.moebius(G; label=true).coeff))))
    res = InducedFlag{EdgeColoredGraph{-1,true}}[]
    for F in supGraphs
        if FlagSOS.countEdges(F)[1] == binomial(size(G), 2)
            push!(res, InducedFlag{EdgeColoredGraph{-1,true}}(F))
        end
    end
    return res
end

function make_induced(G::InducedFlag{EdgeColoredGraph{-1,true}})
    return [G]
end

# generates all ordered colorings of G up to isomorphism 
function ordered_colorings(G::Graph; output_induced=true)
    # induced if the graph is complete, otherwise noninduced
    T = ifelse(countEdges(G)[1] == binomial(size(G), 2), InducedFlag{EdgeColoredGraph{-1,true}}, EdgeColoredGraph{-1,true})
    res = T[]
    n = size(G)
    A = G.A
    orders = SymmetricGroup(n)
    partitions = gen_partitions(n)
    for σ in orders, p in partitions
        B = zeros(Int, n, n)
        B .= A[σ.d, σ.d]
        for i = 1:n
            B[i, i:n] *= p[i]
            B[i:n, i] *= p[i]
        end
        push!(res, T(B))
    end
    graphs = unique(labelCanonically.(res))

    if !output_induced
        return graphs
    end

    res_induced = reduce(vcat, make_induced(G) for G in graphs)
    res_induced = unique(labelCanonically.(res_induced))
    return res_induced
end


function rainbow_coloring(G::Graph; output_induced=true)
    # induced if the graph is complete, otherwise noninduced
    T = ifelse(countEdges(G)[1] == binomial(size(G), 2), InducedFlag{EdgeColoredGraph{-1,true}}, EdgeColoredGraph{-1,true})
    B = Int.(G.A)
    n = size(B, 1)
    c = 1
    for i = 1:n
        for j = i:n
            if B[i, j] == 1
                B[i, j] = c
                B[j, i] = c
                c += 1
            end
        end
    end
    Gr = labelCanonically(T(B))
    if !output_induced
        return [Gr]
    end
    @show Gr
    @show FlagSOS.aut(Gr)
    res_induced = make_induced(Gr)
    res_induced = unique(labelCanonically.(res_induced))
    return res_induced
end


## forbid orderable G and rainbow H 

using Hypatia, JuMP
#MosekTools

function bound_CR(G, H, n)


    # Blow-up graphs 
    transitiveIndependence = labelCanonically(CanGraph([0 1 0; 1 0 0; 0 0 0]))
    multiColorEdge = labelCanonically(CanGraph([0 1 2; 1 0 0; 2 0 0]))

    forbiddenGraphs = [transitiveIndependence, multiColorEdge, ordered_colorings(G)..., rainbow_coloring(H)...]

    # m = FlagModel{CanGraph,:limit,Rational{Int}}()
    m = FlagModel{CanGraph,:limit,Float64}()

    for F in forbiddenGraphs
        addForbiddenFlag!(m, F)
    end

    display(m.forbiddenFlags)

    # n = 5
    rM = addRazborovBlock!(m, n)


    @show modelSize(rM)
    @show AbstractAlgebra.Partition([size(rM.blockSymmetry[b].pattern, 1) for b in keys(rM.basis)])

    @time computeSDP!(m)


    function CRGHAllowed(G)
        return FlagSOS.isAllowed(m, G)
    end
    ##

    function missingEdge_on_n(n)
        if n == 2
            return CanGraph([0 0; 0 0])
        end
        I1 = CanGraph([0;;])
        missingEdge_n_minus_1 = missingEdge_on_n(n - 1)
        res = labelCanonically(missingEdge_n_minus_1 * I1)
        filter!(x -> CRGHAllowed(x.first), res.coeff)
        return res
    end

    missingEdge = missingEdge_on_n(n)
    @show missingEdge

    function one_on_n(n)
        I1 = CanGraph([0;;])

        if n == 1
            return I1
        end
        I_n_minus_1 = one_on_n(n - 1)
        res = labelCanonically(*(I1, I_n_minus_1; isAllowed=CRGHAllowed))
        filter!(x -> CRGHAllowed(x.first), res.coeff)
        return res
    end

    oneN = one_on_n(n)
    @show oneN

    jm = buildJuMPModel(m)

    obj = 1 * missingEdge
    t = @variable(jm.model, base_name = "t")
    for (G, c) in jm.variables
        if CRGHAllowed(G) # || T() in keys(objL.coeff))
            @assert G == labelCanonically(G)
            # constraints[end][G] = 
            @show G
            @show get(obj.coeff, G, 0)
            @constraint(jm.model, c == Float64.(get(obj.coeff, G, 0)) - t * Float64.(get(oneN.coeff, G, 0)))
        else
            @show G
            error("Shouldn't happen.")
        end
    end

    for (G, c) in obj.coeff
        @assert haskey(jm.variables, G)
    end
    for (G, c) in oneN.coeff
        @assert haskey(jm.variables, G)
    end

    @objective(jm.model, Max, t)

    # set_optimizer(jm.model, Mosek.Optimizer)
    set_optimizer(jm.model, Hypatia.Optimizer)

    # print(jm.model)

    optimize!(jm.model)
    @show termination_status(jm.model)
    @show objective_value(jm.model)

    return 1 / objective_value(jm.model), termination_status(jm.model)
end

##


# # forbid orderable G
# G = Graph(Bool[0 1 1; 1 0 1; 1 1 0]) # C3
# H = Graph(Bool[0 1 1 0; 1 0 0 1; 1 0 0 1; 0 1 1 0]) # C4
# # G = H
# H = Graph(Bool[
#     0 0 1 1 1;
#     0 0 1 1 1;
#     1 1 0 0 0;
#     1 1 0 0 0;
#     1 1 0 0 0]) # K3,2
# # G = Graph(Bool[
# #     0 1 1 1 1;
# #     1 0 1 1 1;
# #     1 1 0 1 1;
# #     1 1 1 0 1;
# #     1 1 1 1 0
# # ])

# @time bound_CR(G, H, 5)
# @time bound_CR(H, G, 5)

# ## K2,3 K2,3

# @time bound_CR(H, H, 6)


##
# function is_tree(G)
#     return length(connectedComponents(G)) == 1 && countEdges(G)[1] == size(G) - 1
# end

# n = 5

# bipartite = FlagSOS.generateAll(Graph, n, 1000; withProperty=f -> (FlagSOS.isBipartite(f) && length(connectedComponents(f)) == 1))[2:end]
# # bipartite = FlagSOS.generateAll(Graph, n, 1000; withProperty = FlagSOS.isBipartite)[2:end]

# not_trees = filter(!is_tree, bipartite)


# bipartiteBounds = Dict()
# for G in not_trees, H in bipartite
#     if !haskey(bipartiteBounds, (G, H, n))
#         @show G, H
#         @timev res = bound_CR(G, H, n)
#         bipartiteBounds[(G, H, n)] = res
#     end
# end

##

# using JLD2

# @save "bipartiteBounds_lvl$n.jld2" bipartiteBounds


##


## Problem case
# (G, H) = ((4,1-3 2-4 3-4), (3,1-3 2-3))
# -> type mono C3 does not have flags of size 4.
# G = bipartite[3]
# H = bipartite[2]
# res = bound_CR(G, H, 5)
# res = bound_CR(H, G, 5)

##

# H = Graph(Bool[0 1 1 0; 1 0 0 1; 1 0 0 1; 0 1 1 0]) # C4
# # G = H
# G = Graph(Bool[
#     0 0 0 1 1 1;
#     0 0 0 1 1 1;
#     0 0 0 1 1 1;
#     1 1 1 0 0 0;
#     1 1 1 0 0 0;
#     1 1 1 0 0 0]) # K3,3

# bound_CR(G, H, 6)

##

# G = bipartite[5]
# H = bipartite[1]

# bound_CR(G, H, 4)

## List of graphs for Lidicky


# G = Graph(Bool[0 1 1; 1 0 1; 1 1 0]) # C3
# H = Graph(Bool[
#     0 0 1 1 1;
#     0 0 1 1 1;
#     1 1 0 0 0;
#     1 1 0 0 0;
#     1 1 0 0 0]) # K2,3
# G = H
# n = 4

# bound_CR(G,H,6)

function forbidden_and_allowed(G, H, n)


    transitiveIndependence = labelCanonically(InducedFlag{EdgeColoredGraph{-1,true}}([0 1 0; 1 0 0; 0 0 0]))
    multiColorEdge = labelCanonically(InducedFlag{EdgeColoredGraph{-1,true}}([0 1 2; 1 0 0; 2 0 0]))

    forbiddenGraphs = InducedFlag{EdgeColoredGraph{-1,true}}[transitiveIndependence, multiColorEdge, ordered_colorings(G)..., rainbow_coloring(H)...]
    @show length(forbiddenGraphs)


    # CRGH_allowed(F) = !any(f -> isSubFlag(f, F), forbiddenGraphs)
    CRGH_allowed(F::InducedFlag{EdgeColoredGraph{-1,true}}) = !isSubFlag(forbiddenGraphs, F)

    @timev allowedGraphs = generateAll(
        InducedFlag{EdgeColoredGraph{-1,true}},
        n,
        [99999];
        withProperty=CRGH_allowed
    )
    @show length(allowedGraphs)

    max_colors = maximum(F -> maximum(F.F.A; init=0), allowedGraphs)
    return forbiddenGraphs, allowedGraphs, max_colors
end

# checks if any F in Fs appears as sublag of G. If F is induced, it checks for the usual induced subgraph isomorphic to F. If F is not induced, it checks for matching subgraphs, where missing edges in F can be any color (but not missing) in G. Assumes Fs are canonically labelled
function canonical_subflag(Fs, G::CanGraph)
    # Very basic brute force algorithm
    ms = sort!(unique(size.(Fs)))
    n = size(G)
    #FsL = labelCanonically.(Fs)
    for m in ms
        tmp = zeros(Int, m, m)
        for c in combinations(1:n, m)
            Gc = labelCanonically(FlagSOS.subFlag(G, c))
            for F in Fs
                size(F) != m && continue
                if F isa CanGraph
                    if F == Gc
                        return true
                    end
                else
                    # @info "checking non-induced"
                    if countEdges(Gc.F)[1] != binomial(m, 2)
                        continue
                    end
                    # @info "checking permutations!"
                    for σ in SymmetricGroup(m)
                        tmp .= Gc.F.A[σ.d, σ.d]
                        tmp[F.A.==0] .= 0
                        FlagSOS.sortEntries!(tmp)
                        if F.A == tmp
                            return true
                        end
                    end
                end
            end
        end
    end
    return false
end

function canonical_subflag(Fs, G::FlagSOS.EdgeMarkedFlag{CanGraph})
    # Very basic brute force algorithm
    ms = sort!(unique(size.(Fs)))
    n = size(G)
    #FsL = labelCanonically.(Fs)
    for m in ms
        tmp = zeros(Int, m, m)
        for c in combinations(1:n, m)
            Gc = labelCanonically(FlagSOS.subFlag(G.F, c))
            for F in Fs
                size(F) != m && continue
                if F isa CanGraph
                    if FlagSOS.finalized_subgraph(G, c) && F == Gc
                        return true
                    end
                else
                    # @info "checking non-induced"
                    if countEdges(Gc.F)[1] != binomial(m, 2)
                        continue
                    end
                    # @info "checking permutations!"
                    for σ in SymmetricGroup(m)
                        tmp .= Gc.F.A[σ.d, σ.d]
                        tmp[F.A.==0] .= 0
                        FlagSOS.sortEntries!(tmp)
                        if F.A == tmp
                            return true
                        end
                    end
                end
            end
        end
    end
    return false
end

function ramsey_subflag(Fs, G::CanGraph; outputErr = false)
    # Very basic brute force algorithm
    ms = sort!(unique(size.(Fs)))
    n = size(G)
    #FsL = labelCanonically.(Fs)
    for m in ms
        tmp = zeros(Int, m, m)
        for c in combinations(1:n, m)
            Gc = labelCanonically(FlagSOS.subFlag(G, c))
            for F in Fs
                size(F) != m && continue
                if F isa CanGraph
                    if F == Gc
                        if outputErr
                            @show c, F, Gc
                        end
                        return true
                    end
                else
                    # @info "checking permutations!"
                    for σ in SymmetricGroup(m)
                        tmp .= Gc.F.A[σ.d, σ.d]
                        tmp[F.A.==0] .= 0
                        FlagSOS.sortEntries!(tmp)
                        if F.A == tmp
                            if outputErr
                                @show c, σ, F, Gc
                            end
                            return true
                        end
                    end
                end
            end
        end
    end
    return false
end

function ramsey_subflag(Fs, G::FlagSOS.EdgeMarkedFlag{CanGraph}; outputErr = false)
    # Very basic brute force algorithm
    ms = sort!(unique(size.(Fs)))
    n = size(G)
    #FsL = labelCanonically.(Fs)
    for m in ms
        tmp = zeros(Int, m, m)
        for c in combinations(1:n, m)
            Gc = labelCanonically(FlagSOS.subFlag(G.F, c))
            for F in Fs
                size(F) != m && continue
                if F isa CanGraph
                    if FlagSOS.finalized_subgraph(G, c) && F == Gc
                        if outputErr
                            @show c, F, Gc
                        end
                        return true
                    end
                else
                    # @info "checking permutations!"
                    for σ in SymmetricGroup(m)
                        tmp .= Gc.F.A[σ.d, σ.d]
                        tmp[F.A.==0] .= 0
                        FlagSOS.sortEntries!(tmp)
                        if F.A == tmp
                            if outputErr
                                @show c, σ, F, Gc
                            end
                            return true
                        end
                    end
                end
            end
        end
    end
    return false
end

# no blow-ups! Generates only complete colored graphs
function ramsey_graphs(G, H, n)
    missingEdge = labelCanonically(InducedFlag{EdgeColoredGraph{-1,true}}([0 0; 0 0]))
    orderedGs = ordered_colorings(G; output_induced=false)
    rainbowH = rainbow_coloring(H; output_induced=false)


    forbiddenGraphs = labelCanonically.([missingEdge, orderedGs..., rainbowH...])
    @show length(forbiddenGraphs)
    @show forbiddenGraphs


    # CRGH_allowed(F) = !any(f -> isSubFlag(f, F), forbiddenGraphs)
    function CRGH_allowed(F)#::InducedFlag{EdgeColoredGraph{-1,true}})
        # return !any(G -> canonical_subflag(G, F), forbiddenGraphs)
        return !ramsey_subflag(forbiddenGraphs, F)
    end
    function CRGH_allowed_marked(F)#::InducedFlag{EdgeColoredGraph{-1,true}})
        # return !any(G -> canonical_subflag(G, F), forbiddenGraphs)
        return !ramsey_subflag(forbiddenGraphs, F)
    end

    @timev res = generateAll(
        InducedFlag{EdgeColoredGraph{-1,true}},
        n,
        [99999];
        withProperty=CRGH_allowed,
        withPropertyMarked=CRGH_allowed_marked
    )
    @show length(res)
    return res
end


# computes the blow-up of the matrix A, blowing up the rows and columns of A according to the sizes
function matrix_blow_up(A::Matrix{Int}, sizes::Vector{Int})
    @assert size(A, 1) == length(sizes)
    n = sum(sizes)
    B = zeros(Int, n, n)
    idx = 1
    for i in 1:length(sizes)
        idy = 1
        for j in 1:length(sizes)
            B[idx:idx+sizes[i]-1, idy:idy+sizes[j]-1] .= A[i, j]
            idy += sizes[j]
        end
        idx += sizes[i]
    end
    return B
end


function blow_up(G::CanGraph, sizes::Vector{Int})
    n = size(G)
    @assert n == length(sizes)
    Ab = matrix_blow_up(Matrix(G.F.A), sizes)
    return CanGraph(Ab)
end

# generates all integer vectors with entries >= 1 summing to n of length size
function sum_n_vectors(size::Int, n::Int)
    if size == 1
        return [[n]]
    end
    res = Vector{Int}[]
    # first entry is at least: n if size = 1, otherwise 1
    minc = 1
    # first entry is at most: n - size + 1
    maxc = n - size + 1
    for c in minc:maxc
        tmp = sum_n_vectors(size - 1, n - c)
        for v in tmp
            push!(res, [c, v...])
        end
    end
    return res
end

# returns all possibilities to blow up the graphs in Gs to at most n vertices
# (replacing vertices by independent sets)
function generate_blow_ups(Gs::Vector{F}, n) where {F}
    res = F[]
    for G in Gs
        m = size(G)
        push!(res, G)
        for m2 = m+1:n
            if m == 0
                push!(res, F(zeros(Int, m2, m2)))
            else
                for sizes in sum_n_vectors(m, m2)
                    # @show sizes
                    push!(res, blow_up(G, sizes))
                end
            end
        end
    end
    return unique(labelCanonically.(res))
end

# does not compute a full list of forbidden (induced) subgraphs! There are too many. Instead uses canonical_subflag to generate allowed graphs only.
function allowed_graphs(G, H, n)


    transitiveIndependence = labelCanonically(InducedFlag{EdgeColoredGraph{-1,true}}([0 1 0; 1 0 0; 0 0 0]))
    multiColorEdge = labelCanonically(InducedFlag{EdgeColoredGraph{-1,true}}([0 1 2; 1 0 0; 2 0 0]))
    orderedGs = ordered_colorings(G; output_induced=false)
    rainbowH = rainbow_coloring(H; output_induced=false)


    forbiddenGraphs = labelCanonically.([transitiveIndependence, multiColorEdge, orderedGs..., rainbowH...])
    @show length(forbiddenGraphs)
    @show forbiddenGraphs

    forbiddenGraphsInduced = [transitiveIndependence, multiColorEdge]



    # CRGH_allowed(F) = !any(f -> isSubFlag(f, F), forbiddenGraphs)
    function CRGH_allowed(F)#::InducedFlag{EdgeColoredGraph{-1,true}})
        # return !any(G -> canonical_subflag(G, F), forbiddenGraphs)
        return !canonical_subflag(forbiddenGraphs, F)
    end
    function CRGH_allowed_marked(F)#::InducedFlag{EdgeColoredGraph{-1,true}})
        # return !any(G -> canonical_subflag(G, F), forbiddenGraphs)
        return !canonical_subflag(forbiddenGraphsInduced, F)
    end

    @timev allowedGraphs = generateAll(
        InducedFlag{EdgeColoredGraph{-1,true}},
        n,
        [99999];
        withProperty=CRGH_allowed,
        withPropertyMarked=CRGH_allowed_marked
    )
    @show length(allowedGraphs)

    max_colors = maximum(F -> maximum(F.F.A; init=0), allowedGraphs)
    return forbiddenGraphs, allowedGraphs, max_colors
end

function allowed_graphs_ramsey_blow_ups(G, H, n)


    transitiveIndependence = labelCanonically(InducedFlag{EdgeColoredGraph{-1,true}}([0 1 0; 1 0 0; 0 0 0]))
    multiColorEdge = labelCanonically(InducedFlag{EdgeColoredGraph{-1,true}}([0 1 2; 1 0 0; 2 0 0]))
    orderedGs = ordered_colorings(G; output_induced=false)
    rainbowH = rainbow_coloring(H; output_induced=false)


    forbiddenGraphs = labelCanonically.([transitiveIndependence, multiColorEdge, orderedGs..., rainbowH...])
    @show length(forbiddenGraphs)
    @show forbiddenGraphs

    @info "Computing ramsey graphs"

    res_ramsey = ramsey_graphs(G, H, n)


    @info "Blowing up graphs"
    res_ramsey_blown_up = generate_blow_ups(res_ramsey, n)

    max_colors = maximum(F -> maximum(F.F.A; init=0), res_ramsey_blown_up)
    return forbiddenGraphs, res_ramsey_blown_up, max_colors
end

##
G = Graph(Bool[0 1 1; 1 0 1; 1 1 0]) # C3
H = Graph(Bool[
    0 0 1 1 1;
    0 0 1 1 1;
    1 1 0 0 0;
    1 1 0 0 0;
    1 1 0 0 0]) # K2,3
G = H

@timev res = forbidden_and_allowed(G, H, 5)
@show length(res[1])
@show length(res[2])
@show res[3]

# res_ramsey = ramsey_graphs(G, H, 5)
# res_ramsey_blown_up = generate_blow_ups(res_ramsey, 5)


@timev res2 = allowed_graphs(G, H, 5)
@show length(res2[1])
@show length(res2[2])
@show res2[3]

@timev res3 = allowed_graphs_ramsey_blow_ups(G, H, 5)
@show length(res3[1])
@show length(res3[2])
@show res3[3]

##
using JLD2
# @save "CR_K23_K23/forbidden_and_allowed_CR_K23_K23.jld2" res

# @load "CR_K23_K23/forbidden_and_allowed_CR_K23_K23.jld2" res

##
# open("CR_K23_K23/F_edges24partition__forbidden.txt", "w") do io
#     for G in res[1][3:end] # all but the blow-up forbidden graphs
#         A = G.F.A
#         n = size(A, 1)
#         print(io, "$n 0 ")
#         for i = 1:n
#             for j = i+1:n
#                 print(io, "$(A[i,j]+1) ")
#             end
#         end
#         println(io)
#     end
# end

# open("CR_K23_K23/F_edges24partition__n6_unlabeled.txt", "w") do io
#     for G in res[2]
#         A = G.F.A
#         n = size(A, 1)
#         print(io, "$n 0 ")
#         for i = 1:n
#             for j = i+1:n
#                 print(io, "$(A[i,j]+1) ")
#             end
#         end
#         println(io)
#     end
# end


## Full script to generate forbidden and allowed graphs for CR(G,H), store them in files, and run Bernard's code

function bound_CR_with_Lidicky(G::Graph, H::Graph, lvl::Int, folder::String)
    forbidden, allowed, max_color = forbidden_and_allowed(G, H, lvl)

    project_dir = @__DIR__

    instance_folder = project_dir * "/$folder"

    !isdir(instance_folder) && mkdir(instance_folder)

    # Bernard's code does not need a list of forbidden graphs!
    open(instance_folder * "/F_edges24partition__forbidden.txt", "w") do io
        for G in forbidden # all but the blow-up forbidden graphs
            A = G.F.A
            n = size(A, 1)
            print(io, "$n 0 ")
            for i = 1:n
                for j = i+1:n
                    print(io, "$(A[i,j]+1) ")
                end
            end
            println(io)
        end
    end

    open(instance_folder * "/F_edges24partition__n$(lvl)_unlabeled.txt", "w") do io
        for G in allowed
            A = G.F.A
            n = size(A, 1)
            print(io, "$n 0 ")
            for i = 1:n
                for j = i+1:n
                    print(io, "$(A[i,j]+1) ")
                end
            end
            println(io)
        end
    end

    lidicky_code = project_dir * "/LidickyOrdered/Canonical/a.out"

    # ../LidickyOrdered/Canonical/a.out -n 6 -lb -v 
    # @show `cd $instance_folder \&\& $lidicky_code -n $n -lb -v`
    cd(instance_folder)
    open(instance_folder * "/F_edges24partition__objectivee.txt", "w") do io
        println(io, "1 2 0 1")
    end
    run(`$lidicky_code -n $lvl -lb -v -omp $nthreads`)

end

function bound_CR_with_Lidicky_only_allowed(G::Graph, H::Graph, lvl::Int, folder::String)
    # forbidden, allowed, max_color = allowed_graphs(G, H, lvl)
    forbidden, allowed, max_color = allowed_graphs_ramsey_blow_ups(G, H, lvl)

    project_dir = @__DIR__

    instance_folder = project_dir * "/$folder"

    !isdir(instance_folder) && mkdir(instance_folder)

    # Bernard's code does not need a list of forbidden graphs!
    open(instance_folder * "/F_edges24partition__forbidden_non_induced.txt", "w") do io
        for G in forbidden # all but the blow-up forbidden graphs
            if G isa CanGraph
                A = G.F.A
                n = size(A, 1)
                print(io, "$n 0 ")
                for i = 1:n
                    for j = i+1:n
                        print(io, "$(A[i,j]+1) ")
                    end
                end
            else
                A = G.A
                n = size(A, 1)
                print(io, "$n 0 ")
                for i = 1:n
                    for j = i+1:n
                        if A[i, j] == 0
                            print(io, ">1 ")
                        else
                            print(io, "$(A[i,j]+1) ")
                        end
                    end
                end
            end
            println(io)
        end
    end

    open(instance_folder * "/F_edges24partition__n$(lvl)_unlabeled.txt", "w") do io
        for G in allowed
            A = G.F.A
            n = size(A, 1)
            print(io, "$n 0 ")
            for i = 1:n
                for j = i+1:n
                    print(io, "$(A[i,j]+1) ")
                end
            end
            println(io)
        end
    end

    lidicky_code = project_dir * "/LidickyOrdered/Canonical/a.out"

    # ../LidickyOrdered/Canonical/a.out -n 6 -lb -v 
    # @show `cd $instance_folder \&\& $lidicky_code -n $n -lb -v`
    cd(instance_folder)
    open(instance_folder * "/F_edges24partition__objectivee.txt", "w") do io
        println(io, "1 2 0 1")
    end

    nthreads = Threads.nthreads()
    run(`$lidicky_code -n $lvl -lb -v -omp $nthreads`)

end


# bound_CR_with_Lidicky(G, H, 6, "C3C4")

##
function is_tree(G)
    return length(connectedComponents(G)) == 1 && countEdges(G)[1] == size(G) - 1
end

function no_degree_one(G)
    return !any(sum(G.A, dims=1) .== 1)
end

n = 6

bipartite = FlagSOS.generateAll(Graph, n, 1000; withProperty=f -> (FlagSOS.isBipartite(f) && length(connectedComponents(f)) == 1))[2:end]
# bipartite = FlagSOS.generateAll(Graph, n, 1000; withProperty = FlagSOS.isBipartite)[2:end]

not_trees = filter(!is_tree, bipartite)
no_deg_1 = filter(no_degree_one, bipartite)


# for G in not_trees, H in bipartite
# for G in bipartite[10:10], H in bipartite[10:10]
for (i, G) in enumerate(no_deg_1), (j, H) in enumerate(no_deg_1)
    # for (i,G) in enumerate(no_deg_1), (j,H) in enumerate(bipartite)
    lvl = 6
    @show G, H, i, j
    cd(@__DIR__)
    project_dir = @__DIR__
    instance_folder = project_dir * "/bipartite_CR/$G$H/"
    if !isfile(instance_folder * "SDP_n$(lvl)_LB_F_edges24partition__objectivee.txt.dat-s")
        # if !isfile(instance_folder * "SDP_n$(lvl)_LB_F_edges24partition__objectivee.txt.dat-s.sol")
        # bound_CR_with_Lidicky(G, H, 5, "bipartite_CR/$G$H")
        bound_CR_with_Lidicky_only_allowed(G, H, lvl, "bipartite_CR/$G$H")
    else
        @info "skipped, already done"
    end
end

# gets stuck at (G, H) = ((4,1-2 1-3 2-4 3-4), (6,1-2 1-3 1-5 4-6 5-6))
# faster with new function _only_allowed! Does not generate full list of forbidden graphs

## Generate Table

function read_result(G, H, lvl)
    cd(@__DIR__)
    project_dir = @__DIR__
    instance_folder = project_dir * "/bipartite_CR/$G$H/"
    sol_file = instance_folder * "SDP_n$(lvl)_LB_F_edges24partition__objectivee.txt.dat-s.sol"
    dats_file = instance_folder * "SDP_n$(lvl)_LB_F_edges24partition__objectivee.txt.dat-s"

    p_obj = "PRIMAL OBJECTIVE    : "
    d_obj = "DUAL OBJECTIVE      : "
    status = "SOLUTION STATUS     : "

    if !isfile(sol_file)
        if isfile(dats_file)
            return :fail
        else
            return :todo
        end
    end

    p_obj_val = 0.0
    d_obj_val = 0.0
    status_val = ""

    for line in eachline(sol_file)
        if startswith(line, status)
            status_val = line[length(p_obj)+1:end]
        end
        if startswith(line, p_obj)
            p_obj_val = parse(Float64, line[length(p_obj)+1:end])
        end
        if startswith(line, d_obj)
            d_obj_val = parse(Float64, line[length(p_obj)+1:end])
            break
        end
    end
    return status_val, p_obj_val, d_obj_val
end

##

cd(@__DIR__)
open("bipartite_CR/results.tex", "w") do io
    println(io, "\\begin{table}")
    println(io, "\\centering")
    print(io, "\t\\begin{tabular}{")
    print(io, "l|")
    for _ in 1:length(no_deg_1)
        print(io, "c")
    end
    println(io, "}\\toprule")
    print(io, "\t\t \${}_{G}\$ \${}^{H}\$&")
    for G in no_deg_1
        print(io, "$G")
        if G == no_deg_1[end]
            println(io, "\\\\\\midrule")
        else
            print(io, " & ")
        end
    end
    for G in no_deg_1
        print(io, "\t\t$G & ")
        for H in no_deg_1
            res = read_result(G, H, 6)
            if res == :fail
                print(io, "{\\color{red}\\text{FAIL}}")
            elseif res == :todo
                # print(io, "\\text{todo}")
            else
                stat, p, d = res
                bound = floor(Int, (-1 / p) + 0.001) + 1
                print(io, "$bound")
            end
            H != no_deg_1[end] && print(io, " & ")
        end
        println(io, "\\\\")
    end
    println(io, "\t\\bottomrule")
    println(io, "\t\\end{tabular}")
    println(io, "\\end{table}")
end

## Verifying Diane's results

function check_feasible(G, H, A)

    missingEdge = labelCanonically(InducedFlag{EdgeColoredGraph{-1,true}}([0 0; 0 0]))
    orderedGs = ordered_colorings(G; output_induced=false)
    rainbowH = rainbow_coloring(H; output_induced=false)


    forbiddenGraphs = labelCanonically.([missingEdge, orderedGs..., rainbowH...])

    !ramsey_subflag(forbiddenGraphs, A; outputErr = true)
end

using JLD2

# @load "bipartite_CR/feasibleGraphs.jld2" results 
# @load "bipartite_CR/feasibleGraphsK24.jld2" results 
@load "bipartite_CR/feasibleGraphsK33.jld2" results 
results 

for (G,H,A) in results 
    G = Graph(BitMatrix(G))
    H = Graph(BitMatrix(H))
    A = CanGraph(Matrix{Int}(A))
    feas = check_feasible(G, H, A)

    print("$G $H ")
    if feas 
        printstyled("feasible!\n", color = :green)
    else 
        printstyled("infeasible!\n", color = :red)
    end
end