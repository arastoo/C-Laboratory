cc = gcc
CFLAGS= -g -Wall

all: sigsend sigcatch clean

sigsend: error.o sigsend.o
	$(cc) error.o sigsend.o -o sigsend

sigcatch: error.o sigcatch.o
	$(cc) error.o sigcatch.o -o sigcatch

error.o: error.h error.c
	$(cc) $(CFLAGS) -c error.c

sigsend.o: ename.c.inc general.h sigsend.c
	$(cc) $(CFLAGS) -c sigsend.c

sigcatch.o: ename.c.inc general.h sigcatch.c
	$(cc) $(CFLAGS) -c sigcatch.c

clean: 
	-rm error.o sigsend.o sigcatch.o
