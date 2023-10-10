# makefile generico para os TADs
# Carlos Maziero - DINF/UFPR

CC     = gcc
CFLAGS = -Wall -Wextra -g
LIB    = lef
MAIN   = testa-lef

$(MAIN):   $(MAIN).o $(LIB).o
$(MAIN).o: $(MAIN).c $(LIB).h
$(LIB).o:  $(LIB).c  $(LIB).h

test: $(MAIN)
	./$(MAIN)

valgrind: $(MAIN)
	valgrind --leak-check=full ./$(MAIN)

clean:
	rm -f *.o *~ $(MAIN)
