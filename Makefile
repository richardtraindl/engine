CC = g++

all: ./values.o ./helper.o ./move.o ./board.o ./match.o ./engine.o \
	 ./compute/analyze_helper.o ./compute/analyze_tactics.o \
	 ./compute/analyze_move.o ./compute/analyze_position.o ./compute/calc.o \
	 ./pieces/touch.o ./pieces/searchforpiece.o ./pieces/piece_ext2.o \
	 ./pieces/piece_ext1.o ./pieces/piece.o \
	 ./ui/play.o \
	 ./bricks/brick.o

	$(CC) -Wall --std=c++17 -lpthread \
		./ui/play.o \
		./pieces/touch.o ./pieces/searchforpiece.o \
		./pieces/piece_ext1.o ./pieces/piece_ext2.o ./pieces/piece.o \
		./compute/analyze_helper.o ./compute/analyze_tactics.o \
		./compute/analyze_move.o ./compute/analyze_position.o ./compute/calc.o \
		./helper.o ./values.o ./move.o ./board.o ./match.o ./engine.o -o ./engine

engine.o: ./engine.cpp
	$(CC) -Wall --std=c++17 -c ./engine.cpp -o ./engine.o

match.o: ./match.cpp match.hpp
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
	rm -f *.o rm -f ./pieces/*.o
	rm -f *.o rm -f ./compute/*.o
	rm -f *.o rm -f ./ui/*.o
