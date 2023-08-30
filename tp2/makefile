# Makefile de exemplo (Manual do GNU Make)

CFLAGS = -ansi -g -Wall -Wextra # flags de compilacao
CC = gcc

all: tp2.o racionais.o
	$(CC) -o tp2 tp2.o racionais.o

racionais.o: racionais.c
	$(CC) -c $(CFLAGS) racionais.c

tp2.o: tp2.c
	$(CC) -c $(CFLAGS) tp2.c

clean:
	rm -f *.o *~ tp2
