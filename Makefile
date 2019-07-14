CC = g++

all: values.o helper.o move.o board.o match.o values.o ./pieces/pieceshelper.o ./pieces/searchforpiece.o ./pieces/piece.o ./pieces/rook.o ./pieces/bishop.o ./pieces/knight.o ./pieces/queen.o ./pieces/king.o ./pieces/pawn.o ./pieces/whitepawn.o ./pieces/blackpawn.o ./compute/calc.o engine.o 
	$(CC) -Wall --std=c++17 values.o helper.o match.o board.o move.o ./pieces/pieceshelper.o ./pieces/searchforpiece.o ./pieces/piece.o ./pieces/rook.o ./pieces/bishop.o ./pieces/knight.o ./pieces/queen.o ./pieces/king.o ./pieces/pawn.o ./pieces/whitepawn.o ./pieces/blackpawn.o ./compute/calc.o engine.o -o engine

engine.o: engine.cpp
	$(CC) -Wall --std=c++17 -c engine.cpp

values.o: values.cpp values.hpp
	$(CC) -Wall --std=c++17 -c values.cpp

helper.o: helper.cpp helper.hpp helper.hpp values.hpp
	$(CC) -Wall --std=c++17 -c helper.cpp

move.o: move.cpp move.hpp helper.hpp values.hpp
	$(CC) -Wall --std=c++17 -c move.cpp

board.o: board.cpp board.hpp helper.hpp values.hpp
	$(CC) -Wall --std=c++17 -c board.cpp

match.o: match.cpp match.hpp board.hpp helper.hpp values.hpp
	$(CC) -Wall --std=c++17 --std=c++17  -c match.cpp

make: 
	make ./pieces/Makefile
	make ./compute/Makefile

clean:
	rm -f *.o rm -f ./pieces/*.o
	rm -f *.o rm -f ./compute/*.o

