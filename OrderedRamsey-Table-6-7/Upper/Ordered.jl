using Combinatorics, FlagSOS, AbstractAlgebra

# Upper bounds for Table 6

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

function extend_allowed_ways_Ordered!(res::Vector{Matrix{Int}}, C, n, A, cur_pos=1; limit=Inf)
    if cur_pos >= n
        if length(res) >= limit
            return res
        end
        push!(res, deepcopy(C))
        return res
    end

    m = size(A, 1)
    # forbidden_check = zeros(Int, m, m)

    inds = collect(1:m)
    inds[end] = n

    for c in 1:2
        C[n, cur_pos] = c
        C[cur_pos, n] = c

        C_is_allowed = true


        # check if allowed!
        for d in combinations(1:n-1, m - 1)
            @views inds[1:end-1] .= d
            forbidden_check = @view C[inds, inds]

            has_gaps = false
            found_col = -1
            for c in eachindex(A)
                if !A[c]
                    # forbidden_check[c] = 0
                    continue
                end
                if forbidden_check[c] == 0
                    has_gaps = true
                    break
                end
                if found_col == -1
                    found_col = forbidden_check[c]
                elseif found_col != forbidden_check[c]
                    has_gaps = true  # not mono A
                    break
                end
            end
            has_gaps && continue
            C_is_allowed = false # found mono A
            break
        end
        !C_is_allowed && continue

        extend_allowed_ways_Ordered!(res, C, n, A, cur_pos + 1; limit=limit)
        C[n, cur_pos+1:end] .= 0
        C[cur_pos+1:end, n] .= 0

        if length(res) >= limit
            return res
        end

    end
    res
end

function generate_allowed_graphs_Ordered(n::Int, A::BitMatrix; limit=Inf)
    if n == 1
        return Matrix{Int}[zeros(Int, 1, 1)]
    end

    # smaller_graphs = Matrix{Int}
    res = generate_allowed_graphs_Ordered(n - 1, A; limit=limit)

    # forbidden = forbidden_graphs_ER(A)
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
        # res = vcat(res, extend_allowed_ways_Ordered(C, n, A, forbidden))
        extend_allowed_ways_Ordered!(res, C, n, A; limit=limit)
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
# display.(forbidden_graphs_ER(Kn))


# @profview res = generate_allowed_graphs_Ordered(5, Kn);
@timev res = generate_allowed_graphs_Ordered(3, Kn; limit=100);
@timev res = generate_allowed_graphs_Ordered(5, Kn);
length(res)
@assert res == unique(res)

@timev res = generate_allowed_graphs_Ordered(5, Kn; limit=100);
res2 = generate_blow_ups(res[2:2], 3)

##

P4 = BitMatrix(Bool[
    0 1 0 0
    1 0 1 0
    0 1 0 1
    0 0 1 0])
res = generate_allowed_graphs_Ordered(4, P4)

##

limit = 1_000_000
lvl = 7

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
    res = generate_allowed_graphs_Ordered(lvl, G; limit=limit)
    ramsey_graphs_full[G] = res
    noGraphs = length(res)
    max_verts = size(res[end], 1)
    # @show G, noGraphs, max_verts 
    if noGraphs < limit
        if max_verts < lvl
            @info "R_ord($G) = $(max_verts+1)"
            ramsey_bounds[G] = (max_verts + 1, max_verts + 1)
        else
            @info "R_ord($G) computable on $lvl verts with $noGraphs graphs"
            ramsey_bounds[G] = (max_verts + 1, Inf)
        end
        ramsey_graphs[G] = res
    else
        res_smaller = [G for G in res if size(G, 1) < max_verts]
        biggest = size(res_smaller[end], 1)
        @info "R_ord($G) computable on $biggest verts with $(length(res_smaller)) graphs"
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

open("Ordered/bounds.txt", "w") do io
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
    folder = "Ordered/$folder_name/"
    !isdir(folder) && mkdir(folder)
    open(folder * "/F_ordered_edges3partition__objectivee.txt", "w") do io
        println(io, "1 2 0 1")
    end
    # forbidden = forbidden_graphs_ER(G)
    open(folder * "/F_ordered_edges3partition__forbidden.txt", "w") do io
        # for A in forbidden # all but the blow-up forbidden graphs
        println(io, adj_to_lidicky(G; induced=false))
        # end
    end

    # only if we managed to generate all!
    allowed = ramsey_graphs[G]
    max_size = maximum(x -> size(x, 1), allowed)
    blow_ups = generate_blow_ups(allowed, max_size)
    @show length(allowed), length(blow_ups)
    for lvl in size(G, 1):max_size
        blow_ups_lvl = filter(x -> size(x, 1) <= lvl, blow_ups)
        if length(blow_ups_lvl) > 1_000_000
            @info "Too many blow-ups for $G at lvl $lvl"
            continue
        end

        open(folder * "/F_ordered_edges3partition__n$(lvl)_unlabeled.txt", "w") do io
            println(io, "0 0")
            for A in blow_ups_lvl
                println(io, adj_to_lidicky(A; induced=true))
            end
        end
    end
end

## for table output

function upper_triangle_vec(x)
    res = Int[]
    n = size(x, 1)
    for i = 1:n
        for j = i+1:n
            push!(res, x[i, j])
        end
    end
    return res
end

G_iso = unique(labelCanonically(Graph.(keys(ramsey_bounds))))
sort!(G_iso, by=x -> (size(x), countEdges(x)))

G_sorted = [
    G => sort!([H for H in keys(ramsey_bounds) if labelCanonically(Graph(H)) == G]; by=upper_triangle_vec)
    for G in G_iso
]


# sorting ramsey bounds 
# ramsey_bounds_sorted = collect(ramsey_bounds)
# sort!(ramsey_bounds_sorted, by=x -> (size(x[1], 1), vec(x[1])))

cd(@__DIR__)
open("Ordered/table.tex", "w") do io
    println(io, "\\begin{table}")
    println(io, "\\centering")
    print(io, "\t\\begin{tabular}{")
    print(io, "l|ccccccccc")
    println(io, "}\\toprule")
    # println(io, "\t\t G & LB & UB Enum & UB Flag Algebra\\\\\\midrule")

    for (H, Gs) in G_sorted
        print(io, "\t\t$H")

        for G in Gs
            (l, u) = ramsey_bounds[G]

            print(io, " & \\ramseybound{")
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
            print(io, "}{$l}{$u}")
        end
        println(io, "\\\\\\midrule")
    end
    println(io, "\t\\bottomrule")
    println(io, "\t\\end{tabular}")
    println(io, "\\end{table}")
end

