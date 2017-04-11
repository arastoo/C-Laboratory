#ifndef		TRACE_PROGRAM_BREAK_LIB_H
#define		TRACE_PROGRAM_BREAK_LIB_H


#include	<stdio.h>


void errExit(char *, int);
void convertToInt(const char * const, long int * integer);
int	stringParser(const char * const, char *, char **);
void printHelp(FILE *);


#endif
