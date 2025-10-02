import sys
load("../rounding_Integer.sage")
round_program("SDP_n"+sys.argv[1]+"_LB_F_ordered_edges3__objective.txt.dat-s",scale=10)
'''
Result:
Minimum (after unscaling) found at 3773 / 6065 Value is 0.020166009528582995
201660095285829954691538561448176407569728 / 10000000000000000000000000000000000000000000  # =  0.020166009528582995
'''
