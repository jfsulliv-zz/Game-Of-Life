CFLAGS=-Wall -g -DNDEBUG

all: game

game: 
	gcc $(CFLAGS) game.c -o game

clean: 
	rm game
	rm *.o
