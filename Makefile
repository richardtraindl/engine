CC = g++

all: ./values.o ./move.o ./board.o ./piece.o ./match.o ./daemon.o ./generator.o ./generator2.o ./evaluator.o ./threading.o ./util/test.o ./util/debug.o ./engine.o \

	$(CC) -Wall --std=c++17 -lpthread -O3 \
	./values.o ./move.o ./board.o ./piece.o ./match.o ./daemon.o ./generator.o ./generator2.o ./evaluator.o ./threading.o ./util/test.o ./util/debug.o ./engine.o -o ./engine

engine.o: ./engine.cpp
	$(CC) -Wall --std=c++17 -O3 -c ./engine.cpp -o ./engine.o

debug.o: ./util/debug.cpp ./util/debug.hpp
	$(CC) -Wall --std=c++17 -O3 -c ./util/debug.cpp -o ./util/debug.o

test.o: ./util/test.cpp ./util/test.hpp
	$(CC) -Wall --std=c++17 -O3 -c ./util/test.cpp -o ./util/test.o

threading.o: ./threading.cpp ./threading.hpp
	$(CC) -Wall --std=c++17 -O3 -c ./threading.cpp -o ./threading.o

evaluator.o: ./evaluator.cpp ./evaluator.hpp
	$(CC) -Wall --std=c++17 -O3 -c ./evaluator.cpp -o ./evaluator.o

generator2.o: ./generator2.cpp ./generator2.hpp
	$(CC) -Wall --std=c++17 -O3 -c ./generator2.cpp -o ./generator2.o

generator.o: ./generator.cpp ./generator.hpp
	$(CC) -Wall --std=c++17 -O3 -c ./generator.cpp -o ./generator.o

daemon.o: ./daemon.cpp ./daemon.hpp
	$(CC) -Wall --std=c++17 -O3 -c ./daemon.cpp -o ./daemon.o

match.o: ./match.cpp ./match.hpp
	$(CC) -Wall --std=c++17 -O3 -c ./match.cpp -o ./match.o

piece.o: ./piece.cpp ./piece.hpp
	$(CC) -Wall --std=c++17 -O3 -c ./piece.cpp -o ./piece.o

board.o: ./board.cpp ./board.hpp
	$(CC) -Wall --std=c++17 -O3 -c ./board.cpp -o ./board.o

move.o: ./move.cpp ./move.hpp
	$(CC) -Wall --std=c++17 -O3 -c ./move.cpp -o ./move.o

values.o: ./values.cpp ./values.hpp
	$(CC) -Wall --std=c++17 -O3 -c ./values.cpp -o ./values.o

clean:
	rm -f *.o
	rm -f ./util/*.o
