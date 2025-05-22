#!/bin/usr/sage 
r"""
This is program written in sage for rounding exact solutions from flag algebra computations.
Does not do everything, but can do something if you are lucky.
In general, you need to provide
 - SDP program  dat-s and its solution dat-s.result from CSDP
 - eigenvectors corresponding to zero eigenvalues of matrices in the solution.

NOTE: SDP program needs to have ALL entries integers. So before rounding, you need
to multiply the objective function or constraints accordingly. This is because the
rounding is done over integers to avoid rounding errors.

The rounding procedure is trying to guess graphs, where the CSDP program
must be tight. Usually, subgraphs of the extremal example. You may provide them
or hope the program can guess them right.

Few more notes:
 - solutions can be seen as matrices but also as one long vector
   during rounding, we look at the solution as a long vector.


cat altered.dat-s.solution | sed -n '/yMat/,$p' | sed 's/}$/],/g' | sed 's/}/]/g'| sed 's/{/[/g'  | grep -v '=' | sed 's/^ *\[ *$/yMat = \[/' | sed 's/^],$/]/' > altered.dat-s.solution.py
 
 Some description of the rounding procedure:
 Problem:
 csdp is solving problem like:
    max C.X
    subject to A_1.X = b_1
               A_2.X = b_2
               A_mdim.X = b_mdim
    where X is positive semidefinite matrix
 We actually solve it as
 max t 
 subject to 
    A_1.X = b_1
    A_2.X = b_2
 A_mdim.X = b_mdim
 where X is positive semidefinite matrix

   b_i + A_i X >= t
 
 //      max   -t
 //      s.t.  [A.X]_i + s_i - t = -D_i 
 

load('../rounding_exact.sage') 
p=FAProblem('SDP_n6_UB_F_3edges2__objective.txt.dat-s')
zv=[]
pp=p.change_base(zv)
write_problem_file('altered.dat-s',pp)

solve_using_csdp("altered.dat-s")
 
pa=FAProblem('altered.dat-s')
pa.perform_rounding(objective)

write_rounded_solution_to_file(pa)
 

uf=load_unlabeled_flags("F_vertices2_4edges__n7_unlabeled.txt",7)

[ uf[id] for id in pa.tightgraphsIDS ]

check_rounded_solution('altered.dat-s')

"""

import numpy
import subprocess
import sys
import pickle  # for saving/loading the rounded matrices


field=Integer

def rescale_number(x, constants_rescale, kill_if_fail=True):
    """ Rescaling and testing that rescaling indeed gives an integer"""
    
    if constants_rescale == 1:
        return field(x)
    
    x_float = float(x)*constants_rescale
    x_rounded = round(x_float)
    if abs(x_float - x_rounded) > 0.0000001:
        print("Rescaling did not work for x=",x," result was",x_rounded,"which is not precise for",x_float)
        print("Try a different rescaling of the semidefinite program.")
        if kill_if_fail:
            sys.exit()
 
    return field(x_rounded)
#field(int(float(parts[i])*constants_rescale+0.5))


################################################################################### UTILITIES
# Reads the whole problem as integer program
# the program has to be integral. This is a hack
# for SDP programs that were solved without scaled objective
# and are expensive to resolve
def read_problem_file(filename,constants_rescale=1,constraint_from=0, constraint_to=10000000):
    f = open(filename, 'r')

    line = f.readline()
    line = line.strip()
    num_constraints = int(line)

    line = f.readline()
    line = line.strip()
    num_blocks = int(line)

    line = f.readline()
    parts = line.split()
    block_sizes = []
    for part in parts:
        block_sizes.append(int(part))

    line = f.readline()
    parts = line.split()

    constants = []
    used_constraints=0

    for i in range(num_constraints):
        if i >= constraint_from and i < constraint_to:
            constants.append(rescale_number(parts[i], constants_rescale))
            used_constraints += 1
    """            
    if constants_rescale == 1:
        for i in xrange(num_constraints):
           if i >= constraint_from and i < constraint_to:
               constants.append(field(parts[i]))
               used_constraints += 1
    else:
        for i in xrange(num_constraints):
           if i >= constraint_from and i < constraint_to:
               constants.append(field(int(float(parts[i])*constants_rescale+0.5)))
               used_constraints += 1
    """

    # blocks that are integer numbers right away
    blocks = []
    for size in block_sizes:
        if size < 0:
            #block = [ matrix(ZZ,1,-size) for x in range(num_constraints+1) ]
            block = [ matrix(ZZ,1,-size) for x in range(used_constraints+1) ]
        else:
            block = [ matrix(ZZ,size,size, sparse=True) for x in range(used_constraints+1) ]
            #block = [ matrix(ZZ,size,size, sparse=True) for x in range(num_constraints+1) ]
            #block = [ matrix(QQ,size,size) for x in range(num_constraints+1) ]
        blocks.append(block)


    while True:
        line = f.readline()
        if len(line) == 0:
            break
        parts = line.split()
        if len(parts) == 0:
            continue
        constraint = int(parts[0])
        # Read also a bit arround to accommodate for mistakes..
        # Blocks are shifted by one
        #if constraint <= constraint_from or constraint > constraint_to:
        #    continue
        block_num = int(parts[1])
        row = int(parts[2])
        column = int(parts[3])
        value = rescale_number(parts[4], constants_rescale)
        '''
        if constants_rescale == 1:
            value = field(parts[4])
        else:
            value = field(float(parts[4])*constants_rescale)
        '''
        if block_sizes[block_num - 1] < 0:
            blocks[block_num - 1][constraint-constraint_from][0, column - 1] += value
        else:
            blocks[block_num - 1][constraint-constraint_from][row - 1, column - 1] += value
            if row != column:
                blocks[block_num - 1][constraint-constraint_from][column - 1, row - 1] += value

    problem = dict()
    problem['num_constraints'] = used_constraints
    problem['num_blocks'] = num_blocks
    problem['block_sizes'] = block_sizes
    problem['blocks'] = blocks
    problem['constants'] = constants
    f.close()
    return problem


