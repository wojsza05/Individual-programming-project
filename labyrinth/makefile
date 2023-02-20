.PHONY: all clean

CC = gcc
CFLAGS = -Wall -Wextra -Wno-implicit-fallthrough -std=c17 -O2 -c
LDFLAGS = 

all: labyrinth

structs.o: structs.c structs.h
	$(CC) $(CFLAGS) $<

reading.o: reading.c reading.h structs.h
	$(CC) $(CFLAGS) $<

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) $<

bfs.o: bfs.c bfs.h queue.h structs.h
	$(CC) $(CFLAGS) $<

labyrinth.o: labyrinth.c reading.h structs.h bfs.h
	$(CC) $(CFLAGS) $<

labyrinth: labyrinth.o reading.o structs.o bfs.o queue.o
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	-rm *.o
	-rm labyrinth