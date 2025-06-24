using ProgressMeter
using LinearAlgebra
using SuiteSparseGraphBLAS
using SparseArrays
using SDPSymmetryReduction

function read_dats_file(filename, constants_rescale=1, constraint_from=0, constraint_to=10000000)
    # f = IOStream(read(filename, String))
    lines = split(read(filename, String), "\n")
    @info "File data in RAM."

    curline = 0
    nextline() = begin
        curline += 1
        lines[curline]
    end

    # Read number of constraints
    num_constraints = parse(Int, strip(nextline()))

    # Read number of blocks
    num_blocks = parse(Int, strip(nextline()))

    # Read block sizes
    block_sizes = parse.(Int, split(strip(nextline())))

    # Read constants
    parts = split(strip(nextline()))
    constants = Float64[]
    used_constraints = 0

    for i in 1:num_constraints
        if constraint_from <= i <= constraint_to
            value = parse(Float64, parts[i])
            push!(constants, value * constants_rescale)
            used_constraints += 1
        end
    end

    # Initialize blocks
    block_data = Vector{Any}(undef, length(block_sizes))
    for (j, size) in enumerate(block_sizes)
        if size < 0
            block_data[j] = [(rows=Int[], cols=Int[], values=Float64[], m=1, n=-size) for _ in 1:used_constraints+1]
        else
            block_data[j] = [(rows=Int[], cols=Int[], values=Float64[], m=size, n=size) for _ in 1:used_constraints+1]
        end
    end

    rows = Int[]
    cols = Int[]
    values = Float64[]

    nlines = length(lines)

    # Read data lines
    while curline < nlines
        
        if curline % floor(Int,nlines/100) == 0 
            println("Parsing data $curline/$nlines")
        end

        line = strip(nextline())
        if isempty(line)
            continue
        end

        parts = split(line)
        constraint = parse(Int, parts[1])
        block_num = parse(Int, parts[2])
        row = parse(Int, parts[3])
        column = parse(Int, parts[4])
        value = parse(Float64, parts[5]) * constants_rescale

        bd = block_data[block_num][constraint-constraint_from+1]
        if block_sizes[block_num] < 0
            push!(bd.rows, 1)
            push!(bd.cols, column)
            push!(bd.values, value)
        else
            push!(bd.rows, row)
            push!(bd.cols, column)
            push!(bd.values, value)
            if row != column
                push!(bd.rows, column)
                push!(bd.cols, row)
                push!(bd.values, value)
            end
        end
    end

    @info "read data, forming blocks"

    blocks = [[GBMatrix(bd.rows, bd.cols, bd.values, bd.m, bd.n) for bd in B] for B in block_data]

    # Return problem dictionary
    return Dict(
        :num_constraints => used_constraints,
        :num_blocks => num_blocks,
        :block_sizes => block_sizes,
        :blocks => blocks,
        :constants => constants
    )
end


function write_dats_file(filename, problem)
    open(filename, "w") do f
        num_constraints = problem[:num_constraints]
        num_blocks = problem[:num_blocks]
        block_sizes = problem[:block_sizes]
        blocks = problem[:blocks]
        constants = problem[:constants]

        # Write the number of constraints and blocks
        write(f, "$num_constraints\n")
        write(f, "$num_blocks\n")

        # Write block sizes
        write(f, join(block_sizes, " ") * "\n")

        # Write constants

        write(f, join(constants, " ") * "\n")

        # Helper function to print a row if the value is non-zero
        function print_row(constraint, block_num, row, column, value)
            if value != 0
                #rounded_value = round(value)
                #if abs(rounded_value-value) < 1e-7
                #    write(f, "$constraint $(block_num + 1) $(row + 1) $(column + 1) $(rounded_value)\n")
                #else
                write(f, "$constraint $(block_num + 1) $(row + 1) $(column + 1) $(value)\n")
                #end
            end
        end

        # Write matrix entries
        @showprogress for constraint in 0:num_constraints
            for block_num in 1:num_blocks
                block_size = block_sizes[block_num]
                if block_size < 0
                    # Diagonal entries in the 1 x (-block_size) block
                    for column in 1:-block_size
                        row = column
                        value = blocks[block_num][constraint+1][1, column]
                        print_row(constraint, block_num - 1, row - 1, column - 1, value)
                    end
                else
                    matrix = blocks[block_num][constraint+1]
                    if issparse(matrix)
                        # Handle sparse matrix
                        rows, cols, values = findnz(matrix)
                        for (row, column, value) in zip(rows, cols, values)
                            if column >= row
                                print_row(constraint, block_num - 1, row - 1, column - 1, value)
                            end
                        end
                    else
                        # Handle dense matrix
                        for row in 1:size(matrix, 1)
                            for column in row:size(matrix, 2)
                                value = matrix[row, column]
                                print_row(constraint, block_num - 1, row - 1, column - 1, value)
                            end
                        end
                    end
                end
            end
        end
    end
end



##


