import sys
load("../rounding_Integer.sage")
round_program("SDP_n"+sys.argv[1]+"_LB_F_ordered_edges3__objective.txt.dat-s",scale=10)
'''
Result:
Minimum (after unscaling) found at 5694 / 6001 Value is 0.03906345111431516
390634511143151657709439378392479666426456 / 10000000000000000000000000000000000000000000  # =  0.03906345111431517
'''
