CC = g++

piece.o: ./piece.cpp ./piece.hpp
	$(CC) -Wall --std=c++17 -c ./piece.cpp -o ./piece.o

piece_ext1.o: ./piece_ext1.cpp ./piece_ext1.hpp
	$(CC) -Wall --std=c++17 -c ./piece_ext1.cpp -o ./piece_ext1.o

piece_ext2.o: ./piece_ext2.cpp ./piece_ext2.hpp
	$(CC) -Wall --std=c++17 -c ./piece_ext2.cpp -o ./piece_ext2.o

searchforpiece.o: ./searchforpiece.cpp ./searchforpiece.hpp
	$(CC) -Wall --std=c++17 -c ./searchforpiece.cpp -o ./searchforpiece.o

touch.o: ./touch.cpp ./touch.hpp
	$(CC) -Wall --std=c++17 -c ./touch.cpp -o ./touch.o

clean:
	rm -f *.o
