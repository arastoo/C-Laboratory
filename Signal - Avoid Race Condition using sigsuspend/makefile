cc = gcc
CFLAGS= -g -Wall

all: t_sigsuspend clean

t_sigsuspend: error.o signal_functions.o main.o
	$(cc) error.o signal_functions.o main.o -o t_sigsuspend

error.o: error.h error.c
	$(cc) $(CFLAGS) -c error.c

signal_functions.o: signal_functions.h signal_functions.c
	$(cc) $(CFLAGS) -c signal_functions.c

main.o: ename.c.inc general.h main.c
	$(cc) $(CFLAGS) -c main.c

clean: 
	-rm error.o signal_functions.o main.o