def write_problem_file(filename, problem):
    f = open(filename, 'w')

    num_constraints = problem['num_constraints']
    num_blocks = problem['num_blocks']
    block_sizes = problem['block_sizes']
    blocks = problem['blocks']
    constants = problem['constants']

    f.write("%d\n" % num_constraints)
    f.write("%d\n" % num_blocks)
    for size in block_sizes:
        f.write("%d " % size)
    f.write("\n")

    for constant in constants:
        f.write("%.20g " % constant)
    f.write("\n")

    def print_row():
        if value != 0:
            f.write("%d %d %d %d %.20g\n" % (constraint, block_num + 1, row + 1, column + 1, value))

    for constraint in range(num_constraints + 1):
        for block_num in range(num_blocks):
            block_size = block_sizes[block_num]
            if block_size < 0:
                for column in range(-block_size):
                    row = column
                    #print 'Writing',block_num,column,constraint
                    value = blocks[block_num][constraint][0,column]
                    print_row()
            else:
                d = blocks[block_num][constraint].dict()
                #d = blocks[block_num][constraint].dict(copy=False)
                for entry in d:
                    row = entry[0]
                    column = entry[1]
                    if column < row:
                        continue
                    value = d[entry]
                    print_row()
                #                for row in xrange(block_size):
                #                    for column in xrange(row, block_size):
                #                        value = blocks[block_num][constraint][row,column]
                #                        print_row()

    f.close()



# This reads the solution file - but only the primal. Dual is being ignored
# and it would waste space anyway. The problem is read as real numbers - because
# we 
def read_solution_file(filename, problem):
    f = open(filename, 'r')

    num_constraints = problem['num_constraints']
    num_blocks = problem['num_blocks']
    block_sizes = problem['block_sizes']

    Y = numpy.zeros([num_constraints], float)
    line = f.readline()
    parts = line.split()
    for i in range(num_constraints):
        Y[i] = float(parts[i])

    Primal = []
    Dual = []


    # ignore Dual ############################ We work only with primal anyway

    for block_num in range(num_blocks):
        if block_sizes[block_num] < 0:
            Primal.append(matrix(RDF, 1, -block_sizes[block_num]))
            #Dual.append(numpy.zeros([1, -block_sizes[block_num]], float))
        else:
            Primal.append(matrix(RDF, block_sizes[block_num], block_sizes[block_num]))
            #Dual.append(numpy.zeros([block_sizes[block_num], block_sizes[block_num]], float))

    while True:
        line = f.readline()
        if len(line) == 0:
            break
        parts = line.split()
        if len(parts) == 0:
            continue
        constraint = int(parts[0])
        block_num = int(parts[1])
        row = int(parts[2])
        column = int(parts[3])
        value = float(parts[4])

        if constraint == 2:
            block = Primal[block_num - 1]
        else:
            continue
            #block = Dual[block_num - 1]

        if block_sizes[block_num - 1] < 0:
            block[0, column - 1] += value
        else:
            block[row - 1, column - 1] += value
            if row != column:
                block[column - 1, row - 1] += value


    solution = dict()
    solution['Y'] = Y
    solution['Primal'] = Primal
    solution['Dual'] = Dual
    return solution


def calculateIndexes(problem):
    indexes = [] # matrix containing indeces for variables
    blocks = len(problem['blocks']) 
    cnt = 0
    for b in range(blocks):
        blocksize = problem['block_sizes'][b]
        if blocksize == -problem['num_constraints']-1 or blocksize == -problem['num_constraints']-2:
            indexes.append(-1)
            continue
        if blocksize > 0:
            MS = MatrixSpace(QQ,blocksize)
            indexes.append(MS())
            for i in range(blocksize):
                for j in range(i,blocksize):
                     indexes[b][j,i] = indexes[b][i,j] = cnt
                     cnt = cnt+1
        if blocksize < 0:
            MS = MatrixSpace(QQ,1,-blocksize)
            indexes.append(MS())
            for i in range(-blocksize):
                indexes[b][0,i] = cnt
                cnt = cnt+1
    return indexes


def all_blocks_positive_definite(M, printstuff = True, QQField = QQ):
    success = True
    for i in range(len(M)):
        B = M[i]
        if B.is_square() and B.ncols() >= 1:
            if B.is_positive_definite():
                if printstuff:
                    if QQField == QQ:
                        print("Block",i,"is positive definite")
                            #print "min eigval is",float(min(B.eigenvalues())) # this takes long time
                    else:
                        print("Block",i,"is positive definite")
            elif B.is_positive_semidefinite():
                if printstuff:
                    if QQField == QQ:
                        print("Block",i,"is positive semidefinite")
                            #print "min eigval is",float(min(B.eigenvalues())) # this takes long time
                    else:
                        print("Block",i,"is positive semidefinite")
            else:
                if printstuff:
                    if QQField == QQ:
                        print("Block",i,"IS NOT POSITIVE DEFINITE, min eigval is",float(min(B.eigenvalues())))
                    else:
                        print("Block",i,"is IS NOT POSITIVE DEFINITE")
                success=False
        else:
            for j in range(B.ncols()):
                if B[0,j] < 0:
                    if printstuff: print("Block",i,"IS NOT POSITIVE DEFINITE")
                    success=False
    return success

