cc = gcc
CFLAGS= -g -Wall

all: nonreentrant clean

nonreentrant: error.o main.o
	$(cc) error.o main.o -o nonreentrant -lcrypt

error.o: error.h error.c
	$(cc) $(CFLAGS) -c error.c

main.o: ename.c.inc general.h main.c
	$(cc) $(CFLAGS) -c main.c 

clean: 
	-rm error.o main.o
