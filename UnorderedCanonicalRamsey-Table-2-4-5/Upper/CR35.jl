using FlagSOS
using FlagSOS: EdgeColoredGraph, isSubFlag


# CanGraph = EdgeColoredGraph{-1, true}
CanGraph = InducedFlag{EdgeColoredGraph{-1,true}}


## forbid orderable K3 and rainbow K5 

# Ramsey graphs
monoK3 = labelCanonically(CanGraph([0 1 1; 1 0 1; 1 1 0]))
orderableK3 = labelCanonically(CanGraph([0 2 1; 2 0 1; 1 1 0]))
rainbowK5 = labelCanonically(CanGraph([
    0 1 2 3 4;
    1 0 5 6 7;
    2 5 0 8 9;
    3 6 8 0 10;
    4 7 9 10 0
]))

# Blow-up graphs 
transitiveIndependence = labelCanonically(CanGraph([0 1 0; 1 0 0; 0 0 0]))
multiColorEdge = labelCanonically(CanGraph([0 1 2; 1 0 0; 2 0 0]))

forbiddenInduced = [transitiveIndependence, multiColorEdge]
forbiddenNonInduced = [monoK3, orderableK3, rainbowK5]

##

m = FlagModel{CanGraph,:limit,Rational{Int}}()
for F in forbiddenInduced
    addForbiddenFlag!(m, F)
end
for F in forbiddenNonInduced
    addForbiddenFlag!(m, F)
end

rM = addRazborovBlock!(m, 5);

@show modelSize(rM)
@show AbstractAlgebra.Partition([size(rM.blockSymmetry[b].pattern,1) for b in keys(rM.basis)])

##
edge = CanGraph([0 1; 1 0])

computeSDP!(m);

##
missingEdge = CanGraph([0 0; 0 0])

function CR35Allowed(G)
    # !CR35Allowed_NonInduced(G) && return false
    for F in forbiddenNonInduced
        if isSubFlag(F, G)
            return false
        end
    end
    for F in forbiddenInduced
        if isSubFlag(F, G)
            return false
        end
    end
    return true
end
##

I1 = CanGraph([0;;])
missingEdge3 = labelCanonically(missingEdge * I1)
filter!(x->CR35Allowed(x.first), missingEdge3.coeff)
missingEdge4 = labelCanonically(missingEdge3 * I1)
filter!(x->CR35Allowed(x.first), missingEdge4.coeff)
missingEdge5 = labelCanonically(missingEdge4 * I1)
filter!(x->CR35Allowed(x.first), missingEdge5.coeff)
# missingEdge6 = labelCanonically(missingEdge5 * I1)
# filter!(x->CR35Allowed(x.first), missingEdge6.coeff)

@assert missingEdge6 == labelCanonically(missingEdge6)
##

one1 = 1*I1
one2 = labelCanonically(one1 * I1)
filter!(x->CR35Allowed(x.first), one2.coeff)
one3 = labelCanonically(one2 * I1)
filter!(x->CR35Allowed(x.first), one3.coeff)
one4 = labelCanonically(one3 * I1)
filter!(x->CR35Allowed(x.first), one4.coeff)
# @timev one5 = labelCanonically(one4 * I1);
@timev one5 = labelCanonically(*(I1, one4; isAllowed = CR35Allowed));
filter!(x->CR35Allowed(x.first), one5.coeff)
# @timev one6 = *(I1, one5; isAllowed = CR35Allowed);
# filter!(x->CR35Allowed(x.first), one6.coeff)
# one6 = labelCanonically(one6)
# @assert one6 == labelCanonically(one6)


##
# m.objective = 1 * missingEdge5
# m.objective = 1 * missingEdge
# m.objective = -1 * edge

using Hypatia, JuMP, MosekTools
jm = buildJuMPModel(m)

# obj = 1 * missingEdge6
obj = 1 * missingEdge5
t = @variable(jm.model, base_name = "t")
for (G, c) in jm.variables
    if CR35Allowed(G) # || T() in keys(objL.coeff))
        @assert G == labelCanonically(G)
        # constraints[end][G] = 
        @show G 
        @show get(obj.coeff, G, 0)
        # @show get(one6.coeff, G, 0)
        # @constraint(jm.model, c == Float64.(get(obj.coeff, G, 0)) - t*Float64.(get(one6.coeff, G, 0)))
        @constraint(jm.model, c == Float64.(get(obj.coeff, G, 0)) - t*Float64.(get(one5.coeff, G, 0)))
    else
        @show G
        error("Shouldn't happen.")
    end
end

for (G,c) in obj.coeff 
    @assert haskey(jm.variables, G)
end
for (G,c) in one5.coeff 
# for (G,c) in one6.coeff 
    @assert haskey(jm.variables, G)
