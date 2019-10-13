# Copyright 2018, Gurobi Optimization, LLC

TWOUP    = ..
CC       = gcc
CPP      = g++

agmr_rl: agmr_rl.cpp
	$(CPP) -o $@ $<

run_agmr_rl: agmr_rl
	./agmr_rl $(args)

clean:
	rm -rf *.o *_c *_c++ *.class *.log *.rlp *.lp *.bas *.ilp *.mps *.prm kmp