def checkSolution(problem, variables, QQField=QQ, indexes=[], testconstraints=[], verbose=false):
   """Evaluates problem (program) with variables. It works as a test if you want to see if your
   solution is any good.
   """
   num_constraints = problem['num_constraints']
   num_blocks = problem['num_blocks']
   block_sizes = problem['block_sizes']
   blocks = problem['blocks']
   constants = problem['constants']
   
   if len(indexes) == 0:
       indexes = calculateIndexes(problem)
   
   if len(testconstraints) == 0:
        testconstraints = list(range(num_constraints))
   
   constantsVector = vector(QQField,constants)
   constantsVector = vector(QQField,constantsVector)

   RoundingLinearSystemVariableNumber = len(variables)   
   densityVector = [0] * len(constants)
   for i in testconstraints:
      #OriginalProbelmMatrix = matrix(QQField, 1, RoundingLinearSystemVariableNumber, sparse=True)
      VariablesDotBlocks =  QQField(0);
      for index in range(num_blocks):          
          size = block_sizes[index]
          if size > 0:
                d = blocks[index][i+1].dict()
                for entry in d:
                    x = entry[0]
                    y = entry[1]
                    if x < y:
                        continue
                    if x == y:
                        #OriginalProbelmMatrix[0, indexes[index][x,y] ] = d[entry] # blocks[index][i+1][x,y]
                        VariablesDotBlocks += variables[ indexes[index][x,y] ] * d[entry]
                    else:
                        #OriginalProbelmMatrix[0, indexes[index][x,y] ] = 2*d[entry] # 2*blocks[index][i+1][x,y] 
                        VariablesDotBlocks += 2 * variables[ indexes[index][x,y] ] * d[entry]
          if size < 0 and -size != num_constraints+1 and -size != num_constraints+2:
              for x in range(-size):
                  #OriginalProbelmMatrix[0, indexes[index][0,x] ] = blocks[index][i+1][0,x]
                  VariablesDotBlocks += variables[ indexes[index][0,x] ] * blocks[index][i+1][0,x]
          
      #densityVector[i] =  constantsVector[i] - (OriginalProbelmMatrix * variables)[0]
      densityVector[i] =  constantsVector[i] - VariablesDotBlocks
   #densityVector = constantsVector - OriginalProbelmMatrix * variables
   return densityVector


# Function to write Ping's solution 
def printLinear(linear,denominator,f=sys.stdout):
    f.write('denominatorLinear='+str(denominator)+'\n')
    f.write('solutionLinear=[')
    for i in range(len(linear)):
        if i != 0:
            f.write(', ')
        f.write(str(linear[i]*denominator))
    f.write(']')
    f.write('\n]')

def printMatrix(roundedMatrices,denominator):
    sys.stdout.write('denominatorLinear='+str(denominator)+'\n')
    sys.stdout.write('solutionLinear=[')
    for b in range(len(roundedMatrices)):
        if b != 0:
            sys.stdout.write(',\n')
        for i in range(len(roundedMatrices[b][0])):
            for j in range(i,len(roundedMatrices[b][0])):
                if i != 0 or j != 0: 
                   sys.stdout.write(', ')
                sys.stdout.write(str(roundedMatrices[b][i,j]*denominator))
    sys.stdout.write(']')
    sys.stdout.write('\n')

def write_rounded_solution_to_file(p, filename=None):
    if filename == None:
        filename = p.filename+".linear_solution.sage"
    f = open(filename, "w")
    f.write('solutionLinear=[')
    for i in range(len(p.solutionRounded)):
        if i != 0:
            f.write(', ')
        f.write(str( p.solutionRounded[i]))
    f.write(']')
    f.write('\n')  
    f.close()  


def load_rounded_solution_from_file(p = None, filename = None):
    if p != None and filename == None:
        filename = p.filename+".linear_solution.sage"
    load(filename)
    if p != None:
        p.solutionRounded = solutionLinear
    return solutionLinear

def check_rounded_solution(filename, optimal_value, constants_rescale=1, solution_filename=None):
    print("Loading problem and solution")
    p = FAProblem(filename, constants_rescale)
    linearSolution = load_rounded_solution_from_file(p, solution_filename)
    print("checking value of the solution")
    output = checkSolution(p.problem, linearSolution)
    if min(output) != optimal_value*constants_rescale:
        print(("Not optimal value! match for", min(output),"and",optimal_value*constants_rescale))
        return False
    print("Optimal value match. Checking if blocks are positive definite")
    # check positive definite
    matrixSolution = linearToMatrixP(p.problem, linearSolution)
    if all_blocks_positive_definite(matrixSolution) == False:
        return False

    print("All looks good")

    return True

def caculateDim(problem):
    dim = []  # may be negative 
    for i in range(len(problem['blocks'])):
        blocksize=problem['block_sizes'][i]
        # Slack variables
        if (blocksize == -problem['num_constraints']-1 or blocksize == -problem['num_constraints']-2):
            dim.append(0)
            continue
        dim.append(blocksize)
    return dim


def linearToMatrix(blocks,dim,linearSolution,QQField=QQ):
   M=[]
   cnt = 0
   for b in range(blocks):
       if dim[b] > 0:
           MS = MatrixSpace(QQField,dim[b])
           M.append(MS())
           for i in range(dim[b]):
               for j in range(i,dim[b]):
                   M[b][j,i] = M[b][i,j] = linearSolution[cnt]
                   cnt=cnt+1
       if dim[b] < 0:
           MS = MatrixSpace(QQField,1,-dim[b])
           M.append(MS())
           for i in range(-dim[b]):
                   M[b][0,i] = linearSolution[cnt]
                   cnt=cnt+1
       if dim[b] == 0:
           MS = MatrixSpace(QQField,0,0)
           M.append(MS())
 
   return M

def linearToMatrixP(problem,linearSolution,QQField=QQ):
    return linearToMatrix(len(problem['blocks']), caculateDim(problem), linearSolution, QQField)


def matrixToLinear(blocks,dim,M,QQField=QQ):
   variables = 0
   for b in range(blocks):
      variables += (dim[b]*(dim[b]+1))/2
   L=vector(QQField,int(variables))
   cnt = 0
   for b in range(blocks):
       for i in range(dim[b]):
           for j in range(i,dim[b]):
               L[cnt] = M[b][j,i]
               cnt=cnt+1
   return L


