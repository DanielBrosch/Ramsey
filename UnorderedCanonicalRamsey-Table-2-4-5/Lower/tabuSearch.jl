## Install branch: pkg> dev FlagSOS#EdgeColoredGraphslRamsey.git
## pkg> dev https://github.com/DanielBrosch/FlagSOS.jl/tree/EdgeColoredGraphs
## pkg> add https://github.com/DanielBrosch/FlagSOS.jl/tree/EdgeColoredGraphs

using FlagSOS
using FlagSOS: EdgeColoredGraph, isSubFlag
using LinearAlgebra, Combinatorics
using Plots, GraphPlot, Compose
using DelimitedFiles
using Cairo, Fontconfig
using Graphs
import Random
using Memoize
using JLD2
using Base.Threads
# using DataStructures

const CanGraph = InducedFlag{EdgeColoredGraph{-1,true}}
# const CanGraph = EdgeColoredGraph{-1,true}
##

@show Threads.nthreads()



# Blow-up graphs        
# transitiveIndependence = labelCanonically(CanGraph([0 1 0; 1 0 0; 0 0 0]))
# multiColorEdge = labelCanonically(CanGraph([0 1 2; 1 0 0; 2 0 0]))

# @memoize Dict{Tuple{CanGraph,CanGraph},Bool}
function isIsomorphic_memoized(F::T, G::T) where {T<:Flag}
    # @info "memoizing..."
    return FlagSOS.isIsomorphic(F, G)
end


function countSubflags(F::T, G::T; induced=F isa InducedFlag) where {T<:Flag}
    # Very basic brute force algorithm
    m = size(F)
    n = size(G)
    count = 0
    for c in combinations(1:n, m)
        Gc = FlagSOS.subFlag(G, c)
        if induced
            # if FlagSOS.isIsomorphic(F, Gc)
            if isIsomorphic_memoized(F, Gc)
                count += 1
            end
        else
            @error "Waaay tooo slow"
            for d in SymmetricGroup(m)
                if subFlag(G, c) == glue(F, Gc, d.d)
                    count += 1
                end
            end
        end
    end
    return count
end


## 


function forbidden_graphs(s, t)

    forbiddenOrderable_s = Set{CanGraph}()
    forbiddenRainbow_t = Set{CanGraph}()

    if s == 3
        monoK3 = labelCanonically(CanGraph([0 1 1; 1 0 1; 1 1 0]))
        orderableK3 = labelCanonically(CanGraph([0 2 1; 2 0 1; 1 1 0]))
        push!(forbiddenOrderable_s, monoK3, orderableK3)

    elseif s == 4

        monoK4 = labelCanonically(CanGraph([
            0 1 1 1;
            1 0 1 1;
            1 1 0 1;
            1 1 1 0
        ]))

        orderableK4_1 = labelCanonically(CanGraph([
            0 1 1 1;
            1 0 2 2;
            1 2 0 3;
            1 2 3 0
        ]))
        orderableK4_2 = labelCanonically(CanGraph([
            0 1 1 1;
            1 0 2 2;
            1 2 0 1;
            1 2 1 0
        ]))
        orderableK4_3 = labelCanonically(CanGraph([
            0 1 1 1;
            1 0 2 2;
            1 2 0 2;
            1 2 2 0
        ]))
        orderableK4_4 = labelCanonically(CanGraph([
            0 1 1 1;
            1 0 1 1;
            1 1 0 2;
            1 1 2 0
        ]))

        push!(forbiddenOrderable_s, monoK4, orderableK4_1, orderableK4_2, orderableK4_3, orderableK4_4)

    else
        println("s > 4 not yet supported, sorry")
    end

    if t == 3
        rainbowK3 = labelCanonically(CanGraph([0 1 2; 1 0 3; 2 3 0]))
        push!(forbiddenRainbow_t, rainbowK3)

    elseif t == 4
        rainbowK4 = labelCanonically(CanGraph([
            0 1 2 3;
            1 0 4 5;
            2 4 0 6;
            3 5 6 0
        ]))
        push!(forbiddenRainbow_t, rainbowK4)

    elseif t == 5
        rainbowK5 = labelCanonically(CanGraph([
            0 1 2 3 4;
            1 0 5 6 7;
            2 5 0 8 9;
            3 6 8 0 10;
            4 7 9 10 0
        ]))
        push!(forbiddenRainbow_t, rainbowK5)

    else
        println("t > 5 not yet supported, sorry")
    end
    return forbiddenOrderable_s, forbiddenRainbow_t
end

function score_coloring(s, t, G)

    forbiddenOrderable_s, forbiddenRainbow_t = forbidden_graphs(s, t)
    score = 0
    for O in forbiddenOrderable_s
        score += countSubflags(O, G)
    end
    for R in forbiddenRainbow_t
        score += countSubflags(R, G)
    end
    return score
end

function score_coloring_countCol(s, t, G)

    @assert s == 3
    @assert t == 5
    score = 0

    n = size(G)

    # forbidding orderable K3 <=> all K3 are rainbow 
    for i = 1:n-2
        for j = i+1:n-1
            for k = j+1:n
                if G.F.A[i, j] == G.F.A[i, k] || G.F.A[i, j] == G.F.A[j, k] || G.F.A[i, k] == G.F.A[j, k]
                    score += 1
                end
            end
        end
    end

    # forbidding rainbow K5 <=> all K5 have a duplicate color
    inds = Int[0, 0, 0, 0, 0]
    colors = zeros(Bool, maximum(G.F.A; init=0))
    # for c in combinations(setdiff(1:n, [i, j]), 3)

    for c1 in 1:n-4
        inds[1] = c1
        for c2 in c1+1:n-3
            inds[2] = c2
            for c3 in c2+1:n-2
                inds[3] = c3
                for c4 in c3+1:n-1
                    inds[4] = c4
                    for c5 in c4+1:n
                        inds[5] = c5


                        colors .= false
                        rainbow = true
                        for k = 1:5-1
                            for l = k+1:5
                                # for p in combinations(inds, 2)
                                col = G.F.A[inds[k], inds[l]]
                                if colors[col]
                                    rainbow = false
                                    break
                                end
                                colors[col] = true
                            end
                        end
                        if rainbow
                            score += 1
                        end
                    end
                end
            end
        end
    end


    return score
