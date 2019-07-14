CC = g++

all: calc.o 

calc.o: calc.cpp calc.hpp ../helper.hpp ../values.hpp
	$(CC) -Wall --std=c++17 -c calc.cpp -o calc.o

clean:
	rm -f *.o