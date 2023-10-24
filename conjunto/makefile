# Makefile de exemplo (Manual do GNU Make)
     
CFLAGS = -Wall -g  # flags de compilacao
LDFLAGS = -lm

CC = gcc

# arquivos-objeto
	objects = testa_conjunto.o conjunto.o 

testa_conjunto: testa_conjunto.o conjunto.o
	$(CC) -o testa_conjunto testa_conjunto.o conjunto.o $(LDFLAGS)

conjunto.o: conjunto.c
	$(CC) -c $(CFLAGS) conjunto.c

testa_conjunto.o: testa_conjunto.c
	$(CC) -c $(CFLAGS) testa_conjunto.c

clean:
	rm -f $(objects) testa_conjunto
