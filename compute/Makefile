CC = g++

all: analyze_position.o calc.o 

calc.o: calc.cpp calc.hpp ../helper.hpp ../values.hpp
	$(CC) -Wall --std=c++17 -c calc.cpp -o calc.o

analyze_position.o: analyze_position.hpp analyze_position.cpp
	$(CC) -Wall --std=c++17 -c analyze_position.cpp -o analyze_position.o

clean:
	rm -f *.o