end

function score_ord(forbiddenOrderable_s, G)
    score = 0
    for O in forbiddenOrderable_s
        score += countSubflags(O, G)
    end
    return score
end

function score_rain(forbiddenRainbow_t, G)
    score = 0
    for R in forbiddenRainbow_t
        score += countSubflags(R, G)
    end
    return score
end


function score_coloring_fast(s, t, G)

    forbiddenOrderable_s, forbiddenRainbow_t = forbidden_graphs(s, t)

    score = score_rain(forbiddenRainbow_t, G)

    chunks_s = Iterators.partition(forbiddenOrderable_s, length(forbiddenOrderable_s) ÷ min(Threads.nthreads(), length(forbiddenOrderable_s)))
    tasks = map(chunks_s) do chunk
        Threads.@spawn score_ord(chunk, G)
    end
    chunk_sums_s = fetch.(tasks)
    score += sum(chunk_sums_s)

    return score

end

# s = 3
# t = 4

# G = CanGraph([0 1 2 1; 1 0 3 1; 2 3 0 2; 1 1 2 0])
# # # G = CanGraph([0 1 1 2 4; 1 0 1 2 4; 1 1 0 3 4 ; 1 2 3 0 2; 4 4 4 2 0])

# score_coloring(s, t, G)



##
function delta_score_chunk(all_K, forbiddenGraphs, GOld, GNew)

    delta_score = 0
    for K in all_K
        # GsOld = labelCanonically(FlagSOS.subFlag(GOld, K))
        # GsNew = labelCanonically(FlagSOS.subFlag(GNew, K))
        GsOld = FlagSOS.subFlag(GOld, K)
        GsNew = FlagSOS.subFlag(GNew, K)
        # @show length(K), size.(forbiddenGraphs)
        for F in forbiddenGraphs
            @assert length(K) == size(F)
            if isIsomorphic_memoized(F, GsNew)
                delta_score += 1
            end
            if isIsomorphic_memoized(F, GsOld)
                delta_score -= 1
            end
            # delta_score += countSubflags(F, GsNew) - countSubflags(F, GsOld)
        end
    end
    return delta_score
end



# !!! Do NOT label canonically Gnew before this function !!!
function delta_score_coloring(s, t, i, j, GOld, GNew, all_Ks, all_Kt)

    forbiddenOrderable_s, forbiddenRainbow_t = forbidden_graphs(s, t)

    all_Ks_with_ij = [Ks for Ks in all_Ks if i ∈ Ks && j ∈ Ks]
    all_Kt_with_ij = [Kt for Kt in all_Kt if i ∈ Kt && j ∈ Kt]

    # chunks_s = Iterators.partition(all_Ks_with_ij, length(all_Ks_with_ij) ÷ min(Threads.nthreads(), length(all_Ks_with_ij)))
    # tasks = map(chunks_s) do chunk
    #     Threads.@spawn delta_score_chunk(chunk, forbiddenOrderable_s, GOld, GNew)
    # end
    # chunk_sums_s = fetch.(tasks)
    # delta_score = sum(chunk_sums_s)

    # if length(all_Kt_with_ij) > 1
    #     chunks_t = Iterators.partition(all_Kt_with_ij, length(all_Kt_with_ij) ÷ min(Threads.nthreads(), length(all_Kt_with_ij)))
    #     tasks = map(chunks_t) do chunk
    #         Threads.@spawn delta_score_chunk(chunk, forbiddenRainbow_t, GOld, GNew)
    #     end
    #     chunk_sums_t = fetch.(tasks)
    #     delta_score += sum(chunk_sums_t)
    # else
    #     delta_score += delta_score_chunk(all_Kt_with_ij, forbiddenRainbow_t, GOld, GNew)
    # end

    delta_score = 0

    for Ks in all_Ks_with_ij
        # GsOld = labelCanonically(FlagSOS.subFlag(GOld, Ks))
        # GsNew = labelCanonically(FlagSOS.subFlag(GNew, Ks))
        # for O in forbiddenOrderable_s
        #     delta_score += countSubflags(O, GsNew) - countSubflags(O, GsOld)
        # end

        GsOld = FlagSOS.subFlag(GOld, Ks)
        GsNew = FlagSOS.subFlag(GNew, Ks)
        # @show length(K), size.(forbiddenGraphs)
        for F in forbiddenOrderable_s
            # @assert length(K) == size(F)
            if isIsomorphic_memoized(F, GsNew)
                delta_score += 1
            end
            if isIsomorphic_memoized(F, GsOld)
                delta_score -= 1
            end
            # delta_score += countSubflags(F, GsNew) - countSubflags(F, GsOld)
        end
    end
    for Kt in all_Kt_with_ij
        # GtOld = labelCanonically(FlagSOS.subFlag(GOld, Kt))
        # GtNew = labelCanonically(FlagSOS.subFlag(GNew, Kt))
        # for R in forbiddenRainbow_t
        #     delta_score += countSubflags(R, GtNew) - countSubflags(R, GtOld)
        # end
        GtOld = FlagSOS.subFlag(GOld, Kt)
        GtNew = FlagSOS.subFlag(GNew, Kt)
        # @show length(K), size.(forbiddenGraphs)
        for F in forbiddenRainbow_t
            # @assert length(K) == size(F)
            if isIsomorphic_memoized(F, GtNew)
                delta_score += 1
            end
            if isIsomorphic_memoized(F, GtOld)
                delta_score -= 1
            end
            # delta_score += countSubflags(F, GsNew) - countSubflags(F, GsOld)
        end
    end

    return delta_score

end


