CC = g++

all: searchforpiece.o piece.o rook.o bishop.o knight.o queen.o pawn.o whitepawn.o blackpawn.o

piece.o: piece.cpp piece.hpp
	$(CC) -Wall -ggdb -c piece.cpp -o piece.o

rook.o: rook.cpp rook.hpp
	$(CC) -Wall -ggdb -c rook.cpp -o rook.o

bishop.o: bishop.cpp bishop.hpp
	$(CC) -Wall -ggdb -c bishop.cpp -o bishop.o

knight.o: knight.cpp knight.hpp
	$(CC) -Wall -ggdb -c knight.cpp -o knight.o

queen.o: queen.cpp queen.hpp
	$(CC) -Wall -ggdb -c queen.cpp -o queen.o

pawn.o: pawn.cpp pawn.hpp
	$(CC) -Wall -ggdb -c pawn.cpp -o pawn.o

whitepawn.o: whitepawn.cpp whitepawn.hpp
	$(CC) -Wall -ggdb -c whitepawn.cpp -o whitepawn.o

blackpawn.o: blackpawn.cpp blackpawn.hpp
	$(CC) -Wall -ggdb -c blackpawn.cpp -o blackpawn.o

searchforpiece.o: searchforpiece.cpp searchforpiece.hpp
	$(CC) -Wall -ggdb -c searchforpiece.cpp -o searchforpiece.o

clean:
	rm -f *.o