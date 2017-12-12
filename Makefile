#
# Makefile

CFLAGS = -std=c99 -g2 -O2

.PHONY: clean

all: clean ep2 controle run

ep2: compila.tab.o lex.yy.o symrec.o acertos.o arena.o maq.o pilha.o posicao.o
	$(CC) -o $@ $^ -lm -lfl

controle: controle.o
	$(CC) -o $@ $^

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

compila.tab.o: compila.y
	bison -d compila.y
	gcc -c compila.tab.c

lex.yy.o: compila.l
	flex compila.l
	gcc -c lex.yy.c -std=c99

clean:
	rm -f *.o *~ ep2

run: 
	./ep2