function delta_score_coloring_countCol(s, t, i, j, GOld, GNew, inds=zeros(Int, t), colors=zeros(Bool, max(maximum(GOld.F.A; init=0), maximum(GNew.F.A; init=0))))

    delta_score = 0
    n = size(GOld)

    @assert s == 3 "TODO"
    # forbidding orderable K3 <=> all K3 are rainbow 
    for k = 1:n
        (k == i || k == j) && continue
        if GOld.F.A[i, j] == GOld.F.A[i, k] || GOld.F.A[i, j] == GOld.F.A[j, k] || GOld.F.A[i, k] == GOld.F.A[j, k]
            # @info "Orderable K3 in old:"
            # display(GOld.F.A[[i,j,k], [i,j,k]])
            delta_score -= 1
        end
        if GNew.F.A[i, j] == GNew.F.A[i, k] || GNew.F.A[i, j] == GNew.F.A[j, k] || GNew.F.A[i, k] == GNew.F.A[j, k]
            # @info "Orderable K3 in new:"
            # display(GNew.F.A[[i,j,k], [i,j,k]])
            delta_score += 1
        end
    end

    @assert t == 5 "TODO"
    # forbidding rainbow K5 <=> all K5 have a duplicate color
    # inds = Int[i, j, 0, 0, 0]
    inds[1] = i
    inds[2] = j
    # colors = zeros(Bool, max(maximum(GOld.F.A; init=0), maximum(GNew.F.A; init=0)))
    # for c in combinations(setdiff(1:n, [i, j]), 3)
    for a = 1:n-2
        (a == i || a == j) && continue
        inds[3] = a
        for b = a+1:n-1
            (b == i || b == j) && continue
            inds[4] = b
            for c = b+1:n
                (c == i || c == j) && continue
                inds[5] = c

                # inds[3:end] .= c

                colors .= false
                rainbow = true
                for k = 1:5-1
                    for l = k+1:5
                        # for p in combinations(inds, 2)
                        col = GOld.F.A[inds[k], inds[l]]
                        if colors[col]
                            rainbow = false
                            break
                        end
                        colors[col] = true
                    end
                end
                if rainbow
                    delta_score -= 1
                end

                colors .= false
                rainbow = true

                # for p in combinations(inds, 2)
                for k = 1:5-1
                    for l = k+1:5
                        col = GNew.F.A[inds[k], inds[l]]
                        if colors[col]
                            rainbow = false
                            break
                        end
                        colors[col] = true
                    end
                end
                if rainbow
                    delta_score += 1
                end

            end
        end

        # if allunique(GOld.F.A[p...] for p in combinations(inds, 2))
        #     delta_score -= 1
        # end
        # if allunique(GNew.F.A[p...] for p in combinations(inds, 2))
        #     delta_score += 1
        # end
    end


    return delta_score

end

function cheap_label(F::CanGraph)
    WCellInd = BitVector(false for _ in 1:size(F))
    WCellInd .= true
    d = sortperm([FlagSOS.distinguish(F, v, WCellInd) for v = 1:size(F)])
    return permute(F, d)
end



##


function tabuSearch(s::Int, t::Int, n::Int, T_init::Float64=1.0, T_factor::Float64=0.5, nb_iter_max::Union{Nothing,Int}=100000, done=Atomic{Bool}(false), labelFunc=labelCanonically)

    println("\nLooking for a feasible coloring of K$n without orderable K$s and rainbow K$t \n")
    # max_colors = div(binomial(n,2),2)+1
    # max_colors = n + 1
    # max_colors = n + 2
    # max_colors = n * 2

    max_colors = (n % 2 == 0 ? n - 1 : n)

    # Initial random coloring
    coloring = rand(1:max_colors, n, n)
    coloring[diagind(coloring)] .= 0
    coloring = Symmetric(coloring)
    G = CanGraph(coloring)
    G = labelFunc(G)

    # Calculating score of the coloring
    score = score_coloring_countCol(s, t, G)
    # FlagSOS.isSubFlag(transitiveIndependence, G)
    # foundGraphs = Set{CanGraph}()
    # foundGraphs = SortedSet{UInt64}()
    foundGraphs = Set{UInt64}()
    push!(foundGraphs, hash(G))

    println("Found initial coloring with score $score \n")
    # Computing one all the cliques of size s and t
    # all_Ks = collect(powerset(collect(1:n), s, s))
    # all_Kt = collect(powerset(collect(1:n), t, t))

    iter = 0
    T = T_init


    duplicate_count = 0

    bestScore = Inf


    inds = zeros(Int, 5)
    colorsTmp = zeros(Bool, max_colors)

    coloring_neighbor = Matrix(deepcopy(G.F.A))
    coloring_neighborTmp = deepcopy(coloring_neighbor)

    while score > 0
        done[] && break

        # if score == 0
        #     last_iter = iter
        #     break
        # end

        # Create a neighbor coloring by randomly changing the color of an edge
        i, j = rand(1:n), rand(1:n)
        while i == j
            j = rand(1:n)
        end
        new_color = rand(1:max_colors)
        while new_color == G.F.A[i, j]
            new_color = rand(1:max_colors)
        end


        # coloring_neighbor = Matrix(G.F.A)
        coloring_neighborTmp .= G.F.A
        # @assert coloring_neighbor == coloring_neighbor'
        coloring_neighborTmp[i, j] = new_color
        coloring_neighborTmp[j, i] = new_color
        # G_neighbor = CanGraph(deepcopy(coloring_neighbor))

        # coloring_neighbor .= coloring_neighborTmp

        G_neighbor = CanGraph(coloring_neighborTmp)
        # G_neighbor = labelCanonically(G_neighbor)

        # G_neighbor_labelled = labelCanonically(G_neighbor)
        # G_neighbor_labelled = G_neighbor
        # G_neighbor_labelled = cheap_label(G_neighbor)
        G_neighbor_labelled = labelFunc(G_neighbor)


        maxDup = n * n * max_colors * 100

        reached_max_duplicates = duplicate_count == maxDup

        gHash = hash(G_neighbor_labelled)

        if duplicate_count < maxDup && gHash in foundGraphs
            duplicate_count += 1
            continue
        else
            # reached_max_duplicates && @info "Max duplicates"
            duplicate_count = 0
            push!(foundGraphs, gHash)
        end

        # score_neighbor = score_coloring(s,t,G_neighbor)
        # if score_neighbor <= score || rand() <= exp((score-score_neighbor/T))
        # delta_score = delta_score_coloring(s, t, i, j, G, G_neighbor, all_Ks, all_Kt)
        # display(G)
        # display(G_neighbor)
        delta_score = delta_score_coloring_countCol(s, t, i, j, G, G_neighbor, inds, colorsTmp)
        # @show delta_score
        # @show delta_score_coloring(s, t, i, j, G, G_neighbor, all_Ks, all_Kt)
        # @assert delta_score == delta_score_coloring(s, t, i, j, G, G_neighbor, all_Ks, all_Kt)
        # if reached_max_duplicates || delta_score <= 0 || rand() <= exp(-delta_score / T)
        if reached_max_duplicates || delta_score < 0 || rand() <= exp(-delta_score / T)
            # G = labelFunc(G_neighbor_labelled)
            G = deepcopy(G_neighbor_labelled)
            # coloring = coloring_neighbor
            # score = score_neighbor
            score = score + delta_score
            T = T * T_factor
            # @show score, bestScore
            if score < bestScore
                bestScore = score
                @show bestScore
                # global bestGraph = G
                # global bestGraphScore = score
            end
        end


        iter += 1

        if iter % 100000 == 0
            # println("\n")
            @show iter, T, Threads.threadid(), bestScore, score
            # println("\n")
        end

        if !isnothing(nb_iter_max) && iter == nb_iter_max
            println("Max iter")
            break
        end
    end

    if !done[] && score == 0
        println("\nFeasible coloring of K$n without orderable K$s and rainbow K$t found in $iter iterations\n ")
        A = Matrix(G.F.A)
        @save "./coloredGraphs/G$n.jld2" A
        Gdraw = SimpleGraph(A)
        # edgecolor_dict = Dict()
        colors_draw = distinguishable_colors(max_colors + 1, [RGB(1, 1, 1), RGB(0, 0, 0)], dropseed=true)
        feasible_colors = []
        for e in edges(Gdraw)
            c = A[e.src, e.dst]
            push!(feasible_colors, colors_draw[c])
            #     edgecolor_dict[i,j] = colors_draw[A[i,j]+1]
        end
        # x = vcat([i for i=1:div((n+1),2)], [i for i=1:div(n,2)])
        # y = vcat([i for i=1:div((n+3),4)], [i for i=div((n),4):-1:1], [-i for i=1:div((n+2),4)], [-i for i=div(n+1,4):-1:1])
        # x = [1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8]
        # y = [1,2,3,4,4,3,2,1,-1,-2,-3,-4,-4,-3,-2,-1]

        # @show x,y
        # graphplot(A; edgecolor=edgecolor_dict, x=x, y=y, edgewidth= (s, d, w) -> 3)
        # savefig("code/CanonicalRamsey/coloredGraphs/tabu_s$(s)_t$(t)_K$n.pdf")

        # FlagSOS.isSubFlag(transitiveIndependence, G)
        # countSubflags(transitiveIndependence, G)

        draw(PDF("./coloredGraphs/tabu_s$(s)_t$(t)_K$n.pdf", 16cm, 16cm), gplot(Gdraw, layout=circular_layout, nodelabel=1:n, edgestrokec=feasible_colors))
        draw(SVG("./coloredGraphs/tabu_s$(s)_t$(t)_K$n.svg", 16cm, 16cm), gplot(Gdraw, layout=circular_layout, nodelabel=1:n, edgestrokec=feasible_colors))
        # draw(PDF("coloredGraphs/tabu_s$(s)_t$(t)_K$n.pdf", 16cm, 16cm), graphplot(A, edgecolor=edgecolor_dict, x=x, y=y))
        # draw(SVG("coloredGraphs/tabu_s$(s)_t$(t)_K$n.svg", 16cm, 16cm), graphplot(A, edgecolor=edgecolor_dict, x=x, y=y))

        @show coloring
    end
    if !done[]
        done[] = true
        return G
    else
        return nothing
    end
