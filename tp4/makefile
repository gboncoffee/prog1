# Makefile de exemplo (Manual do GNU Make)
     
CFLAGS = -Wall -Wextra -std=c90 -g  # flags de compilacao
LDFLAGS = -lm

CC = gcc

# arquivos-objeto
	objects = testa_pilha.o pilha.o 
     
all: testa_pilha.o pilha.o
	$(CC) -o tp4 testa_pilha.o pilha.o $(LDFLAGS)

pilha.o: pilha.c
	$(CC) -c $(CFLAGS) pilha.c

testa_pilha.o: testa_pilha.c
	$(CC) -c $(CFLAGS) testa_pilha.c

clean:
	rm -f $(objects) tp4


