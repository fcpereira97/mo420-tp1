# Copyright 2018, Gurobi Optimization, LLC

TWOUP    = ..
CC       = gcc
CPP      = g++

relaxlag: relaxlag.cpp mystruct.h preprocessing.cpp kruskal.cpp inspection.cpp heuristic.cpp subgradient.cpp 
	$(CPP) -o $@ preprocessing.cpp kruskal.cpp inspection.cpp heuristic.cpp subgradient.cpp $<

run_relaxlag: relaxlag
	./relaxlag $(args)

clean:
	rm -rf *.o *_c *_c++ *.class *.log *.rlp *.lp *.bas *.ilp *.mps *.prm kmp