end

function tabuSearch_greedy(s::Int, t::Int, n::Int, T_init::Float64=1.0, T_factor::Float64=0.5, nb_iter_max::Union{Nothing,Int}=100000, done=Atomic{Bool}(false), labelFunc=labelCanonically)

    println("\nLooking for a feasible coloring of K$n without orderable K$s and rainbow K$t \n")
    # max_colors = div(binomial(n,2),2)+1
    # max_colors = n + 1
    # max_colors = n + 2
    # max_colors = n * 2

    max_colors = (n % 2 == 0 ? n - 1 : n)

    # Initial random coloring
    coloring = rand(1:max_colors, n, n)
    coloring[diagind(coloring)] .= 0
    coloring = Symmetric(coloring)
    G = CanGraph(coloring)
    G = labelFunc(G)

    # Calculating score of the coloring
    # score = score_coloring(s, t, G)
    score = score_coloring_countCol(s, t, G)
    # FlagSOS.isSubFlag(transitiveIndependence, G)
    # foundGraphs = Set{CanGraph}()
    # push!(foundGraphs, G)
    foundGraphs = Set{UInt64}()
    push!(foundGraphs, hash(G))

    println("Found initial coloring with score $score \n")
    # Computing one all the cliques of size s and t
    # all_Ks = collect(powerset(collect(1:n), s, s))
    # all_Kt = collect(powerset(collect(1:n), t, t))

    iter = 0
    T = T_init


    # duplicate_count = 0

    bestScore = Inf

    while score > 0
        done[] && break

        # if score == 0
        #     last_iter = iter
        #     break
        # end

        bestG = G
        bestDelta = Inf

        # Find best new neighbor coloring by randomly changing the color of an edge
        for i in 1:n, j in 1:n, new_color in 1:max_colors
            i == j && continue
            G.F.A[i, j] == new_color && continue

            coloring_neighbor = Matrix(G.F.A)
            coloring_neighbor[i, j] = new_color
            coloring_neighbor[j, i] = new_color
            G_neighbor = CanGraph(coloring_neighbor)
            # G_neighbor = labelCanonically(G_neighbor)

            # G_neighbor_labelled = labelCanonically(G_neighbor)
            # G_neighbor_labelled = G_neighbor
            # G_neighbor_labelled = cheap_label(G_neighbor)

            # G_neighbor_labelled in foundGraphs && continue

            # #TODO: Only push graph if we actually move to it?
            # push!(foundGraphs, G_neighbor_labelled)

            # delta_score = delta_score_coloring(s, t, i, j, G, G_neighbor, all_Ks, all_Kt)
            delta_score = delta_score_coloring_countCol(s, t, i, j, G, G_neighbor)
            if delta_score < bestDelta

                # G_neighbor_labelled = labelCanonically(G_neighbor)
                G_neighbor_labelled = labelFunc(G_neighbor)
                gHash = hash(G_neighbor_labelled)
                if gHash in foundGraphs
                    continue
                end
                # push!(foundGraphs, gHash)

                bestDelta = delta_score
                bestG = G_neighbor_labelled
            end

        end


        if bestDelta == Inf # all neighbours are old, move at random
            @info "No new neighbours, random move"
            i, j = rand(1:n), rand(1:n)
            while i == j
                j = rand(1:n)
            end
            new_color = rand(1:max_colors)
            while new_color == G.F.A[i, j]
                new_color = rand(1:max_colors)
            end
            coloring_neighbor = Matrix(G.F.A)
            coloring_neighbor[i, j] = new_color
            coloring_neighbor[j, i] = new_color
            G_neighbor = CanGraph(coloring_neighbor)
            bestDelta = delta_score_coloring(s, t, i, j, G, G_neighbor, all_Ks, all_Kt)
            bestG = labelFunc(G_neighbor)
            # bestG = labelCanonically(G_neighbor)
        end

        #TODO: Only push graph if we actually move to it?
        push!(foundGraphs, hash(bestG))

        # always move, even if it gets worse
        G = bestG
        # coloring = coloring_neighbor
        # score = score_neighbor
        score = score + bestDelta
        # T = T * T_factor
        if score < bestScore
            @show bestScore
            bestScore = score
            # global bestGraph = G
            # global bestGraphScore = score
        end
        # end


        iter += 1

        if iter % 1000 == 0
            # println("\n")
            @show iter, T, Threads.threadid(), bestScore, score
            # println("\n")
        end

        if !isnothing(nb_iter_max) && iter == nb_iter_max
            println("Max iter")
            break
        end
    end

    if !done[] && score == 0
        println("\nFeasible coloring of K$n without orderable K$s and rainbow K$t found in $iter iterations\n ")
        A = Matrix(G.F.A)
        @save "./coloredGraphs/G$n.jld2" A
        Gdraw = SimpleGraph(A)
        # edgecolor_dict = Dict()
        colors_draw = distinguishable_colors(max_colors + 1, [RGB(1, 1, 1), RGB(0, 0, 0)], dropseed=true)
        feasible_colors = []
        for e in edges(Gdraw)
            c = A[e.src, e.dst]
            push!(feasible_colors, colors_draw[c])
            #     edgecolor_dict[i,j] = colors_draw[A[i,j]+1]
        end
        # x = vcat([i for i=1:div((n+1),2)], [i for i=1:div(n,2)])
        # y = vcat([i for i=1:div((n+3),4)], [i for i=div((n),4):-1:1], [-i for i=1:div((n+2),4)], [-i for i=div(n+1,4):-1:1])
        # x = [1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8]
        # y = [1,2,3,4,4,3,2,1,-1,-2,-3,-4,-4,-3,-2,-1]

        # @show x,y
        # graphplot(A; edgecolor=edgecolor_dict, x=x, y=y, edgewidth= (s, d, w) -> 3)
        # savefig("code/CanonicalRamsey/coloredGraphs/tabu_s$(s)_t$(t)_K$n.pdf")

        # FlagSOS.isSubFlag(transitiveIndependence, G)
        # countSubflags(transitiveIndependence, G)

        draw(PDF("./coloredGraphs/tabu_s$(s)_t$(t)_K$n.pdf", 16cm, 16cm), gplot(Gdraw, layout=circular_layout, nodelabel=1:n, edgestrokec=feasible_colors))
        draw(SVG("./coloredGraphs/tabu_s$(s)_t$(t)_K$n.svg", 16cm, 16cm), gplot(Gdraw, layout=circular_layout, nodelabel=1:n, edgestrokec=feasible_colors))
        # draw(PDF("coloredGraphs/tabu_s$(s)_t$(t)_K$n.pdf", 16cm, 16cm), graphplot(A, edgecolor=edgecolor_dict, x=x, y=y))
        # draw(SVG("coloredGraphs/tabu_s$(s)_t$(t)_K$n.svg", 16cm, 16cm), graphplot(A, edgecolor=edgecolor_dict, x=x, y=y))

        @show coloring
    end
    if !done[]
        done[] = true
        return G
    else
        return nothing
    end

