CC = g++

all: searchforpiece.o piece.o rook.o

piece.o: piece.cpp piece.hpp
	$(CC) -Wall -ggdb -c piece.cpp -o piece.o

piece.o: rook.cpp rook.hpp
	$(CC) -Wall -ggdb -c rook.cpp -o rook.o

searchforpiece.o: searchforpiece.cpp searchforpiece.hpp
	$(CC) -Wall -ggdb -c searchforpiece.cpp -o searchforpiece.o

clean:
	rm -f *.o