def guess_zero_eigenvectors(m):
    ''' Call as guess_zero_eigenvectors(p.solnMatrix[0])
    This is a fun utility that may work if the extremal construction
    is nicely balanced AND it is used on flags with k vertices and k-1 are labeled.
    It looks at zero eigenvectors, tries to find coordinates that are non-zero
    and in these coordinated it tries 0-1 vectors and sees if any are potentially
    zero vectors. Finally, it returns a basis of all these vectors.
    This is useful if there are possibly many ways to describe rooting or the
    extremal construction is not known.
    '''
    ev = m.eigenvectors_right()
    x= [ y for y in ev if y[0] < 0.000000001 ]
    b = sum( y[1][0] for y in x )
    z=vector([ round(100*y) for y in b ] )
    zID=vector([ y for y in range(len(z)) if z[y] != 0 ] )

    def makeVs(testV, zID, allV):
        if len(zID) == 0:
            #print(testV)
            allV.append(copy(testV))
            return

        id = zID[0]
        for value in [1,0]:
            testV[id] = value
            makeVs(testV, zID[1:], allV)

    testV = vector([ 0 for y in b ] )
    allV = []
    makeVs(testV, zID, allV)

    tightV = []
    for V in allV:
        if V*m*V < 0.0000001:
            tightV.append(vector(QQ, V))


    VV = QQ^len(z)
    zv = VV.span(tightV)

    b=zv.basis()
    return b

def round_vector(x):
    for z in x:
        if abs(round(z)-z) > 0.001:
            print("Coordinate",z,"may be wrong")
    return vector([ round(z) for z in x])

############################################################################ end of utilities