end

function tabuSearch_greedy_shared_dict(s::Int, t::Int, n::Int, T_init::Float64=1.0, T_factor::Float64=0.5, nb_iter_max::Union{Nothing,Int}=100000, done=Atomic{Bool}(false), labelFunc=labelCanonically, foundGraphs::Set{UInt64}=Set{UInt64}(), foundGraphs_lock::ReentrantLock=ReentrantLock())

    println("\nLooking for a feasible coloring of K$n without orderable K$s and rainbow K$t \n")
    # max_colors = div(binomial(n,2),2)+1
    # max_colors = n + 1
    # max_colors = n + 2
    # max_colors = n * 2

    max_colors = (n % 2 == 0 ? n - 1 : n)

    # Initial random coloring
    coloring = rand(1:max_colors, n, n)
    coloring[diagind(coloring)] .= 0
    coloring = Symmetric(coloring)
    G = CanGraph(coloring)
    G = labelFunc(G)

    # Calculating score of the coloring
    # score = score_coloring(s, t, G)
    score = score_coloring_countCol(s, t, G)
    # FlagSOS.isSubFlag(transitiveIndependence, G)
    # foundGraphs = Set{CanGraph}()
    # push!(foundGraphs, G)
    # foundGraphs = Set{UInt64}()

    @lock foundGraphs_lock push!(foundGraphs, hash(G))

    println("Found initial coloring with score $score \n")
    # Computing one all the cliques of size s and t
    # all_Ks = collect(powerset(collect(1:n), s, s))
    # all_Kt = collect(powerset(collect(1:n), t, t))

    iter = 0
    T = T_init


    # duplicate_count = 0

    bestScore = Inf

    while score > 0
        done[] && break

        # if score == 0
        #     last_iter = iter
        #     break
        # end

        bestG = G
        bestDelta = Inf

        # Find best new neighbor coloring by randomly changing the color of an edge
        for i in 1:n, j in 1:n, new_color in 1:max_colors
            i == j && continue
            G.F.A[i, j] == new_color && continue

            coloring_neighbor = Matrix(G.F.A)
            coloring_neighbor[i, j] = new_color
            coloring_neighbor[j, i] = new_color
            G_neighbor = CanGraph(coloring_neighbor)
            # G_neighbor = labelCanonically(G_neighbor)

            # G_neighbor_labelled = labelCanonically(G_neighbor)
            # G_neighbor_labelled = G_neighbor
            # G_neighbor_labelled = cheap_label(G_neighbor)

            # G_neighbor_labelled in foundGraphs && continue

            # #TODO: Only push graph if we actually move to it?
            # push!(foundGraphs, G_neighbor_labelled)

            # delta_score = delta_score_coloring(s, t, i, j, G, G_neighbor, all_Ks, all_Kt)
            delta_score = delta_score_coloring_countCol(s, t, i, j, G, G_neighbor)
            if delta_score < bestDelta

                # G_neighbor_labelled = labelCanonically(G_neighbor)
                G_neighbor_labelled = labelFunc(G_neighbor)
                gHash = hash(G_neighbor_labelled)
                @lock foundGraphs_lock if gHash in foundGraphs
                    continue
                end
                # push!(foundGraphs, gHash)

                bestDelta = delta_score
                bestG = G_neighbor_labelled
            end

        end


        if bestDelta == Inf # all neighbours are old, move at random
            @info "No new neighbours, random move"
            i, j = rand(1:n), rand(1:n)
            while i == j
                j = rand(1:n)
            end
            new_color = rand(1:max_colors)
            while new_color == G.F.A[i, j]
                new_color = rand(1:max_colors)
            end
            coloring_neighbor = Matrix(G.F.A)
            coloring_neighbor[i, j] = new_color
            coloring_neighbor[j, i] = new_color
            G_neighbor = CanGraph(coloring_neighbor)
            bestDelta = delta_score_coloring(s, t, i, j, G, G_neighbor, all_Ks, all_Kt)
            bestG = labelFunc(G_neighbor)
            # bestG = labelCanonically(G_neighbor)
        end

        #TODO: Only push graph if we actually move to it?
        @lock foundGraphs_lock push!(foundGraphs, hash(bestG))

        # always move, even if it gets worse
        G = bestG
        # coloring = coloring_neighbor
        # score = score_neighbor
        score = score + bestDelta
        # T = T * T_factor
        if score < bestScore
            @show bestScore
            bestScore = score
            # global bestGraph = G
            # global bestGraphScore = score
        end
        # end


        iter += 1

        if iter % 1000 == 0
            # println("\n")
            @show iter, T, Threads.threadid(), bestScore, score
            # println("\n")
        end

        if !isnothing(nb_iter_max) && iter == nb_iter_max
            println("Max iter")
            break
        end
    end

    if !done[] && score == 0
        println("\nFeasible coloring of K$n without orderable K$s and rainbow K$t found in $iter iterations\n ")
        A = Matrix(G.F.A)
        @save "./coloredGraphs/G$n.jld2" A
        Gdraw = SimpleGraph(A)
        # edgecolor_dict = Dict()
        colors_draw = distinguishable_colors(max_colors + 1, [RGB(1, 1, 1), RGB(0, 0, 0)], dropseed=true)
        feasible_colors = []
        for e in edges(Gdraw)
            c = A[e.src, e.dst]
            push!(feasible_colors, colors_draw[c])
            #     edgecolor_dict[i,j] = colors_draw[A[i,j]+1]
        end
        # x = vcat([i for i=1:div((n+1),2)], [i for i=1:div(n,2)])
        # y = vcat([i for i=1:div((n+3),4)], [i for i=div((n),4):-1:1], [-i for i=1:div((n+2),4)], [-i for i=div(n+1,4):-1:1])
        # x = [1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8]
        # y = [1,2,3,4,4,3,2,1,-1,-2,-3,-4,-4,-3,-2,-1]

        # @show x,y
        # graphplot(A; edgecolor=edgecolor_dict, x=x, y=y, edgewidth= (s, d, w) -> 3)
        # savefig("code/CanonicalRamsey/coloredGraphs/tabu_s$(s)_t$(t)_K$n.pdf")

        # FlagSOS.isSubFlag(transitiveIndependence, G)
        # countSubflags(transitiveIndependence, G)

        draw(PDF("./coloredGraphs/tabu_s$(s)_t$(t)_K$n.pdf", 16cm, 16cm), gplot(Gdraw, layout=circular_layout, nodelabel=1:n, edgestrokec=feasible_colors))
        draw(SVG("./coloredGraphs/tabu_s$(s)_t$(t)_K$n.svg", 16cm, 16cm), gplot(Gdraw, layout=circular_layout, nodelabel=1:n, edgestrokec=feasible_colors))
        # draw(PDF("coloredGraphs/tabu_s$(s)_t$(t)_K$n.pdf", 16cm, 16cm), graphplot(A, edgecolor=edgecolor_dict, x=x, y=y))
        # draw(SVG("coloredGraphs/tabu_s$(s)_t$(t)_K$n.svg", 16cm, 16cm), graphplot(A, edgecolor=edgecolor_dict, x=x, y=y))

        @show coloring
    end
    if !done[]
        done[] = true
        return G
    else
        return nothing
    end

