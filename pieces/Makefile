CC = g++

all: searchforpiece.o piece.o rook.o bishop.o knight.o queen.o pawn.o whitepawn.o blackpawn.o

piece.o: piece.cpp piece.hpp helper.hpp values.hpp
	$(CC) -Wall --std=c++17 -c piece.cpp -o piece.o

rook.o: rook.cpp rook.hpp helper.hpp values.hpp
	$(CC) -Wall --std=c++17 -c rook.cpp -o rook.o

bishop.o: bishop.cpp bishop.hpp helper.hpp values.hpp
	$(CC) -Wall --std=c++17 -c bishop.cpp -o bishop.o

knight.o: knight.cpp knight.hpp helper.hpp values.hpp
	$(CC) -Wall --std=c++17 -c knight.cpp -o knight.o

queen.o: queen.cpp queen.hpp helper.hpp values.hpp
	$(CC) -Wall --std=c++17 -c queen.cpp -o queen.o

pawn.o: pawn.cpp pawn.hpp helper.hpp values.hpp
	$(CC) -Wall --std=c++17 -c pawn.cpp -o pawn.o

whitepawn.o: whitepawn.cpp whitepawn.hpp helper.hpp values.hpp
	$(CC) -Wall --std=c++17 -c whitepawn.cpp -o whitepawn.o

blackpawn.o: blackpawn.cpp blackpawn.hpp helper.hpp values.hpp
	$(CC) -Wall --std=c++17 -c blackpawn.cpp -o blackpawn.o

searchhelper.o: searchforpiece.cpp searchforpiece.hpp helper.hpp values.hpp
	$(CC) -Wall --std=c++17 -c searchforpiece.cpp -o searchforpiece.o

clean:
	rm -f *.o