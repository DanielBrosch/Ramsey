for d in */ ; do
    cd $d
    ./run.sh $1
    echo "Ran $d"
    cd ..
done
