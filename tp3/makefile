# Makefile de exemplo (Manual do GNU Make)

CFLAGS = -Wall -Wextra -std=c90 -g # flags de compilacao
CC = gcc

all: tp3.o racionais.o
	$(CC) -o tp3 tp3.o racionais.o

racionais.o: racionais.c racionais.h
	$(CC) -c $(CFLAGS) racionais.c

tp3.o: tp3.c racionais.h
	$(CC) -c $(CFLAGS) tp3.c

clean:
	rm -f *.o *~ tp3
