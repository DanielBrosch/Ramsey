#!/bin/bash

mkdir -p ./tmp

# find . -maxdepth 1 -type d | grep 4 | while read dir; do
for dir in "./40211222" "./40212222" "./40221222" "./40222122"
# for dir in "./40211212" "./40211222" "./40221122" "./40212222" "./40221222" "./40222122"
do
cd ./tmp
# dir="./40212212" 
# cd $dir

for i in 7
do
    echo "$i"
    echo $dir
    if test -f ../$dir/F_ordered_edges3partition__n${i}_unlabeled.txt; then
        echo "Running $dir for level $i"
        rm -f ./F_ordered_edges3partition__n${i}_labeled.txt
        rm -f ./F_ordered_edges3partition__n${i}_unlabeled.txt
        cp ../$dir/F_ordered_edges3partition__n${i}_unlabeled.txt ./
        cp ../$dir/F_ordered_edges3partition__objectivee.txt ./
        echo "no graphs:"
        wc -l < F_ordered_edges3partition__n${i}_unlabeled.txt
        ../a.out -n $i -lb -v -csdp ./../../ER/csdp | tee "./../sol.$i.${dir:2}.log"
        # ../a.out -n $i -lb -v -solver mosek | tee "./../sol.$i.${dir:2}.mosek.log"
        # cat file.log | grep 'Dual' | sed 's/Dual.*://'
        VALUE=`cat "./../sol.$i.${dir:2}.log" | grep 'Dual' | sed 's/Dual.*://'`
        echo $dir
        echo $VALUE
        echo "print(1+1/${VALUE})" | python3 >> "./../sol.$i.${dir:2}.log"
    else 
        echo "No graphs for $dir at level $i"
    fi
done

# rm -f ../tmp/F_ordered_edges24partition__n4_labeled.txt
# rm -f ../tmp/F_ordered_edges24partition__n5_labeled.txt
# rm -f ../tmp/F_ordered_edges24partition__n6_labeled.txt
# rm -f ../tmp/F_ordered_edges24partition__n7_labeled.txt
# rm -f ../tmp/F_ordered_edges24partition__n8_labeled.txt
# rm -f ../tmp/F_ordered_edges24partition__n9_labeled.txt
# cp F_ordered_edges3partition__n9_unlabeled.txt F_ordered_edges3partition__n8_unlabeled.txt F_ordered_edges3partition__n7_unlabeled.txt F_ordered_edges3partition__n6_unlabeled.txt F_ordered_edges3partition__n5_unlabeled.txt F_ordered_edges3partition__n4_unlabeled.txt F_ordered_edges3partition__objectivee.txt ../tmp/
# cd ../tmp
# echo "0 0" >> F_ordered_edges24partition__n4_unlabeled.txt
# echo "0 0" >> F_ordered_edges24partition__n5_unlabeled.txt
# echo "0 0" >> F_ordered_edges24partition__n6_unlabeled.txt
# ../a.out -n 4 -lb -v -solver csdp | tee "./../sol.4.${dir:2}.log"
# ../a.out -n 5 -lb -v -solver csdp | tee "./../sol.5.${dir:2}.log"
# ../a.out -n 4 -lb -v -solver mosek | tee "./../sol.4.${dir:2}.log"
# ../a.out -n 5 -lb -v -solver mosek | tee "./../sol.5.${dir:2}.log"
# ../a.out -n 6 -lb -v -solver mosek | tee "./../sol.6.${dir:2}.log"
# ../a.out -n 6 -lb -v -csdp ./csdp | tee "./../sol.6.${dir:2}.log"
# ../a.out -n 4 -lb -v -csdp ./../csdp | tee "./../sol.4.${dir:2}.log"




cd ..
done

