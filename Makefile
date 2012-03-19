LIBS  = -lboost_system -lpng
CFLAGS = -Wall -O2 -L /usr/local/lib

# Should be equivalent to your list of C files, if you don't build selectively
SRC=$(wildcard *.cpp)

all:	obj nbody

obj:	$(SRC)
	g++ -c $(SRC) $^ 

nbody:	$(SRC)
	g++ -o nbody n-body-physics.o simulation.o $(CFLAGS) $(LIBS)

clean:
	rm *.o nbody