class FAProblem:
    """Contains problem and solution"""


    def __init__(self,filename,constants_rescale=1,QQField=QQ):
        """Initialization"""

        self.data = []
        self.problem=None
        self.solution=None
        self.solnMatrix=None
        self.solutionRounded=None

        self.QQField = QQField

        self.tightgraphs=[]
        self.tightgraphsIDS=[]
        self.extraTight=[]
        self.projectionMatrices=[]
        
        self.filename=filename
        
        self.constants_rescale=constants_rescale

        if filename != "":
            self.problem=read_problem_file(self.filename,self.constants_rescale)
            self.solution=read_solution_file(self.filename+".result",self.problem)
            self.solnMatrix = self.solution['Primal']

        self.tightgraphsIDS = None

    def reinit(self, problem, solution):
        """Initialization"""
        
        self.data = []
        self.problem=problem
        self.solution=solution
        if solution != None:
            self.solnMatrix=self.solution['Primal']
        else:
            self.solnMatrix=[]
        self.solutionRounded=None
        
        self.tightgraphs=[]
        self.tightgraphsIDS=[]
        self.extraTight=[]
        self.projectionMatrices=[]
        
        self.filename="unknown"

        self.tightgraphsIDS = None


    def test_ranks(self):
        """Testing ranks of matrices in the solution"""
        block_sizes=self.problem['block_sizes']
        nextBlock = 0
        for idA in range(len(self.solnMatrix)):
            # These are slack variables - do not care about these guys
            if -block_sizes[idA] == self.problem['num_constraints']+1 or -block_sizes[idA] == self.problem['num_constraints']+2:
                continue
            A = self.solnMatrix[idA]
            # Do not bother with matrices that are not squares
            # they do not need any special attention
            if not A.is_square():
                continue
            # Now test rank of A
            print("Block",idA,"of size",A.ncols(),"has rank",A.rank())



    def test_zero_eigenvalues(self):
        """Prints smallest eigenvalue for every block of solution."""
        global problem
        global solutionMatrix
        block_sizes=self.problem['block_sizes']
        nextBlock = 0
        for idA in range(len(self.solnMatrix)):
            # These are slack variables - do not care about these guys
            if -block_sizes[idA] == self.problem['num_constraints']+1 or -block_sizes[idA] == self.problem['num_constraints']+2:
                continue
            A = self.solnMatrix[idA]
            # Do not bother with matrices that are not squares
            # they do not need any special attention
            if not A.is_square():
                continue
            # Now we try to find all eigenvalues that are small
            zero_eigenvalues=[]
            smallest_ev = -1
            for ev in A.eigenvectors_right():
                if (smallest_ev == -1 or abs(ev[0]) < smallest_ev):
                    smallest_ev = abs(ev[0])
                if (abs(ev[0]) < 0.0001):
                    zero_eigenvalues.append(abs(ev[0]))
            print("Block",idA,"has smallest eigenvalue",smallest_ev)

    # Prints ten smallest eigenvalues in block idA
    # This is usefull 
    def test_block(self,idA):
        block_sizes=self.problem['block_sizes']
        A = self.solnMatrix[idA]
        if not A.is_square():
            print("Not a square matrix")
            return
        X = sorted([ real_part(ev[0]) for ev in A.eigenvectors_right()])
        print(X[0:min(len(X),10)])
        return X







    # Try to identify tight graphs
    # This is a sanity check - should not be changing when removing types
    # returns lists that contain 3 elemets:
    # 1... value of slack variable.  Slack = 0 means tight graph
    # 2... value in dual. Dual >= means tight graph (appears in extremal construction)
    # 3... ID in order of the graph
    # can be sorted using
    #  sa=sorted(x, key = lambda y: y[0])
    #  sa=sorted(x, key = lambda y: y[1])
    #
    def guess_tight_graphs(self,threshold = 0.000001):
        block_sizes=self.problem['block_sizes']
        self.tightgraphs=[]
        self.tightgraphsIDS=[]
        tightlist=[]
        # test if zero in primal vector
        for idA in range(len(self.solnMatrix)):
            # These are slack variables - do not care about these guys
            if -block_sizes[idA] == self.problem['num_constraints']+1 or -block_sizes[idA] == self.problem['num_constraints']+2:
                A = self.solnMatrix[idA] 
                # Notice that solnMatrix[idA][0,0] contains the extra variable for
                # objective function. So the first slack is at solnMatrix[idA][0,1]
                for i in range(self.problem['num_constraints']):
                    tightlist.append([A[0,i],self.solution['Y'][i],i])
                    #tightlist.append([A[0,i+1],self.solution['Y'][i],i])
                    #tightlist.append([A[0,i+1],solution['Y'][i],density_in_blowup(i),i])
                    if A[0,i] < threshold:
                        print('tight',i,tightlist[i])
                        self.tightgraphs.append(1)
                        self.tightgraphsIDS.append(i)
                    else:
                        self.tightgraphs.append(0)
                        print('loose',i,tightlist[i])
                print("idA=",idA)
                break
        return tightlist
        # test if non-zero in dual vector. It is slightly unprecise
        # as it only works for complementary slackness
        '''
        for i in range(len(solution['Y'])):
            if solution['Y'][i] > 0.0000001:
                tightgraphs.append(1)
                tightgraphsIDS.append(i)
            else:
                tightgraphs.append(0)
        '''



    # Copies problem and does a base change based on zero vectors.
    # Zero vectors are vectors we believe are zero eigenvectors of
    # blocks in the problem matrix. If we find them all, it would
    # be possible that CSDP returns to us a matrix that is positive
    # definite instead of positive semidefinite and it becomes much
    # easier to round.
    #
    def change_base(self,zerovectors,printstuff=True,inPlace=False):
        #global problem
        #global solutionMatrix
        if printstuff:
            print("Making a copy")
        if inPlace:
            newproblem = self.problem
        else:
            newproblem = deepcopy(self.problem);
        block_sizes=self.problem['block_sizes']
        nextBlock = 0
        self.projectionMatrices=[]
        if printstuff:
            print("Processing")
        #for idA in range(len(self.solnMatrix)):
        for idA in range(len(block_sizes)):
            if -block_sizes[idA] == self.problem['num_constraints']+1 or -block_sizes[idA] == self.problem['num_constraints']+2:
                continue
            #A = self.solnMatrix[idA]
            # Do not bother with matrices that are not squares
            # they do not need any special attention
            # Or if they are just constants...
            #if not A.is_square() or (1 == A.nrows() == A.ncols()):
            if block_sizes[idA] < 0 or block_sizes[idA] == 1:
                #newproblem['blocks'] = newproblem['blocks']+[problem['blocks'][idA]]
                continue

            if len(zerovectors) < nextBlock:
                print("Not enought zero vectors")
                return
            if len(zerovectors[nextBlock]) > 0:
                if printstuff: print("Changing base of block",idA,"of size",newproblem['block_sizes'][idA],"using",len(zerovectors[nextBlock]),"eigenvector(s) for zero")
                
                # Here we want matrix with integer entries
                #V = Matrix(self.QQField, zerovectors[nextBlock],sparse=True)
                # Integers make nicer projection matrices without fractions
                V = Matrix(ZZ, zerovectors[nextBlock],sparse=True)
                # Orthogonal complement of our eigenvector
                RestBase = V.right_kernel_matrix().sparse_matrix()

                self.projectionMatrices.append(RestBase)
                
                #print "Base orthogonal to zerovector: T="
                #print RestBase
                #print RestBase*A*RestBase.transpose()
        
                for idX in range(len(newproblem['blocks'][idA])):
                    newproblem['blocks'][idA][idX] = RestBase*newproblem['blocks'][idA][idX]*RestBase.transpose()
                newproblem['block_sizes'][idA] = newproblem['blocks'][idA][idX].nrows()
        
                if printstuff: print("Block",idA,"has size",newproblem['blocks'][idA][idX].nrows())
                #newproblem['block_sizes'][idA] -= len(zerovectors[nextBlock])
       

                #normZ = vector(zerovector).norm()
                #normalZ = [ float(y/normZ) for y in zerovector ]
                #print "Normalized eigenvector for zero:",normalZ
                #T = Matrix([[ float(y) for y in z/z.norm()] for z in RestBase.rows()]+[normalZ])
                #T = Matrix(RestBase.rows()+[zerovector])
                #print "Matrix T*A*T^T="
                #print T*A*T.transpose()
                #X = Matrix(RestBase.rows()+[zerovector]).transpose()
                #print X
                #print X.inverse()*A*X
            else:
                print("Block",idA,"has no zero eigenvalues.")
                self.projectionMatrices.append(None)
            nextBlock+=1
            #print "Done with",idA
        return newproblem

