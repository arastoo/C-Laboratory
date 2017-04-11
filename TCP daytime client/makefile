cc = gcc
CFLAGS = -g -Wall

all: daytime clean

daytime: error.o main.o
	$(cc) error.o main.o -o daytime

error.o: error.h error.c
	$(cc) $(CFLAGS) -c error.c

main.o: ename.c.inc general.h main.c
	$(cc) $(CFLAGS) -c main.c

clean:
	-rm error.o main.o
