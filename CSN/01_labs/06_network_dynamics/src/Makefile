GCC=g++ --std=c++11

all: graph.o barabasi_albert.o simulation

graph.o: graph.cc graph.h
	$(GCC) -c graph.cc

barabasi_albert.o: barabasi_albert.h barabasi_albert.cc
	$(GCC) -c barabasi_albert.cc

simulation: barabasi_albert.o graph.o simulation.cc
	$(GCC) -c simulation.cc
	$(GCC) -o simulation  simulation.o barabasi_albert.o graph.o

clean:
	rm *.o simulation