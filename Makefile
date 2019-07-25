CC = g++

all: values.o helper.o move.o board.o match.o \
	 ./pieces/searchforpiece.o ./pieces/touch.o ./pieces/piece.o ./pieces/piece_ext1.o ./pieces/piece_ext2.o \
	 ./compute/analyze_position.o ./compute/calc.o engine.o 
	$(CC) -Wall --std=c++17 values.o helper.o move.o board.o match.o \
		./pieces/searchforpiece.o ./pieces/touch.o ./pieces/piece.o ./pieces/piece_ext1.o ./pieces/piece_ext2.o \
		./compute/analyze_position.o ./compute/calc.o engine.o -o engine

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
	$(CC) -Wall --std=c++17 -c match.cpp

searchforpiece.o: searchforpiece.cpp searchforpiece.hpp helper.hpp values.hpp
	$(CC) -Wall --std=c++17 -c searchforpiece.cpp -o searchforpiece.o

touch.o: touch.cpp touch.hpp helper.hpp values.hpp
	$(CC) -Wall --std=c++17 -c touch.cpp -o touch.o

piece.o: piece.cpp piece.hpp helper.hpp values.hpp
	$(CC) -Wall --std=c++17 -c piece.cpp -o piece.o

piece_ext.o: piece_ext.cpp piece_ext.hpp helper.hpp values.hpp
	$(CC) -Wall --std=c++17 -c piece_ext.cpp -o piece_ext.o

piece_valid.o: piece_valid.cpp piece_valid.hpp helper.hpp values.hpp
	$(CC) -Wall --std=c++17 -c piece_valid.cpp -o piece_valid.o

make: 
	make ./compute/Makefile

clean:
	rm -f *.o rm -f ./pieces/*.o
	rm -f *.o rm -f ./compute/*.o

