CC = g++

all: ./pieces/touch.o ./pieces/searchforpiece.o  \
	 ./pieces/piece_ext1.o ./pieces/piece_ext2.o ./pieces/piece.o \
	 ./compute/analyze_helper.o ./compute/analyze_tactics.o \
	 ./compute/analyze_move.o ./compute/analyze_position.o ./compute/calc.o \
	 helper.o values.o move.o board.o match.o engine.o \

	$(CC) -Wall --std=c++17 \
		./pieces/touch.o ./pieces/searchforpiece.o \
		./pieces/piece_ext1.o ./pieces/piece_ext2.o ./pieces/piece.o \
		./compute/analyze_helper.o ./compute/analyze_tactics.o \
		./compute/analyze_move.o ./compute/analyze_position.o ./compute/calc.o \
		helper.o values.o move.o board.o match.o engine.o -o engine

engine.o: engine.cpp
	$(CC) -Wall --std=c++17 -c engine.cpp

match.o: match.cpp match.hpp board.hpp helper.hpp values.hpp
	$(CC) -Wall --std=c++17 -c match.cpp

board.o: board.cpp board.hpp helper.hpp values.hpp
	$(CC) -Wall --std=c++17 -c board.cpp

move.o: move.cpp move.hpp helper.hpp values.hpp
	$(CC) -Wall --std=c++17 -c move.cpp

values.o: values.cpp values.hpp
	$(CC) -Wall --std=c++17 -c values.cpp

helper.o: helper.cpp helper.hpp helper.hpp values.hpp
	$(CC) -Wall --std=c++17 -c helper.cpp

make: 
	make ./pieces/Makefile

make: 
	make ./compute/Makefile

clean:
	rm -f *.o rm -f ./pieces/*.o
	rm -f *.o rm -f ./compute/*.o

