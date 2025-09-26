for d in */ ; do
    cd $d
    ./run.sh
    echo "Ran $d"
    cd ..
done
