CC = g++

all: pieceshelper.o searchforpiece.o touch.o \
	 piece.o rook.o bishop.o knight.o queen.o king.o whitepawn.o blackpawn.o

piece.o: piece.cpp piece.hpp ../helper.hpp ../values.hpp
	$(CC) -Wall --std=c++17 -c piece.cpp -o piece.o

rook.o: rook.cpp rook.hpp piece.cpp piece.hpp ../helper.hpp ../values.hpp
	$(CC) -Wall --std=c++17 -c rook.cpp -o rook.o

bishop.o: bishop.cpp bishop.hpp piece.cpp piece.hpp  ../helper.hpp ../values.hpp
	$(CC) -Wall --std=c++17 -c bishop.cpp -o bishop.o

knight.o: knight.cpp knight.hpp piece.cpp piece.hpp  ../helper.hpp ../values.hpp
	$(CC) -Wall --std=c++17 -c knight.cpp -o knight.o

queen.o: queen.cpp queen.hpp piece.cpp piece.hpp  ../helper.hpp ../values.hpp
	$(CC) -Wall --std=c++17 -c queen.cpp -o queen.o

king.o: king.cpp king.hpp piece.cpp piece.hpp  ../helper.hpp ../values.hpp
	$(CC) -Wall --std=c++17 -c king.cpp -o king.o

whitepawn.o: whitepawn.cpp whitepawn.hpp piece.cpp piece.hpp  ../helper.hpp ../values.hpp
	$(CC) -Wall --std=c++17 -c whitepawn.cpp -o whitepawn.o

blackpawn.o: blackpawn.cpp blackpawn.hpp piece.cpp piece.hpp  ../helper.hpp ../values.hpp
	$(CC) -Wall --std=c++17 -c blackpawn.cpp -o blackpawn.o

touch.o: touch.cpp touch.hpp ../helper.hpp ../values.hpp
	$(CC) -Wall --std=c++17 -c touch.cpp -o touch.o

searchforpiece.o: searchforpiece.cpp searchforpiece.hpp ../helper.hpp ../values.hpp
	$(CC) -Wall --std=c++17 -c pieceshelper.cpp -o pieceshelper.o

pieceshelper.o: pieceshelper.cpp pieceshelper.hpp ../helper.hpp ../values.hpp
	$(CC) -Wall --std=c++17 -c searchforpiece.cpp -o searchforpiece.o

clean:
	rm -f *.o