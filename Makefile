CC = g++

all: ./values.o ./piece.o ./move.o ./board.o ./match.o ./engine.o \

	$(CC) -Wall --std=c++17 -lpthread -O3 \
	./values.o ./piece.o ./move.o ./board.o ./match.o ./engine.o -o ./engine

chess.o: ./engine.cpp
	$(CC) -Wall --std=c++17 -O3 -c ./engine.cpp -o ./engine.o

match.o: ./match.cpp ./match.hpp
	$(CC) -Wall --std=c++17 -O3 -c ./match.cpp -o ./match.o

board.o: ./board.cpp ./board.hpp
	$(CC) -Wall --std=c++17 -O3 -c ./board.cpp -o ./board.o

move.o: ./move.cpp ./move.hpp
	$(CC) -Wall --std=c++17 -O3 -c ./move.cpp -o ./move.o

piece.o: piece.cpp piece.hpp
	$(CC) -Wall --std=c++17 -O3 -c piece.cpp -o ./piece.o

values.o: ./values.cpp ./values.hpp
	$(CC) -Wall --std=c++17 -O3 -c ./values.cpp -o ./values.o

clean:
	rm -f *.o
