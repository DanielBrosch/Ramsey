#!/bin/bash
echo "begin"

# Create a clean directory to put our results
# WARNING: Will erase any data from previous runs. Relocate anything you want
# to save
if [ -d "n$1" ] ; then
    rm -r n$1
fi
mkdir n$1

if [ -d "zippedn$1" ] ; then
    rm -r zippedn$1
fi
mkdir zippedn$1

# Record the graph used
echo '1 1 1 0  0 0 0  0 1  0' > zippedn$1/forbidden_graph.txt

# Generate colored cliques containing a monochromatic copy of forbidden graph
python3 ../general_generate.py  '1 1 1 0  0 0 0  0 1  0' > F_ordered_edges3__forbidden.txt

# Forbidden graphs creating blow-up structure
printf "3 0 1 2 3\n3 0 1 3 2\n3 0 2 1 3\n3 0 2 3 1\n3 0 3 1 2\n3 0 3 2 1\n" >> F_ordered_edges3__forbidden.txt
printf "3 0 1 1 2\n3 0 1 2 1\n3 0 2 1 1\n3 0 1 1 3\n3 0 1 3 1\n3 0 3 1 1" >> F_ordered_edges3__forbidden.txt

# Write objective. Scale so that SDP has integer coefficients
if [ $1 -eq 5 ]
then
printf "10 2 0 1\n" > F_ordered_edges3__objective.txt
else
printf "30 2 0 1\n" > F_ordered_edges3__objective.txt
fi

echo "Constraints set"

# Solve the SDP
echo "Start solving"
../a.out -n $1 -lb -fgf &> n$1/solver_log_$1.txt
echo "Solving finished"

# Round the solution
echo "Start Rounding"
sage round.sage $1 $> n$1/rounding_log$1.txt
echo "Rounding finished"

mv SDP_n$1* n$1

# Optional zipping
if [ ${2:-no} == "-z" ]
then
for f in "n$1"/* ; do
    name=$(basename $f)
    zip zippedn$1/$name.zip n$1/$name
done
fi

echo "Done"
