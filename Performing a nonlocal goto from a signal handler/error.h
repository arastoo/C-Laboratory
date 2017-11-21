#ifndef		ERROR_H
#define		ERROR_H
#ifdef __GNUC__
    /* 
	 * This macro stops 'gcc -Wall' complaining that "control reaches
	 * end of non-void function" if we use the following functions to
	 * terminate main() or some other non-void function. 
	 */
#define NORETURN __attribute__ ((__noreturn__))
#else
#define NORETURN
#endif

void usageError(const char *, ...) NORETURN;
void errExit(const char *, ...) NORETURN;

#endif