end

##

s = 3
t = 5
n = 10
# @time G = tabuSearch(s, t, n, n / 100, 0.9999, nothing)
@time G = tabuSearch(s, t, n, 0.1, 0.9999, nothing, Atomic{Bool}(false), (x) -> x)

# 120 - 350 MiB
@timev G = tabuSearch(s, t, n, 0.1, 0.9999, 100_000, Atomic{Bool}(false), (x) -> x)

# @timev G = tabuSearch(s, t, n, 0.1, 0.9999, 10_000, Atomic{Bool}(false), cheap_label)
@timev G = tabuSearch_greedy(s, t, n, 0.1, 0.9999, 10_000, Atomic{Bool}(false), (x) -> x)
# @timev G = tabuSearch_greedy(s, t, n, 0.1, 0.9999, 10_000, Atomic{Bool}(false), labelCanonically)
@profview_allocs G = tabuSearch(s, t, n, 0.1, 0.9999, 100_000, Atomic{Bool}(false), (x) -> x)
@profview_allocs G = tabuSearch(s, t, n, 0.1, 0.9999, 100_000, Atomic{Bool}(false), cheap_label)
# @profview_allocs G = tabuSearch(s, t, n, 0.1, 0.9999, 10_000, Atomic{Bool}(false), labelCanonically)
##

