LIBS  = -lboost_system
CFLAGS = -g -Wall -O3 -L /home/garethf/scratch/usr/lib

# Should be equivalent to your list of C files, if you don't build selectively
SRC=$(wildcard *.cpp)

all:	obj par seq

obj:	$(SRC)
	mpic++ -c $(SRC) $(CFLAGS) $^ 

par:	$(SRC)
	mpic++ -o par n-body-physics.o parallel-simulation.o $(CFLAGS) $(LIBS) -lboost_mpi

seq:	$(SRC)
	g++44 -o seq n-body-physics.o sequential-simulation.o $(CFLAGS) $(LIBS)

clean:
	rm *.o par seq