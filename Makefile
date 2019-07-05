CC = g++

all: values.o helper.o move.o board.o match.o engine.o
	$(CC) -Wall -ggdb match.o board.o move.o helper.o values.o engine.o -o engine

engine.o: engine.cpp
	$(CC) -Wall -ggdb -c engine.cpp

values.o: values.cpp values.hpp
	$(CC) -Wall -ggdb -c values.cpp

helper.o: helper.cpp helper.hpp
	$(CC) -Wall -ggdb -c helper.cpp

move.o: move.cpp move.hpp helper.hpp values.hpp
	$(CC) -Wall -ggdb -c move.cpp

board.o: board.cpp board.hpp helper.hpp values.hpp
	$(CC) -Wall -ggdb -c board.cpp

match.o: match.cpp match.hpp board.hpp helper.hpp values.hpp
	$(CC) -Wall -ggdb -c match.cpp

make: 
	make ./forth/Makefile

clean:
	rm -f *.o