for n = 13:16
    done::Atomic{Bool} = Atomic{Bool}(false)


    foundGraphs::Set{UInt} = Set{UInt}()
    foundGraphs_lock::ReentrantLock = ReentrantLock()

    # for _ in 1:floor(Threads.nthreads()/2)
    tasks = map(1:Threads.nthreads()) do _
        # Threads.@spawn tabuSearch(3, 5, n, 0.1, 0.9999, nothing, done, (x) -> x)
        # Threads.@spawn tabuSearch_greedy(3, 5, n, 0.1, 0.9999, nothing, done, (x) -> x)
        Threads.@spawn tabuSearch_greedy_shared_dict(3, 5, n, 0.1, 0.9999, nothing, done, (x) -> x, foundGraphs, foundGraphs_lock)
        # Threads.@spawn tabuSearch(3, 5, n, 0.1, 0.9999, nothing, done, cheap_label)
    end

    t = @timed Gs = fetch.(tasks)
    @show n
    for G in Gs
        if G !== nothing
            display(G.F.A)
            @show t
            A = G.F.A
            @save "./coloredGraphs/resG$n.jld2" A t
            # @save "./coloredGraphs/G$(n)stats.jld2" t
        end
    end
end
##

coloring11 = [0 1 2 3 4 5 6 7 8 9 10; 1 0 9 6 7 10 8 2 11 4 5; 2 9 0 1 5 6 11 8 10 7 3; 3 6 1 0 10 9 7 4 5 11 2; 4 7 5 10 0 11 2 9 1 3 8; 5 10 6 9 11 0 4 3 7 8 1; 6 8 11 7 2 4 0 1 3 10 9; 7 2 8 4 9 3 1 0 6 5 11; 8 11 10 5 1 7 3 6 0 2 4; 9 4 7 11 3 8 10 5 2 0 6; 10 5 3 2 8 1 9 11 4 6 0]

G11 = CanGraph(coloring11)

score_coloring_countCol(3, 5, G11)
score_coloring(3, 5, G11)

H11 = FlagSOS.aut(G11)

##

coloring12 = [0 1 2 3 4 5 6 7 8 9 10 11; 1 0 11 2 6 3 10 4 9 7 8 5; 2 11 0 9 10 6 8 5 7 1 4 3; 3 2 9 0 7 11 4 10 1 8 5 6; 4 6 10 7 0 9 1 3 11 5 2 8; 5 3 6 11 9 0 7 8 2 4 1 10; 6 10 8 4 1 7 0 2 5 3 11 9; 7 4 5 10 3 8 2 0 6 11 9 1; 8 9 7 1 11 2 5 6 0 10 3 4; 9 7 1 8 5 4 3 11 10 0 6 2; 10 8 4 5 2 1 11 9 3 6 0 7; 11 5 3 6 8 10 9 1 4 2 7 0]


G12 = CanGraph(coloring12)

score_coloring_countCol(3, 5, G12)
# score_coloring(3, 5, G12)

# (gen = [[1, 7, 3, 6, 10, 4, 2, 12, 9, 5, 11, 8], [4, 11, 2, 9, 5, 1, 3, 8, 6, 10, 7, 12], [11, 6, 9, 7, 12, 2, 4, 10, 3, 8, 1, 5]], size = 16)
# 1349s (WHY???) 
#TODO: Figure out why induced aut is wrong here
@timev H12 = aut(G12)
# (gen = [[1, 12, 7, 4, 8, 9, 3, 5, 6, 10, 11, 2], [10, 2, 7, 11, 8, 6, 3, 5, 9, 1, 4, 12], [4, 9, 5, 1, 3, 12, 8, 7, 2, 11, 10, 6], [8, 2, 4, 7, 1, 6, 11, 10, 9, 5, 3, 12]], size = 16)
# (gen = [[1, 4, 3, 2, 5, 9, 8, 7, 6, 12, 11, 10], [3, 2, 1, 4, 11, 9, 7, 8, 6, 12, 5, 10], [5, 7, 11, 8, 1, 10, 2, 4, 12, 6, 3, 9], [9, 2, 6, 4, 12, 1, 7, 8, 3, 5, 10, 11]], size = 16)
@timev H12 = aut(G12.F)

# wrong!
using AbstractAlgebra
# (2,7)(4,6)(5,10)(8,12)
g1 = perm([1, 7, 3, 6, 10, 4, 2, 12, 9, 5, 11, 8])

# (1,4,9,6)(2,11,7,3)
g2 = perm([4, 11, 2, 9, 5, 1, 3, 8, 6, 10, 7, 12])

# (1,11)(2,6)(3,9)(4,7)(5,12)(8,10)
g3 = perm([11, 6, 9, 7, 12, 2, 4, 10, 3, 8, 1, 5])

fullHWrong = FlagSOS.generateGroup([g1, g2, g3], 16)


# Correct? Also wrong
using FlagSOS
# (2,12)(3,7)(5,8)(6,9)
g1 = perm([1, 12, 7, 4, 8, 9, 3, 5, 6, 10, 11, 2])

# (1,10)(3,7)(4,11)(5,8)
g2 = perm([10, 2, 7, 11, 8, 6, 3, 5, 9, 1, 4, 12])

# (1,4)(2,9)(3,5)(6,12)(7,8)(10,11)
g3 = perm([4, 9, 5, 1, 3, 12, 8, 7, 2, 11, 10, 6])

# (1,8,10,5)(3,4,7,11)
g4 = perm([8, 2, 4, 7, 1, 6, 11, 10, 9, 5, 3, 12])

fullH = FlagSOS.generateGroup([g1, g2, g3, g4], 16)

## Correct? 
using FlagSOS
# (2,4)(6,9)(7,8)(10,12)
g1 = perm([1, 4, 3, 2, 5, 9, 8, 7, 6, 12, 11, 10])

# (1,3)(5,11)(6,9)(10,12)
g2 = perm([3, 2, 1, 4, 11, 9, 7, 8, 6, 12, 5, 10])

