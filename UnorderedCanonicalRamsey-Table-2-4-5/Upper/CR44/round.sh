#!/bin/bash

FILE='SDP_n6_LB_F_edges8blind__objectivee.txt.dat-s'
SCALE='15'
SCALED='False'


sage -c "load('../rounding_Integer.sage'); only_rounding('${FILE}', ${SCALE}, ${SCALED})" | tee round.log


