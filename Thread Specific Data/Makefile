cc = gcc
CFLAGS= -g -Wall

all: main clean

main: main.o tlib.o error.o
	$(cc) error.o main.o tlib.o -o threadExample -lpthread

main.o: main.c general.h ename.c.inc 
	$(cc) $(CFLAGS) -c main.c

tlib.o: tlib.h tlib.c
	$(CC) $(CFLAGS) -c tlib.c

error.o: error.h error.c
	$(cc) $(CFLAGS) -c error.c

clean: 
	-rm error.o tlib.o main.o
