using Combinatorics, FlagSOS, AbstractAlgebra

# computes the blow-up of the matrix A, blowing up the rows and columns of A according to the sizes
function blow_up(A::Matrix{Int}, sizes::Vector{Int})
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
function generate_blow_ups(Gs::Vector{Matrix{Int}}, n)
    res = Matrix{Int}[]
    for G in Gs
        m = size(G, 1)
        push!(res, G)
        for m2 = m+1:n
            if m == 0
                push!(res, zeros(Int, m2, m2))
            else
                for sizes in sum_n_vectors(m, m2)
                    # @show sizes
                    push!(res, blow_up(G, sizes))
                end
            end
        end
    end
    return unique!(res)
end

function remove_zeros(A::BitMatrix)
    n = size(A, 1)
    keep = A * ones(n) .!= 0
    return A[keep, keep]
end

function generate_ordered_graphs(n)
    # generate all symmetric 0/1 matrices?
    res = BitMatrix[]
    push!(res, BitMatrix(zeros(n, n)))
    inds = collect(combinations(1:n, 2))
    for c in combinations(inds)
        A = BitMatrix(zeros(n, n))
        for (i, j) in c
            A[i, j] = 1
            A[j, i] = 1
        end
        push!(res, A)
    end
    return sort!(by=x -> size(x, 1), unique!(remove_zeros.(res)))
end

function forbidden_graphs_ER(A::BitMatrix)
    n = size(A, 1)

    res = Matrix{Int}[]
    push!(res, Int.(A)) # Mono

    orderable = Int.(A)
    for i = 1:n
        orderable[i, i:end] .*= i
        orderable[i:end, i] .*= i
    end
    push!(res, FlagSOS.sortEntries!(orderable))

    orderable = Int.(A)
    for i = 1:n
        orderable[i, 1:i-1] .*= i
        orderable[1:i-1, i] .*= i
    end
    push!(res, FlagSOS.sortEntries!(orderable))

    rainbow = zeros(Int, n, n)
    current_color = 1
    for c in findall(A)
        c[1] > c[2] && continue
        rainbow[c] = current_color
        rainbow[c[2], c[1]] = current_color
        current_color += 1
    end
    push!(res, FlagSOS.sortEntries!(rainbow))

    return unique!(res)
end

# checks if A is not mono, lex or rainbow. Ignores 0 entries
function allowed_ER(A::Matrix{Int}; found=BitSet(), foundCol=BitSet())
    # @show A
    n = size(A, 1)
    mono = true
    rainbow = true
    lex = true
    upperLex = true

    for i = 1:n
        for j = i+1:n
            c = A[i, j]
            c == 0 && continue
            if c in found
                rainbow = false
            elseif !isempty(found)
                mono = false
            end
            push!(found, c)
            if !isempty(foundCol) && !(c in foundCol)
                # @show foundCol
                lex = false
            end
            push!(foundCol, c)
            rainbow || mono || lex || break # only reached if all three are false
        end
        rainbow || mono || lex || break # only reached if all three are false
        empty!(foundCol)
    end
    empty!(foundCol)
    for i = 1:n
        for j = 1:i-1
            c = A[i, j]
            c == 0 && continue
            if !isempty(foundCol) && !(c in foundCol)
                # @show foundCol
                upperLex = false
            end
            push!(foundCol, c)
            rainbow || mono || lex || upperLex || return true # only reached if all three are false
        end
        empty!(foundCol)
    end
    # @show mono, rainbow, lex
    return false
end

function allowed_ER_strict_lex(A::Matrix{Int}; found=BitSet(), foundCol=BitSet())
    # @show A
    n = size(A, 1)
    mono = true
    rainbow = true
    lex = true
    upperLex = true
    nonzeroRows = 0
    nonzeroCols = 0

    for i = 1:n
        for j = i+1:n
            c = A[i, j]
            c == 0 && continue
            if isempty(foundCol)
                nonzeroRows += 1
            end
            if c in found
                rainbow = false
            elseif !isempty(found)
                mono = false
            end
            push!(found, c)
            if !isempty(foundCol) && !(c in foundCol)
                # @show foundCol
                lex = false
            end
            push!(foundCol, c)
            rainbow || mono || lex || break # only reached if all three are false
        end
        rainbow || mono || lex || break # only reached if all three are false
        empty!(foundCol)
    end
    empty!(foundCol)
    for i = 1:n
        for j = 1:i-1
            c = A[i, j]
            c == 0 && continue
            if isempty(foundCol)
                nonzeroCols += 1
            end
            if !isempty(foundCol) && !(c in foundCol)
                # @show foundCol
                upperLex = false
            end
            push!(foundCol, c)
            rainbow || mono || lex || upperLex || return true # only reached if all three are false
        end
        empty!(foundCol)
    end

    if mono || rainbow 
        return false 
    end

    nc = length(found)

    if lex && nonzeroRows == nc
        return false 
    end 

    if upperLex && nonzeroCols == nc
        return false 
    end 

    return true
    # @show mono, rainbow, lex
