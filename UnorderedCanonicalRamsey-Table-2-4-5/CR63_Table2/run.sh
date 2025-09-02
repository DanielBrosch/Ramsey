#!/bin/sh

# The following may take some time to generate
# make
# ./a.out -n 25 -qagu -ub -v

# This is processing the solution
cut -d ' ' -f 1   F_edges20partition__n25_unlabeled.txt  | uniq | while read id ; do
COUNT=`grep "^$id " F_edges20partition__n25_unlabeled.txt  | wc -l`
echo "n= $id   count = $COUNT"
done

python3 testmax.py 


