CC = g++

all: ./analyze_helper.o ./analyze_tactics.o ./analyze_move.o ./analyze_position.o openings.o ./calc.o

calc.o: ./calc.cpp ./calc.hpp
	$(CC) -Wall --std=c++17 -c ./calc.cpp -o ./calc.o

openings.o: ./openings.hpp ./openings.cpp
	$(CC) -Wall --std=c++17 -c ./openings.cpp -o ./openings.o

analyze_position.o: ./analyze_position.hpp ./analyze_position.cpp
	$(CC) -Wall --std=c++17 -c ./analyze_position.cpp -o ./analyze_position.o

analyze_move.o: ./analyze_move.hpp ./analyze_move.cpp
	$(CC) -Wall --std=c++17 -c ./analyze_move.cpp -o ./analyze_move.o

analyze_tactics.o: ./analyze_tactics.hpp ./analyze_tactics.cpp
	$(CC) -Wall --std=c++17 -c ./analyze_tactics.cpp -o ./analyze_tactics.o

analyze_helper.o: ./analyze_helper.hpp ./analyze_helper.cpp
	$(CC) -Wall --std=c++17 -c ./analyze_helper.cpp -o ./analyze_helper.o

clean:
	rm -f *.o
