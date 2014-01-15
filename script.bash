for f in data/*/*/*.{txt,dat}
do 
    echo "=============================================="
    echo "$f scip"
    echo -n "$f    " >> scipRes.txt
    ./pflp_solver -in $f -out test.dat -heu 0 >> scipRes.txt
    for i in {0..7}
    do
        echo "=============================================="
        echo "$f heuristic 1 rec $i"
        echo -n "$f    " >> heu1rec$i.txt
        ./pflp_solver -in $f -out test.dat -heu 1 -rec $i >> heu1rec$i.txt
    done
    echo "=============================================="
    echo "$f heuristic 2"
    echo -n "$f    " >> heu2.txt
    ./pflp_solver -in $f -out test.dat -heu 2 >> heu2.txt
done
rm test.dat
