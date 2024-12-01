CC = gcc
CFLAGS = -Wall -std=c99 -pedantic

all: a3

a3: main.c graph_functions.c graph.h
	$(CC) $(CFLAGS) -o a3 main.c graph_functions.c

clean:
	rm -f a3 *.o