end

function extend_allowed_ways_ER!(res::Vector{Matrix{Int}}, C, n, A, forbidden, cur_pos=1; limit=Inf)
    if cur_pos >= n
        if length(res) >= limit
            return res
        end
        push!(res, deepcopy(C))
        return res
    end

    m = size(A, 1)
    forbidden_check = zeros(Int, m, m)

    max_color = maximum(C)
    inds = collect(1:m)
    inds[end] = n

    found = BitSet()
    foundCol = BitSet()

    for c in 1:max_color+1
        C[n, cur_pos] = c
        C[cur_pos, n] = c

        C_is_allowed = true


        # check if allowed!
        for d in combinations(1:n-1, m - 1)
            @views inds[1:end-1] .= d
            @views forbidden_check .= C[inds, inds]

            has_gaps = false
            for c in eachindex(A)
                if !A[c]
                    forbidden_check[c] = 0
                    continue
                end
                if forbidden_check[c] == 0
                    has_gaps = true
                    break
                end
            end
            has_gaps && continue
            # FlagSOS.sortEntries!(forbidden_check)
            # for F in forbidden
            #     if F == forbidden_check
            #         C_is_allowed = false
            #     end
            #     !C_is_allowed && break
            # end
            # @show C, d, forbidden_check, C_is_allowed, allowed_ER(forbidden_check)
            # @assert C_is_allowed == allowed_ER(forbidden_check)
            empty!(found)
            empty!(foundCol)
            # C_is_allowed = allowed_ER(forbidden_check; found=found, foundCol=foundCol)
            C_is_allowed = allowed_ER_strict_lex(forbidden_check; found=found, foundCol=foundCol)
            !C_is_allowed && break
        end
        !C_is_allowed && continue

        extend_allowed_ways_ER!(res, C, n, A, forbidden, cur_pos + 1; limit=limit)
        C[n, cur_pos+1:end] .= 0
        C[cur_pos+1:end, n] .= 0

        if length(res) >= limit
            return res
        end

    end
    res
end

function generate_allowed_graphs_ER(n::Int, A::BitMatrix; limit=Inf)
    if n == 1
        return Matrix{Int}[zeros(Int, 1, 1)]
    end

    # smaller_graphs = Matrix{Int}
    res = generate_allowed_graphs_ER(n - 1, A; limit=limit)

    forbidden = forbidden_graphs_ER(A)
    # res = Matrix{Int}[]

    # ℓ = findfirst(x->size(x,1) == n-1, res)
    # k = length(res)

    # for B in res[ℓ:k]
    # for B in smaller
    for B in res
        size(B, 1) < n - 1 && continue
        size(B, 1) > n - 1 && break

        C = zeros(Int, n, n)
        C[1:n-1, 1:n-1] .= B
        # res = vcat(res, extend_allowed_ways_ER(C, n, A, forbidden))
        extend_allowed_ways_ER!(res, C, n, A, forbidden; limit=limit)
    end

    return res
end

# A = rand(generate_ordered_graphs(4))
# display.(forbidden_graphs(A))

##

n = 4
graphs = generate_ordered_graphs(n)[2:end]

filter!(x -> vec(x) <= vec(reverse(x, dims=(1, 2))), graphs)

# filter!(x->FlagSOS.isBipartite(Graph(x)), graphs)
# filter!(x->length(connectedComponents(Graph(x))) == 1 , graphs)

Kn = graphs[end]
display.(forbidden_graphs_ER(Kn))


# @profview res = generate_allowed_graphs_ER(5, Kn);
@timev res = generate_allowed_graphs_ER(3, Kn; limit=100);
@timev res = generate_allowed_graphs_ER(6, Kn);
length(res)
@assert res == unique(res)

@timev res = generate_allowed_graphs_ER(5, Kn; limit=100);
res2 = generate_blow_ups(res[2:2], 3)

##

P4 = BitMatrix(Bool[
    0 1 0 0
    1 0 1 0
    0 1 0 1
    0 0 1 0])
res = generate_allowed_graphs_ER(4, P4)

##

limit = 100_000
lvl = 9

# all generated graphs
ramsey_graphs_full = Dict()

# pairs of upper and lower bounds
ramsey_bounds = Dict()

# maximum full sets of graphs totalling less than limit
ramsey_graphs = Dict()

