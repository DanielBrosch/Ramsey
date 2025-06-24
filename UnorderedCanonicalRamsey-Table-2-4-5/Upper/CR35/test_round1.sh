#!/bin/bash


FILE='SDP_n7_LB_F_edges16partition__objective.txt.dat-s'
SCALE='21'
SCALED='False'


MIN=0
#MAX=`head -1 "${FILE}"`
MAX=120000   # Should just fit
STEP=20000  # around 100G

for (( i = MIN; i <= MAX; i += STEP )); do
    #echo "Current value: $i"

    j=$((i + STEP))


   LOGFILE="round.${j}.txt"
   if [ -f "$LOGFILE" ]; then
       echo "Skipping ${LOGFILE}"
   else 
       echo "Computing ${LOGFILE}"
       sage -c "load('../rounding_Integer.sage'); test_rounding('${FILE}',${SCALE},${SCALED}, $i, $j)" > "${LOGFILE}" &
   fi

    # Add your logic here using $i
done


echo 'Everything is started for round 1'

wait

