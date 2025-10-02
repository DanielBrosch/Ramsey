for d in */ ; do
    cd $d
    ./run.sh $1
    echo "Ran $d. Now rounding"
    sage round.sage &> rounding_log_$1.txt
    cd ..
done