# Copies problem and does a base change based on zero vectors.
# Zero vectors are vectors we believe are zero eigenvectors of
# blocks in the problem matrix. If we find them all, it would
# be possible that CSDP returns to us a matrix that is positive
# definite instead of positive semidefinite and it becomes much
# easier to round.
#
    def change_base_using_projectionMatrices(self,projectionMatrices,printstuff=True,inPlace=False):
        if printstuff:
            print("Making a copy")
        if inPlace:
            newproblem = self.problem
        else:
            newproblem = deepcopy(self.problem);
        block_sizes=self.problem['block_sizes']
        self.projectionMatrices = projectionMatrices
        nextBlock = 0
        if printstuff:
            print("Processing")
        for idA in range(len(self.solnMatrix)):
            if -block_sizes[idA] == self.problem['num_constraints']+1 or -block_sizes[idA] == self.problem['num_constraints']+2:
                continue
            A = self.solnMatrix[idA]
            # Do not bother with matrices that are not squares
            # they do not need any special attention
            # Or if they are just constants...
            if not A.is_square() or (1 == A.nrows() == A.ncols()):
                #newproblem['blocks'] = newproblem['blocks']+[problem['blocks'][idA]]
                continue
        
            if projectionMatrices[idA] != None:
                if printstuff: print("Changing base of block",idA,"of size",newproblem['block_sizes'][idA],"using prelodaded projection matrix")
                #V = Matrix(zerovectors[nextBlock],sparse=True)
                # Orthogonal complement of our eigenvector
                #RestBase = V.right_kernel_matrix().sparse_matrix()
                #self.projectionMatrices.append(RestBase)
                RestBase = projectionMatrices[idA]
                
                for idX in range(len(newproblem['blocks'][idA])):
                    newproblem['blocks'][idA][idX] = RestBase*newproblem['blocks'][idA][idX]*RestBase.transpose()
                newproblem['block_sizes'][idA] = newproblem['blocks'][idA][idX].nrows()
                
                if printstuff: print("Block",idA,"has size",newproblem['blocks'][idA][idX].nrows())
            else:
                print("Block",idA,"has no projection.")
            nextBlock+=1
        return newproblem


    def change_base_using_projectionMatricesAsLists(self,projectionMatricesAsList,printstuff=True,inPlace=False):
        projectionMatrices = []
        for ML in projectionMatricesAsList:
            if len(ML) == 0:
               projectionMatrices.append(None)
            else :
                projectionMatrices.append(matrix(ML, sparse=True))
        return  self.change_base_using_projectionMatrices(projectionMatrices,printstuff,inPlace)




    def perform_rounding(self,optimal_value,zerovectors=[],QQField=QQ,printstuff=True,denominator=1000000000):
        # Zerovectors may contain for each block a list of zero eigenvectors
        # QQField is usefull when used as quarticfield
        
        success=True        
        
        # Try to identify tight graphs
        if self.tightgraphsIDS == None:
            tightgraphs = []
            tightgraphsIDS = []
            for i in range(len(self.solution['Y'])):
                if self.solution['Y'][i] > 0.0000001 or i in self.extraTight:
                    tightgraphs.append(1)
                    tightgraphsIDS.append(i)
                else:
                    #print 'Not tight:',solution_alt['Y'][i]
                    tightgraphs.append(0)
        else:
            tightgraphsIDS = self.tightgraphsIDS
            tightgraphs = [0]*self.solution['Y']
            for t in tightgraphsIDS:
                tightgraphs[t] = 1
        self.tightgraphsIDS = tightgraphsIDS
        
        numtight = len(tightgraphsIDS)
        if printstuff: print("Guessed",numtight,"tight graphs")

 
        blocks = len(self.problem['blocks']) 

        dim = []  # may be negative 
        variables = 0


        for i in range(blocks):
            blocksize=self.problem['block_sizes'][i]
            # Slack variables
            if blocksize == -self.problem['num_constraints']-1 or blocksize == -self.problem['num_constraints']-2:
                dim.append(0)
                continue
            if (blocksize > 0):
                dim.append(self.problem['block_sizes'][i])
                variables += dim[i]*(dim[i]+1)/2
            else:
                dim.append(self.problem['block_sizes'][i])
                variables += -dim[i]

        if printstuff: print("Problem has",variables,"variables")

        zero_constraints=sum([ len(b)*len(b[0]) for b in zerovectors if len(b) > 0  ]  )
        #zero_constraints=sum([ len(b) for b in zerovectors   ]  )
        if printstuff: print("Provided additional",zero_constraints,"zero constraints")
        
        constraints = numtight+zero_constraints
        if printstuff: print("Total number of constraints is",constraints)

       
        '''
        indexes = [] # matrix containing indeces for variables
        cnt = 0
        for b in range(blocks):
            #print dim[b]
            if dim[b] > 0:
                MS = MatrixSpace(QQ,dim[b])
                indexes.append(MS())
                for i in range(dim[b]):
                    for j in range(i,dim[b]):
                        indexes[b][j,i] = indexes[b][i,j] = cnt
                        cnt = cnt+1
            if dim[b] < 0:
                MS = MatrixSpace(QQ,1,-dim[b])
                indexes.append(MS())
                for i in range(-dim[b]):
                    indexes[b][0,i] = cnt
                    cnt = cnt+1
            if dim[b] == 0:
                indexes.append(-1)
        '''
        if printstuff: print("Creating indexes")
        indexes = calculateIndexes(self.problem)

        # Linearize the solution
        if printstuff: print("Linearizing solution")
        solutionX = vector(QQField, int(variables))
        cnt = 0
        for b in range(blocks):
            if dim[b] > 0:
                for i in range(dim[b]):
                    for j in range(i,dim[b]):
                        solutionX[cnt] = QQ(self.solution['Primal'][b][i,j])
                        cnt=cnt+1
            if dim[b] < 0:
                for i in range(-dim[b]):
                        solutionX[cnt] = QQ(self.solution['Primal'][b][0,i])
                        cnt=cnt+1

        self.solutionX = solutionX
        
        # ROUND
        if printstuff: print("Testing the linear solution")        
        self.densityVectorX = checkSolution(self.problem, solutionX, QQField, indexes) 
        if printstuff:
            if QQField == QQ:
                print('Min of density vector for original solution is', float(min(self.densityVectorX)),'and we hope for',float(optimal_value*self.constants_rescale))
            else:
                print('Min of density vector for original solution is', min(self.densityVectorX),'and we hope for',optimal_value*self.constants_rescale)
        


        if printstuff: print("Creating and testing entry-wise rounded solution")
        #   Rounding the solution:
        solutionRounded = vector(QQField,len(solutionX))
        for i in range(len(solutionX)):
            solutionRounded[i] = floor(solutionX[i]*denominator)
            solutionRounded[i] /= denominator
        self.solutionRounded = solutionRounded
        
        
        self.densityVectorR = checkSolution(self.problem, solutionRounded, QQField, indexes) 
        if printstuff: 
            if QQField == QQ:
                print('Min of denisty just rounded vector is', float(min(self.densityVectorR)),'and we hope for',float(optimal_value*self.constants_rescale))
            else:
                print('Min of denisty just rounded vector is', min(self.densityVectorR),'and we hope for',optimal_value*self.constants_rescale)

        
        solutionRounded = solutionX
        
        #print 'Max of denisty vector is', float(max(densityVector))
        #print [ float(z) for z in densityVector ]

        #return  [self.densityVectorX,self.densityVectorR]

        if printstuff: print("Constructing a matrix A of constriants for tight graphs")
        MS = MatrixSpace(QQField,constraints,variables,sparse=False)
        #A = MS(sparse=False)  # zero matrix
        A = MS()  # zero matrix
        #AR = matrix(RDF, constraints, variables)  # zero matrix
        Y = vector(QQField,constraints)

        # constraints for tight graphs:
        # Note that the first g=0 is the objective function
        next_constraint = 0
        for g in range(self.problem['num_constraints']):
            if tightgraphs[g] == 1:
                for b in range(blocks):
                    if dim[b] > 0:
                        #'''
                        d = self.problem['blocks'][b][g+1].dict()
                        for entry in d:
                            row = entry[0]
                            column = entry[1]
                            if column < row:
                                continue
                            value = d[entry]
                            if row == column:
                                A[next_constraint, indexes[b][row, column]] = value
                            else:
                                A[next_constraint, indexes[b][row, column]] = 2*value
                            #AR[next_constraint, indexes[b][row, column]] = value
                        '''
                        for i in range(dim[b]):
                            for j in range(dim[b]):
                                if i == j:
                                    A[next_constraint, indexes[b][i,j]] = self.problem['blocks'][b][g+1][i,j]
                                else:
                                    A[next_constraint, indexes[b][i,j]] = 2*self.problem['blocks'][b][g+1][i,j]
                        '''
                    if dim[b] < 0:
                        for i in range(-dim[b]):
                            A[next_constraint, indexes[b][0,i]] = self.problem['blocks'][b][g+1][0][i]
                            #AR[next_constraint, indexes[b][0,i]] = self.problem['blocks'][b][g+1][0][i]
                Y[next_constraint] = self.problem['constants'][g]-(optimal_value*self.constants_rescale)  # 1 is the optimal solution
                next_constraint += 1

        #Zero constraints
        for b in range(len(zerovectors)):
            for v in zerovectors[b]:
                    if dim[b] > 0:
                        for i in range(dim[b]):
                            for j in range(dim[b]):
                                    A[next_constraint, indexes[b][i,j]] = v[j]
                                    #AR[next_constraint, indexes[b][i,j]] = v[j]
                            Y[next_constraint] = 0  
                            next_constraint += 1
                    if dim[b] < 0:
                        print("ERROR - untested") 
                        for i in range(-dim[b]):
                            A[next_constraint, indexes[b][0,i]] = v[0]
                            #AR[next_constraint, indexes[b][0,i]] = v[0]
                            Y[next_constraint] = 0  
                            next_constraint += 1
        
        if printstuff: print("A constructed of size",A.nrows(),"x",A.ncols())                      

        # Now matrix A should not be modified any more
        # it will lead to A.rank() being fast once computed once
        A.set_immutable()
        #AR.set_immutable()
        if printstuff: print("Rank of A is", A.rank())
        #if printstuff: print "A=",A

        # This is for experiments later
        self.A = A 


        # Finding linearly independent columns
        '''
        if printstuff: print "Calculating pivots of AR"
        pivots=AR.pivots()
        if printstuff: print "Pivots are",pivots
        '''
        if printstuff: print("Calculating pivots of A")
        pivots=find_pivots(A,QQField=QQField)
        self.pivots = pivots
        print("Pivots are", pivots)

        '''
        global main_pivots
        pivots=main_pivots
        '''

        test_pivots(A, pivots, QQField=QQField)

        MS = MatrixSpace(QQField,constraints,len(pivots))
        X = MS()
        use_presolved = []

        for p in range(len(pivots)):
            X[:,p] = A[:,pivots[p]]

        YforX = copy(Y)
        for i in range(variables):
            if not i in pivots:
                for j in range(constraints):
                    YforX[j] -= A[j,i]*solutionRounded[i]

        self.X = X
        self.Y = Y
        self.YforX = YforX
        if printstuff: print("Solving for pivot variables")
        not_presolved=X.solve_right(YforX)


        # Round all that are not presolved
        if printstuff: print("Differences in solved variables compared to rounded values (should be small)")
        for p in range(len(pivots)):
            if printstuff: print("Difference:",float(solutionRounded[pivots[p]]-not_presolved[p]))
            solutionRounded[pivots[p]] = not_presolved[p]


        if printstuff: print("Checking the FINAL solution")
        densityVector = checkSolution(self.problem, solutionRounded,QQField,indexes) 
        if printstuff:
            print('Min of denisty vector is', float(min(densityVector)))
            print('which is exact', min(densityVector))
            print(' and target is',optimal_value*self.constants_rescale)
        #print 'Max of denisty vector is', float(max(densityVector))
        self.solutionRoundedValue=float(min(densityVector))

        # These are the variables that violate the constriants
        violators=[]
        for i in range(len(densityVector)):
            if densityVector[i] < optimal_value*self.constants_rescale:
                violators.append(i)

        if len(violators) == 0:
            if printstuff: 
                print("Rounding successful!!")
                print("Solution is tight on the following")
                print([ i for i in range(len(densityVector)) if densityVector[i] == optimal_value*self.constants_rescale])
        else:
            if printstuff: print("Violated constriants:",violators)
            success=False



        if printstuff: print("Testing if the rounded solution is positive definite")
        M=linearToMatrix(blocks,dim,solutionRounded,QQField)
        self.solutionRounded = solutionRounded
        self.solutionRoundedM = M
        
        if all_blocks_positive_definite(M, printstuff, QQField) == False:
            success = False

        if success:
            if printstuff: 
                print("Congratulations! Rounding succeeded")
        else:
            if printstuff: print("Rounding failed")


        return success

    # zero vectors should correspond to the zero eigenvectors
    # we do a test by multiplying the corresponding vectors and matrices
    # and see what is happening.
    def test_zerovectors(self, zerovectors, verbose=True):
        overall_max=None
        block_sizes=self.problem['block_sizes']
        nextBlock = 0
        for idA in range(len(self.solnMatrix)):
            if -block_sizes[idA] == self.problem['num_constraints']+1 or -block_sizes[idA] == self.problem['num_constraints']+2:
                continue
            A = self.solnMatrix[idA]
            # Do not bother with matrices that are not squares
            # they do not need any special attention
            if not A.is_square():
                continue
        
            if len(zerovectors) <= nextBlock:
                print("Not enought zero vectors, missing for block",nextBlock)
                return
            if len(zerovectors[nextBlock]) > 0:
                for v in range(len(zerovectors[nextBlock])):
                    Av = A*vector(zerovectors[nextBlock][v])
                    if verbose:
                        print('Block',idA,"provided zero vector id",v,"gives when mutliplied with block  max value",max([max(Av),-min(Av)]))
                if overall_max == None or overall_max <  max([max(Av),-min(Av)])   :
                    overall_max = max([max(Av),-min(Av)])
            #else:
            #    print "Block",idA,"has no zero eigenvalues."
            nextBlock+=1
        return overall_max



