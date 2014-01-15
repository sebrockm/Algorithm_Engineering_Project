for f in data/*/*/*.{txt,dat}
do 
    echo "$f"
    echo -n "$f    " >> dichten.txt
    ./pflp_solver -ndichte $f >> dichten.txt
done
