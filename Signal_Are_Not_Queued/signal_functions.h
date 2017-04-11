#ifndef		SIGNAL_FUNCTIONS_H
#define		SIGNAL_FUNCTIONS_H
#include	<stdio.h>
#include	<signal.h>

void printSigset(FILE *, const char *, const sigset_t *);
int printSigMask(FILE *, const char *);
int printPendingSigs(FILE *, const char *);

#endif