#unlabeled_flags=[]
# requires one flag per line. Nice in order.
def load_unlabeled_flags(fname, fsize):
    #global unlabeled_flags
    unlabeled_flags=[]
    with open(fname) as f:
        content = f.readlines()
        # remove whitespace characters like `\n` at the end of each line
        content = [x.strip() for x in content]
        for s in content:
            if int(s[0]) == fsize:
                unlabeled_flags.append(s)
    return unlabeled_flags



# Computes density of a graph in blow-up. This is somewhat slow
# as it is using external flag program
def density_in_blowup(fID):
    global unlabeled_flags
    global blow_up
    data=subprocess.Popen("./a.out -FinbupH '{}'  '{}' 2>/dev/null".format(unlabeled_flags[fID], blow_up),shell=True,stdout=subprocess.PIPE).communicate()[0].strip()
    return float(data[9:])

# This is good for getting a list of graphs that should be tight
def tight_in_blowup(unlabeled_flags, blow_up):
    tight_graphs=[]
    for i in range(len(unlabeled_flags)):
        result=subprocess.Popen("./a.out -eFinbupH '{}'  '{}' 2> /dev/null | sed 's/F in blowup of H: //'".format(unlabeled_flags[i], blow_up),shell=True,stdout=subprocess.PIPE).communicate()[0][0]
        if result == '1':
            tight_graphs.append(i)
    return tight_graphs

