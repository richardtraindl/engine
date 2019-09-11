CC = g++

all: ./play.o

play.o: ./play.cpp ./play.hpp
	$(CC) -Wall --std=c++17 -c ./play.cpp -o ./play.o

clean:
	rm -f *.o
