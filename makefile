CC=g++

CFLAGS=-c -Wall

LDFLAGS=-lsfml-system -lsfml-graphics -lsfml-window -lsfml-audio


all: shitty

clean:
	rm -rf shitty *.o

shitty: shitty.o
	$(CC) shitty.o -o shitty $(LDFLAGS)

shitty.o: shitty.cpp
	$(CC) $(CFLAGS) shitty.cpp
	
