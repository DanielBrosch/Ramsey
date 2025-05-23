#!/bin/bash

FILE='SDP_n7_LB_F_edges16partition__objective.txt.dat-s'
SCALE='21'
SCALED='False'


sage -c "load('../rounding_Integer.sage'); only_rounding('${FILE}', ${SCALE}, ${SCALED})" | tee round.log


