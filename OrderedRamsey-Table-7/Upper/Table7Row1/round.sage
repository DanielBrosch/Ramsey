import sys
load("../rounding_Integer.sage")
round_program("SDP_n" + str(sys.argv[1]) + "_LB_F_ordered_edges3__objective.txt.dat-s",scale=sys.argv[2])
'''
Result:
Minimum (after unscaling) found at 5936 / 6001 Value is 0.03746387918200873374638791820087306596553119645636250748306 / 10000000000000000000000000000000000000000000  # =  0.03746387918200873
'''
