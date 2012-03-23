LIBS  = -lboost_system
CFLAGS = -g -Wall -O3 -L /home/garethf/scratch/usr/lib

# Should be equivalent to your list of C files, if you don't build selectively
SRC=$(wildcard *.cpp)

all:	obj par seq

obj:	n-body-physics.cpp n-body-physics.h parallel-simulation.cpp sequential-simulation.cpp
	mpic++ -c *.cpp $(CFLAGS) 

par:	n-body-physics.cpp n-body-physics.h parallel-simulation.cpp
	mpic++ -o par n-body-physics.o parallel-simulation.o $(CFLAGS) $(LIBS) -lboost_mpi

seq:	n-body-physics.cpp n-body-physics.h sequential-simulation.cpp
	g++44 -o seq n-body-physics.o sequential-simulation.o $(CFLAGS) $(LIBS)

clean:
	rm *.o par seq