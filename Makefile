CC = g++

all: ./helper.o ./values.o ./move.o ./board.o ./match.o ./generator.o ./calc.o ./engine.o \

	$(CC) -Wall --std=c++17 -lpthread \
	./helper.o ./values.o ./move.o ./board.o ./match.o ./generator.o ./calc.o ./engine.o -o ./engine

engine.o: ./engine.cpp
	$(CC) -Wall --std=c++17 -c ./engine.cpp -o ./engine.o

./calc.o: ./calc.cpp ./calc.hpp
	$(CC) -Wall --std=c++17 -c ./calc.cpp -o ./calc.o

./generator.o: ./generator.cpp ./generator.hpp
	$(CC) -Wall --std=c++17 -c ./generator.cpp -o ./generator.o

match.o: ./match.cpp ./match.hpp
	$(CC) -Wall --std=c++17 -c ./match.cpp -o ./match.o

board.o: ./board.cpp ./board.hpp
	$(CC) -Wall --std=c++17 -c ./board.cpp -o ./board.o

move.o: ./move.cpp ./move.hpp
	$(CC) -Wall --std=c++17 -c ./move.cpp -o ./move.o

values.o: ./values.cpp ./values.hpp
	$(CC) -Wall --std=c++17 -c ./values.cpp -o ./values.o

helper.o: helper.cpp helper.hpp helper.hpp
	$(CC) -Wall --std=c++17 -c helper.cpp -o ./helper.o

clean:
	rm -f *.o
