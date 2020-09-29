CFLAGS = -std=c99 -pedantic -Wall
CC = gcc

all: array.o array.h

array.o: array.c
	$(CC) $(CFLAGS) -c array.c

test1.o: test1.c
	$(CC) $(CFLAGS) -c test1.c -o test1.o

test1: test1.o array.o array.h
	$(CC) $(CFLAGS) test1.o array.o -o test1

clean:
	rm array.o
