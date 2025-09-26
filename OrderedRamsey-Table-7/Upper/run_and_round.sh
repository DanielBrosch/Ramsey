for d in */ ; do
    cd $d
    ./run.sh
    echo "Ran $d. Now rounding"
    sage round.sage &> temp.txt
    cd ..
done
