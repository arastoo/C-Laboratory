cc = gcc
CFLAGS= -g -Wall

all: main clean

main: main.o error.o
	$(cc) error.o main.o -o threadExample -lpthread

main.o: main.c general.h ename.c.inc 
	$(cc) $(CFLAGS) -c main.c

error.o: error.h error.c
	$(cc) $(CFLAGS) -c error.c

clean: 
	-rm error.o main.o
