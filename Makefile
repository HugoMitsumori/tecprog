#
# Makefile

CFLAGS = -std=c99 -O2

.PHONY: clean

all: clean ep2 controle run

ep2: arena.o maq.o pilha.o posicao.o
	$(CC) -o $@ $^

controle: controle.o
	$(CC) -o $@ $^

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *~ ep2

run: 
	./ep2
