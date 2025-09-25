#!/bin/bash

mkdir -p ./tmp

# for dir in "./40211222" "./40212222" "./40221222" "./40222122"
# for dir in "./40211212" "./40211222" "./40221122" "./40212222" "./40221222" "./40222122"
find . -maxdepth 1 -type d | grep 4 | while read dir; do
do
    cd ./tmp

    for i in 4..7
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

    cd ..
done

