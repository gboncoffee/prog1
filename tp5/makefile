# Makefile de exemplo (Manual do GNU Make)
     
CFLAGS = -Wall -Werror -Wextra -std=c90 -g  # flags de compilacao
LDFLAGS = -lm

CC = gcc

# arquivos-objeto
	objects = testa_fila.o fila.o 

tp5: testa_fila.o fila.o
	$(CC) -o tp5 testa_fila.o fila.o $(LDFLAGS)

fila.o: fila.c
	$(CC) -c $(CFLAGS) fila.c

testa_fila.o: testa_fila.c
	$(CC) -c $(CFLAGS) testa_fila.c

clean:
	rm -f $(objects) tp5