function read_tap_matrices(filename)
    matrices = []  # Array to store the matrices
    sizes = []     # Array to store the sizes of matrices
    matrix_lines = []  # Temporary storage for the current matrix lines
    reading_matrices = false
    block_size = 0

    open(filename, "r") do file
        for line in eachline(file)
            line = strip(line)  # Remove leading/trailing whitespace

            if startswith(line, "#") || isempty(line)
                continue  # Ignore comment lines
            end

            if !reading_matrices
                # First non-comment line contains the sizes
                block_size = parse.(Int, line)
                #println("Having size $block_size")
                push!(sizes, block_size)
                if block_size > 0
                    reading_matrices = true
                else
                    push!(matrices, [])
                end
                continue
            end

            # Process matrix rows
            row = parse.(Int, split(line))

            push!(matrix_lines, row)

            #println("Have row $row")
            #println("Have matrix_lines $matrix_lines")
            #println("Have matrices $matrices")

            # Check if we've read enough rows for the current matrix
            if length(matrix_lines) == block_size
                #println("Have matrix_lines $matrix_lines")
                push!(matrices, hcat(matrix_lines...))  # Combine rows into a matrix
                #push!(matrices, matrix_lines)  # Combine rows into a matrix
                reading_matrices = false
                #sizes = sizes[2:end]  # Move to the next size
                matrix_lines = []  # Reset for the next matrix
            end

            if length(sizes) == 0
                break
            end
        end
    end

    return matrices
end


# Now we make a projection

function make_projections(dats, Qs)
    # These are not changing 
    num_constraints = dats[:num_constraints]
    constants = dats[:constants]

    num_blocks = 0
    block_sizes = []
    blocks = []

    println("Diagonalizing ")
    @showprogress for block_id in 1:dats[:num_blocks]

        # If we run out of Qs or there is just one Q, we do nothing
        # if block_id > length(Qs) || length(Qs[block_id]) <= 1
        if !haskey(Qs, block_id) || length(Qs[block_id]) <= 1
            num_blocks += 1
            push!(block_sizes, dats[:block_sizes][block_id])
            push!(blocks, dats[:blocks][block_id])
            continue
        end

        for (j, Q) in enumerate(Qs[block_id])

            Q_GB = GBMatrix(Q)

            num_blocks += 1
            push!(block_sizes, size(Q)[2])

            # push!(blocks, [  SDPSymmetryReduction.conjugate( B, Q) for B in dats[:blocks][block_id]] )

            # push!(blocks, [Q' * B * Q for B in dats[:blocks][block_id]])
            # push!(blocks, [spzeros(size(Q, 2), size(Q, 2)) for B in dats[:blocks][block_id]])
            push!(blocks, Union{Missing,GBMatrix}[missing for B in dats[:blocks][block_id]])

            Threads.@threads for i in eachindex(dats[:blocks][block_id])
                blocks[end][i] = Q_GB' * (dats[:blocks][block_id][i] * Q_GB)
                # blocks[end][i] = Q' * dats[:blocks][block_id][i] * Q
            end
        end

    end

    return Dict(
        :num_constraints => num_constraints,
        :num_blocks => num_blocks,
        :block_sizes => block_sizes,
        :blocks => blocks,
        :constants => constants
    )
end

#SDPSymmetryReduction.conjugate( dats[:blocks][1][10], Qs[1][2] )



# And here is the main program

function main()
    # Check if an argument is provided
    filename_dats = "SDP_n5_UB_F_edges2__objective.txt.dat-s"
    # filename_dats = "SDP_n6_UB_F_3edges__objectivee.txt.dat-s"
    # filename_dats = "SDP_n8_LB_F_edges2blind__objectivee.txt.dat-s"
    # filename_dats = "SDP_n9_UB_F_edges2__objective.txt.dat-s"


    if length(ARGS) < 1
        println("Usage: julia script.jl dat-s.file")
        #return  # Exit gracefully
    else
        filename_dats = ARGS[1]
    end

    println("Using file $filename_dats")

    filename_tap = filename_dats * ".tap"
    filename_tap_dats = filename_tap * ".dat-s"

    # Get projections
    println("Loading Type Automorphisim Partition from $filename_tap")
    P_matrices = read_tap_matrices(filename_tap)

    Qs = []
    println("Calculation projection matrices Qs")
    @showprogress for P in P_matrices
        if length(P) > 0
            P_partition = SDPSymmetryReduction.Partition(P)
            #println("Diagonalizing matrix of size ",length(P))
            push!(Qs, SDPSymmetryReduction.diagonalize(Float64, P_partition))
        else
            #println("Skipping matrix of size ",length(P))
            push!(Qs, [])
        end

    end

    #P_partitions = [ SDPSymmetryReduction.Partition(P) for P in P_matrices ]

    #Qs = [ SDPSymmetryReduction.diagonalize(Float64, P) for P in P_partitions ]

    # read dat-s file
    println("Reading file $filename_dats")
    dats = read_dats_file(filename_dats)

    datsP = make_projections(dats, Qs)

    filename_tap_dats = filename_tap * ".dat-s"
    println("Writing output to $filename_tap_dats")
    write_dats_file(filename_tap_dats, datsP)

    # Perform some operation
    println("Next step is to run")
    println("csdp $filename_tap_dats ")
end


#


# Call the main function
# main()

