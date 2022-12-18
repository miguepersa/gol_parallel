OBJ = main.o gol.c
CC = gcc
CFLAGS = -ansi -Wall -pedantic -g -pthread

all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o juego_h

main.o: main.c
	$(CC) -c $(CFLAGS) $< -o $@

gol.o: gol.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf *.o juego_h