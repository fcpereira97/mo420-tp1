# Copyright 2018, Gurobi Optimization, LLC

TWOUP    = ..
CC       = gcc
CPP      = g++

relaxlag: relaxlag.cpp mystruct.h agmr_rl1.cpp agmr_rl2.cpp heuristic.cpp subgradient.cpp 
	$(CPP) -o $@ agmr_rl1.cpp agmr_rl2.cpp heuristic.cpp subgradient.cpp $<

run_relaxlag: relaxlag
	./relaxlag $(args)

clean:
	rm -rf *.o *_c *_c++ *.class *.log *.rlp *.lp *.bas *.ilp *.mps *.prm kmp