# Tests if pivots are indeed pivots for matrix A
# Just take the submatrix with the right columns and check the rank
def test_pivots(A, pivots, QQField=QQ):
    if A.rank() != len(pivots):
        print("Expected that",A.rank(),'==',len(pivots))
        

    B = matrix(QQField, A.nrows(), len(pivots))
    for row in range(A.nrows()):
        for column in range(len(pivots)):
            B[row,column] = A[row,pivots[column]]
    rank = B.rank()
    if rank != A.rank():
        print("Wrong rank",rank,'expected',A.rank())
        return False

    #Now test each pivot separately
    
    return True

def create_B(A, pivots, QQField=QQ):
    B = matrix(QQField, A.nrows(), len(pivots)+1)
    for row in range(A.nrows()):
        for column in range(len(pivots)):
            B[row,column] = A[row,pivots[column]]
    return B

def find_pivots_OLD(A,printstuff=False):
    pivots=[0]
    goalRank = A.rank() 
    B = create_B(A, pivots)
    for c in range(1,A.ncols()):
        if printstuff: print("Testing column",c,"out of",A.ncols(),"found",len(pivots),"so far, goal is",goalRank)
        lastB = B.ncols()-1
        for row in range(A.nrows()):
            B[row,lastB] = A[row,c]
        if B.rank() == B.ncols():
            pivots.append(c)
            if len(pivots) == goalRank:
                break
            B  = create_B(A, pivots)
    return pivots

def find_pivots(A,printstuff=False,QQField=QQ):
    pivots=[]
    goalRank = A.rank() 
    B = create_B(A, pivots, QQField)
    for c in range(0,A.ncols()):
        if printstuff: print("Testing column",c,"out of",A.ncols(),"found",len(pivots),"so far, goal is",goalRank)
        lastB = B.ncols()-1
        for row in range(A.nrows()):
            B[row,lastB] = A[row,c]
        if B.rank() == B.ncols():
            pivots.append(c)
            if len(pivots) == goalRank:
                break
            B  = create_B(A, pivots, QQField)
    return pivots

def solve_using_csdp(filename, csdp="csdp", omp=0):
    if omp != 0:
        omp_str = "export OMP_NUM_THREADS="+str(omp)+"; "
    else:
        omp_str = ""
    print(subprocess.Popen(omp_str+csdp+" "+filename+" "+filename+".result | tee "+filename+".log",shell=True,stdout=subprocess.PIPE).communicate()[0])


def rounding_help():
    print("This program helps create rounding")

def resolve(pa):
    write_problem_file('altered.dat-s',pa)
    solve_using_csdp("altered.dat-s")
    return FAProblem('altered.dat-s')
