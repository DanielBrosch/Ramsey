python3 ../general_generate.py  '1 1 0 0  0 0 1  1 0  0' > F_ordered_edges3__forbidden.txt
printf "3 0 1 2 3\n3 0 1 3 2\n3 0 2 1 3\n3 0 2 3 1\n3 0 3 1 2\n3 0 3 2 1\n" >> F_ordered_edges3__forbidden.txt
printf "10 2 0 1\n" > F_ordered_edges3__objective.txt
../a.out -n 5 -lb &> log.txt
