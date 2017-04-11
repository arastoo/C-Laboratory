cc = gcc
CFLAGS = -g -Wall

all: daytimeserver clean

daytimeserver: error.o main.o
	$(cc) error.o main.o -o daytimeserver

error.o: error.h error.c
	$(cc) $(CFLAGS) -c error.c

main.o: ename.c.inc general.h wrapsock.h main.c
	$(cc) $(CFLAGS) -c main.c

clean:
	-rm error.o main.o
