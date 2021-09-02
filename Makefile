CC = g++

all: ./values.o ./piece.o ./move.o ./board.o ./bitboard.o ./endgame.o ./match.o ./threading.o ./engine.o \

	$(CC) -Wall --std=c++17 -lpthread -O3 \
	./values.o ./piece.o ./move.o ./board.o ./bitboard.o ./endgame.o ./match.o ./threading.o ./engine.o -o ./engine

engine.o: ./engine.cpp
	$(CC) -Wall --std=c++17 -O3 -c ./engine.cpp -o ./engine.o

threading.o: ./threading.cpp ./threading.hpp
	$(CC) -Wall --std=c++17 -O3 -c ./threading.cpp -o ./threading.o

match.o: ./match.cpp ./match.hpp
	$(CC) -Wall --std=c++17 -O3 -c ./match.cpp -o ./match.o

board.o: ./board.cpp ./board.hpp
	$(CC) -Wall --std=c++17 -O3 -c ./board.cpp -o ./board.o

bitboard.o: ./bitboard.cpp ./bitboard.hpp
	$(CC) -Wall --std=c++17 -O3 -c ./bitboard.cpp -o ./bitboard.o

endgame.o: ./endgame.cpp ./endgame.hpp
	$(CC) -Wall --std=c++17 -O3 -c ./endgame.cpp -o ./endgame.o

move.o: ./move.cpp ./move.hpp
	$(CC) -Wall --std=c++17 -O3 -c ./move.cpp -o ./move.o

piece.o: piece.cpp piece.hpp
	$(CC) -Wall --std=c++17 -O3 -c piece.cpp -o ./piece.o

values.o: ./values.cpp ./values.hpp
	$(CC) -Wall --std=c++17 -O3 -c ./values.cpp -o ./values.o

clean:
	rm -f *.o