end

@objective(jm.model, Max, t)

set_optimizer(jm.model, Mosek.Optimizer)
optimize!(jm.model)
@show termination_status(jm.model)
@show objective_value(jm.model)

@show 1/objective_value(jm.model)

# jm.blocks
##
using LinearAlgebra


for (mu, B) in jm.constraints[1] 
    @show mu
    display(value.(B))
    display(dual.(B))
    @show rank(value.(B),  0.01)
    @show rank(dual.(B),  0.01)
end


##

using ClusteredLowRankSolver

P = buildClusteredLowRankModel(m)

bits = 2^8
setprecision(bits)
status, primalsol, dualsol, time, errorcode = solvesdp(P;
    # step_length_threshold=BigFloat(10e-14),
    duality_gap_threshold=1 / (BigFloat(2)^(bits / 4)));
objvalue(P, dualsol)

##
success, exactdualsol = exact_solution(P, primalsol, dualsol)
@assert success
objvalue(P, exactdualsol)

##
using Dualization
JuMP.write_to_file(dualize(jm.model), "test.dat-s")


##

for (mu, B) in rM.basis
    length(B) == 1 && continue
    @show mu 
    display.(B)
    println()
end

##

function CR35Allowed(G)
    # !CR35Allowed_NonInduced(G) && return false
    for F in forbiddenNonInduced
        if isSubFlag(F, G)
            return false
        end
    end
    for F in forbiddenInduced
        if isSubFlag(F, G)
            return false
        end
    end
    return true
end

I1 = CanGraph([0;;])
I2 = CanGraph([0 0; 0 0])
I3 = CanGraph([0 0 0; 0 0 0; 0 0 0])

(I1 * I1).coeff 
I1 * (I1 * I1) - (I1 * I1) * I1
I1*I2

labelCanonically(I1*I1*I1).coeff

missingEdge * (I1 * I1 * I1)

test = missingEdge * I1 * I1
test = labelCanonically(labelCanonically(labelCanonically(missingEdge * I1) * I1)*I1)


filter!(x -> CR35Allowed(x.first), test.coeff)

allowed(collect((I1 * I3).coeff)[1][1])

##

collect(rM.basis)[2][2]
Fs = collect(rM.basis)[5][2]
(Fs[1]*Fs[1]).coeff

##

F = EdgeColoredGraph{-1, true}(Int[0 1 1; 1 0 0; 1 0 0])
F = CanGraph(Int[0 1 1; 1 0 0; 1 0 0])
F = CanGraph(Int[0 1 2; 1 0 0; 2 0 0])

F = PartiallyLabeledFlag{typeof(F)}(F, 2)

F*F
(F*F).coeff

# error("Labels in wrong position")

##

F = EdgeColoredGraph{-1, true}(Int[0 1 2; 1 0 0; 2 0 0])

glue(F, F, [1,2,4])

error("Misses graph")

##
F = EdgeColoredGraph{-1, true}(Int[0 1 1 0 0; 1 0 0 1 1; 1 0 0 0 0; 0 1 0 0 0; 0 1 0 0 0])
permute(F, [2,3,1,4,5]).A
# permute(F, [3,1,2,4,5]).A
##

B = rM.basis[CanGraph()]

CR35Allowed(B[3].F)

##

missingEdge = CanGraph([0 0; 0 0])
F = CanGraph(Int[0 1 2; 1 0 3; 2 3 0])

@timev G = missingEdge*F;
##

missingEdge = PartiallyLabeledFlag{CanGraph}(CanGraph([0 0; 0 0]),0)
F = PartiallyLabeledFlag{CanGraph}(CanGraph(Int[0 1 2; 1 0 3; 2 3 0]),0)

@timev G = glue(missingEdge.F, F.F, [4,5,6]; isAllowed = CR35Allowed);

# (muc, maximum((m.blockSymmetry[mu]).pattern), length(B), mu) = (161, 6, 3, InducedFlag{EdgeColoredGraph{-1, true}}(∅ ))
# T1 = InducedFlag{EdgeColoredGraph{-1, true}}((3,{1-2}{1-3}{2-3}))
# T2 = InducedFlag{EdgeColoredGraph{-1, true}}((3,{1-2}{1-3}{2-3}))
# p1Fin = [4, 5, 6, 1, 2, 3]
F = CanGraph(Int[0 1 2; 1 0 3; 2 3 0])
@timev t = FlagSOS.glueFinite(:limit, F, F, [4,5,6,1,2,3]; labelFlags=true, isAllowed = CR35Allowed)
@timev t = FlagSOS.glue(F, F, [4,5,6,1,2,3]; isAllowed = CR35Allowed)

##

