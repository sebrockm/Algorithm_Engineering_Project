for f in data/anderetests/maps/*.txt
do 
    echo "=============================================="
    echo "$f scip"
    echo -n "$f    " >> scipRes3h.txt
    ./pflp_solver3h -in $f -out test3h.dat -heu 0 >> scipRes3h.txt
    #for i in {0..7}
    #do
        #echo "=============================================="
        #echo "$f heuristic 1 rec $i"
        #echo -n "$f    " >> heu1rec$i.txt
        #./pflp_solver -in $f -out test.dat -heu 1 -rec $i >> heu1rec$i.txt
    #done
    #echo "=============================================="
    #echo "$f heuristic 2"
    #echo -n "$f    " >> heu2.txt
    #./pflp_solver -in $f -out test.dat -heu 2 >> heu2.txt
done
rm test3h.dat
