OBJ = mainh.o funciones.o golh.c
CC = gcc
CFLAGS = -ansi -Wall -pedantic -g -pthread

all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o juego_h

mainh.o: mainh.c
	$(CC) -c $(CFLAGS) $< -o $@

funciones.o: funciones.c
	$(CC) -c $(CFLAGS) $< -o $@

golh.o: golh.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf *.o juego_h