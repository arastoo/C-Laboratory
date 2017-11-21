CC = gcc
CFLAGS= -g -Wall

all: main0 main1 clean

main0: error.o siglib.o sigjmplib0.o main0.o
	$(CC) -DUSE_SIGSETJMP error.o siglib.o sigjmplib.o main.o -o sigsetjmp

main1: error.o siglib.o sigjmplib1.o main1.o
	$(CC) error.o siglib.o sigjmplib.o main.o -o setjmp

siglib.o: siglib.h siglib.c
	$(CC) $(CFLAGS) -c siglib.c

sigjmplib0.o: sigjmplib.h sigjmplib.c
	$(CC) $(CFLAGS) -c -DUSE_SIGSETJMP sigjmplib.c

sigjmplib1.o: sigjmplib.h sigjmplib.c
	$(CC) $(CFLAGS) -c sigjmplib.c

error.o: error.h error.c
	$(CC) $(CFLAGS) -c error.c

main0.o: general.h ename.c.inc main.c
	$(CC) $(CFLAGS) -c -DUSE_SIGSETJMP main.c

main1.o: general.h ename.c.inc main.c
	$(CC) $(CFLAGS) -c main.c

clean: 
	-rm error.o siglib.o sigjmplib.o main.o
