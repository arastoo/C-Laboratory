#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#ifndef LIBUE
#define LIBUE


#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<errno.h>


#include	"libue.h"


#define printError(string) fprintf(stderr, "%s\n", string)


void
usage(void)
{
	printf("\nUsage: %s [Options]\n"
			"\n\t-s\t\t--source\t\tDefine source device."
			"\n\t-m\t\t--mount\t\t\tDefine mount point.\n",program_invocation_name);
}/* usage(void) */


void 
errExit(const char * const msg, int error)
{
	if(NULL != msg) printError(msg);
	if(0 != error) printError(strerror(error));
	exit(EXIT_FAILURE);
}/* errExit(const char * const msg, int error) */

#endif
