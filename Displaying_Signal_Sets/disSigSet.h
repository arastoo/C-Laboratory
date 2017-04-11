#ifndef DISSIGSET_H
#define DISSIGSET_H
#include	<stdio.h>
#include	<signal.h>

void printSigset(FILE *, const char *, const sigset_t *);
int printSigMask(FILE * of, const char * msg);
int printPendingSigs(FILE *of, const char *msg);

#endif
