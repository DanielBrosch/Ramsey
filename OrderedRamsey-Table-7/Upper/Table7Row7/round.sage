import sys
load("../rounding_Integer.sage")
round_program("SDP_n"+sys.argv[1]+"_LB_F_ordered_edges3__objective.txt.dat-s",scale=10)
'''
Result:
Minimum (after unscaling) found at 3156 / 6001 Value is 0.049824245075478425
498242450754784284408569662226534768356878 / 10000000000000000000000000000000000000000000  # =  0.049824245075478425
'''
