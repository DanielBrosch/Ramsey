import sys
load("../rounding_Integer.sage")
S = 10
if int(sys.argv[1]) == 6:
	S = 30
round_program("SDP_n" + str(sys.argv[1]) + "_LB_F_ordered_edges3__objective.txt.dat-s",scale=S)
