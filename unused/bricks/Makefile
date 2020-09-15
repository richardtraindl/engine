CC = g++

all: ./brick.o

brick.o: ./brick.cpp ./brick.hpp
	$(CC) -Wall --std=c++17 -c ./brick.cpp -o ./brick.o

clean:
	rm -f *.o