# using LinearAlgebra
# G = [0 2 1 2
#     2 0 2 1
#     1 2 0 2
#     2 1 2 0]
# G .-= 1
# G[diagind(G)] .= 0
# G = BitMatrix(G)

for G in graphs
    res = generate_allowed_graphs_ER(lvl, G; limit=limit)
    ramsey_graphs_full[G] = res
    noGraphs = length(res)
    max_verts = size(res[end], 1)
    # @show G, noGraphs, max_verts 
    if noGraphs < 100_000
        if max_verts < lvl
            @info "ER($G) = $(max_verts+1)"
            ramsey_bounds[G] = (max_verts + 1, max_verts + 1)
        else
            @info "ER($G) computable on $lvl verts with $noGraphs graphs"
            ramsey_bounds[G] = (max_verts + 1, Inf)
        end
        ramsey_graphs[G] = res
    else
        res_smaller = [G for G in res if size(G, 1) < max_verts]
        biggest = size(res_smaller[end], 1)
        @info "ER($G) computable on $biggest verts with $(length(res_smaller)) graphs"
        ramsey_bounds[G] = (max_verts + 1, Inf)
        ramsey_graphs[G] = res_smaller
    end

end

##

# writes the adjacency matrix A in Bernard's format:
# vectorized upper triangle
# 1 = non edge
# 2,3,4... = colors
function adj_to_lidicky(A; induced=true)
    lid = ""
    n = size(A, 1)
    lid *= "$(n) 0  "
    for i = 1:n
        for j = i+1:n
            c = A[i, j] + 1
            if c == 1 && !induced
                c = ">1"
            end
            lid *= "$c"
            lid *= " "
        end
        if i != n
            lid *= " "
        end
    end
    return lid
end

open("ER/bounds_strict_lex.txt", "w") do io
    for (G, (l, u)) in ramsey_bounds
        if l == u
            println(io, "$(adj_to_lidicky(G)) = $l")
            noGraphs = count(x -> size(x, 1) == u - 1, ramsey_graphs_full[G])
            @show G, l, noGraphs
        else
            println(io, "$(adj_to_lidicky(G)) >= $l")
        end
    end
end

for (G, (l, u)) in ramsey_bounds
    l == u && continue
    G_lid = adj_to_lidicky(G)
    folder_name = replace(G_lid, " " => "")
    folder = "ER/strict/$folder_name/"
    !isdir(folder) && mkdir(folder)
    open(folder * "/F_ordered_edges24partition__objectivee.txt", "w") do io
        println(io, "1 2 0 1")
    end
    forbidden = forbidden_graphs_ER(G)
    open(folder * "/F_ordered_edges24partition__forbidden.txt", "w") do io
        for A in forbidden # all but the blow-up forbidden graphs
            println(io, adj_to_lidicky(A; induced=false))
        end
    end

    # only if we managed to generate all!
    allowed = ramsey_graphs[G]
    max_size = maximum(x -> size(x, 1), allowed)
    blow_ups = generate_blow_ups(allowed, max_size)
    @show length(allowed), length(blow_ups)
    for lvl in size(G, 1):max_size
        blow_ups_lvl = filter(x -> size(x, 1) <= lvl, blow_ups)

        open(folder * "/F_ordered_edges24partition__n$(lvl)_unlabeled.txt", "w") do io
            println(io, "0 0")
            for A in blow_ups_lvl
                println(io, adj_to_lidicky(A; induced=true))
            end
        end
    end
end

## for table output

# sorting ramsey bounds 
ramsey_bounds_sorted = collect(ramsey_bounds)
sort!(ramsey_bounds_sorted, by=x -> (size(x[1], 1), vec(x[1])))

cd(@__DIR__)
open("ER/table_strict.tex", "w") do io
    println(io, "\\begin{table}")
    println(io, "\\centering")
    print(io, "\t\\begin{tabular}{")
    print(io, "l|ccc")
    println(io, "}\\toprule")
    println(io, "\t\t G & LB & UB Enum & UB Flag Algebra\\\\\\midrule")
    for (G, (l, u)) in ramsey_bounds_sorted
        print(io, "\t\t")
        n = size(G, 1)
        if n == 2
            print(io, "\\Orderedtwo")
        elseif n == 3
            print(io, "\\Orderedthree")
        elseif n == 4
            print(io, "\\Orderedfour")
        else
            @error "TODO"
        end
        for i = 1:n
            for j = i+1:n
                print(io, Int(G[i, j]))
            end
        end
        print(io, " & ")
        if l == u
            print(io, "\\sharpbound{$l} & \\sharpbound{$u} ")
        else
            print(io, "$l & ")
        end
        println(io, "& \\\\\\hline")
    end
    println(io, "\t\\bottomrule")
    println(io, "\t\\end{tabular}")
    println(io, "\\end{table}")
end

