#
# Makefile

CFLAGS = -Wall -std=c99 -O2

.PHONY: clean

all: clean ep2
	./ep2 clean

ep2: arena.o
	$(CC) -o $@ $^

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *~ ep2