# (1,5)(2,7)(3,11)(4,8)(6,10)(9,12)
g3 = perm([5, 7, 11, 8, 1, 10, 2, 4, 12, 6, 3, 9])

# (1,9,3,6)(5,12,11,10)
g4 = perm([9, 2, 6, 4, 12, 1, 7, 8, 3, 5, 10, 11])

# inds = [1,3,5,6,9,10,11,12]
fullH = FlagSOS.generateGroup([g1, g2, g3, g4], 16)



G12 == permute(G12, g4.d)

##
using GraphRecipes
using Plots
plotly()
# gr()

n = 12
A = coloring12
B = coloring12[[11, 6, 9, 7, 12, 2, 4, 10, 3, 8, 1, 5], [11, 6, 9, 7, 12, 2, 4, 10, 3, 8, 1, 5]]
# xPos = [i < n ? cos(i*2*pi/(n-1)) : 0 for i = 1:n]
# yPos = [i < n ? sin(i*2*pi/(n-1)) : 0 for i = 1:n]

# from wrong aut
xPos = vec([0 -5 0 5 -10 -5 5 10 0 10 0 -10])
yPos = vec([-8 2 5 -2 10 -2 2 -10 -5 10 8 -10])

# from correct aut
# still wrong
# yay
# guess: aut returns auts of canonically labeled graph + broken canonical labeling for CanGraphs
xPos = [-4, 0, 0, -3, 0, 0, 0, 0, 0, 4, 3, 0]
yPos = [0, 1, -3, 0, -4, -2, 3, 4, 2, 0, 0, -1]

#
xPos = [-4, 6, 4, 6, -3, 0, 6, 6, 0, 0, 3, 0]
yPos = [0, 1, 0, -1, 0, -4, 2, -2, 4, -3, 0, 3]


colors_draw = distinguishable_colors(n, [RGB(1, 1, 1), RGB(0, 0, 0)], dropseed=true)
cRange = colors_draw

p = graphplot(A, nodeshape=:circle, nodesize=1, names=1:12, edgewidth=(s, d, w) -> 3, x=xPos, y=yPos, edgecolor=Dict((c[1], c[2]) => A[c] == 0 ? nothing : cRange[A[c]] for c in eachindex(IndexCartesian(), A)), size=(1000, 1000))

# savefig(p, "G12.png")
## 3d plot?
using GLMakie

pos = rand(Point3f, 12)
pos[1] = Point3f(-1, 1, 1)
pos[9] = Point3f(1, 1, 1)
pos[3] = Point3f(1, -1, 1)
pos[6] = Point3f(-1, -1, 1)
pos[5] = Point3f(-1, 1, -1)
pos[12] = Point3f(1, 1, -1)
pos[11] = Point3f(1, -1, -1)
pos[10] = Point3f(-1, -1, -1)

# pos[2] = Point3f(3,0,0) + pos[9] #+ Point3f(0.222,0.444,0.666)
# pos[7] = Point3f(3,0,0) + pos[12] #+ Point3f(0.222,0.444,0.666)
# pos[4] = Point3f(3,0,0) + pos[6] #+ Point3f(0.222,0.444,0.666)
# pos[8] = Point3f(3,0,0) + pos[10] #+ Point3f(0.222,0.444,0.666)

pos[2] = Point3f(0, -0.5, 0.5)
pos[7] = Point3f(0, -0.5, -0.5)
pos[4] = Point3f(0, 0.5, 0.5)
pos[8] = Point3f(0, 0.5, -0.5)

ps = []
cs = []

A = coloring12

for i = 1:12
    for j = i+1:12
        if i in [2, 7, 4, 8] || j in [2, 7, 4, 8]
            # if i in [2,7] || j in [2,7]
            # if i in [4,8] || j in [4,8]
            # continue 
        end
        push!(ps, (pos[i], pos[j]))
        push!(cs, colors_draw[A[i, j]])
    end
end

f = Figure(size=(600, 650))
linesegments(f[1, 1], ps, color=cs, linewidth=4)

f
##

coloring8 = [0 1 2 3 4 5 6 7; 1 0 6 2 7 4 3 5; 2 6 0 7 5 1 4 3; 3 2 7 0 1 6 5 4; 4 7 5 1 0 3 2 6; 5 4 1 6 3 0 7 2; 6 3 4 5 2 7 0 1; 7 5 3 4 6 2 1 0]

G8 = CanGraph(coloring8)

score_coloring_countCol(3, 5, G8)
score_coloring(3, 5, G8)

GL8 = labelCanonically(G8)
H8 = aut(GL8)

##

coloring10 = [0 1 2 3 4 5 6 7 8 9; 1 0 3 9 5 8 4 2 6 7; 2 3 0 5 8 4 7 9 1 6; 3 9 5 0 1 6 8 4 7 2; 4 5 8 1 0 7 2 6 9 3; 5 8 4 6 7 0 9 3 2 1; 6 4 7 8 2 9 0 1 3 5; 7 2 9 4 6 3 1 0 5 8; 8 6 1 7 9 2 3 5 0 4; 9 7 6 2 3 1 5 8 4 0]

G10::CanGraph = CanGraph(coloring10)

score_coloring_countCol(3, 5, G10::CanGraph)
# score_coloring(3, 5, G8)

@timev GL10 = labelCanonically(G10::CanGraph)
@profview GL10 = labelCanonically(G10::CanGraph)
@profview_allocs GL10 = labelCanonically(G10::CanGraph)
H10 = aut(GL10)

using Profile

function myfunc()
    A = rand(2000, 200, 400)
    maximum(A)
end

myfunc()

@profile myfunc()

Profile.print()

##

@time tabuSearch_greedy(s, t, n, n / 100, 0.99999, nothing)
##
for _ in 1:Threads.nthreads()
    Threads.@spawn tabuSearch_greedy(s, t, n, 0.1, 0.9999, nothing)
end


##
using Profile
@profview tabuSearch(3, 5, 10, 10.0 / 10, 0.99, 5000)

##
for n = 11:16
    @time tabuSearch(s, t, n, n^2 / 100, 0.99, nothing)
end

##

F = CanGraph(Int[0 1 2; 1 0 3; 2 3 0])
G = CanGraph(Int[0 1 2; 1 0 3; 2 3 0])

isIsomorphic_memoized(F, G)