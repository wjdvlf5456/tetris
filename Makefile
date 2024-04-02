FLAGS = -Werror -std=c++11 -g
CC = g++

all: main

main: main.o game.o tetromino.o
	$(CC) $(FLAGS) -o main main.o game.o tetromino.o

main.o: main.cpp game.h tetromino.h
	$(CC) $(FLAGS) -c main.cpp

game.o: game.cpp game.h tetromino.h
	$(CC) $(FLAGS) -c game.cpp

tetromino.o: tetromino.cpp tetromino.h
	$(CC) $(FLAGS) -c tetromino.cpp

clean:
	rm -f *.o main.exe main