echo "begin"
python3 ../general_generate.py  '1 1 1 0  0 0 0  0 0  1' > F_ordered_edges3__forbidden.txt
printf "3 0 1 2 3\n3 0 1 3 2\n3 0 2 1 3\n3 0 2 3 1\n3 0 3 1 2\n3 0 3 2 1\n" >> F_ordered_edges3__forbidden.txt
printf "3 0 1 1 2\n3 0 1 2 1\n3 0 2 1 1\n3 0 1 1 3\n3 0 1 3 1\n3 0 3 1 1" >> F_ordered_edges3__forbidden.txt
if [ $1 -eq 5 ]
then
printf "10 2 0 1\n" > F_ordered_edges3__objective.txt
else
printf "30 2 0 1\n" > F_ordered_edges3__objective.txt
fi
echo "constraints set"
rm  n$1/*
echo "Start solving"
csdp=/lustre/hdd/LAS/lidicky-lab/miyasaki/csdp6.2.0linuxx86_64/bin/csdp
../a.out -n $1 -lb -fgf -csdp $csdp &> n$1/solver_log_$1.txt
echo "Solving finished"
echo "Start Rounding"
sage round.sage $1 $> n$1/rounding_log$1.txt
echo "Rounding finished"
mv SDP_n$1* n$1
zip -r n$1.zip n$1
echo "Done"
