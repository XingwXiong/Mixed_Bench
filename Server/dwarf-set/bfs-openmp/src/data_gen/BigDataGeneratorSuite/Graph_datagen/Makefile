#
# makefile for gen_kronecker_graph
#
#

all: gen_kronecker_graph.cpp
	g++ -std=c++98 -Wall -DNDEBUG -o gen_kronecker_graph gen_kronecker_graph.cpp kronecker.cpp Snap.o -I./snap-core -I./snap-adv -I./glib-core -I./snap-exp  -lrt -O3 #-g

.PHONY : clean
clean :
	-rm gen_kronecker_graph
