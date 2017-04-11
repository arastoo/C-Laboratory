#include	<string.h>
#include	<errno.h>
#include	<stdio.h>
#include	<stdlib.h>

#include	"error_lib.h"

void
errExit(const char * msg, int error)
{
	if(NULL != msg) fprintf(stderr, "%s.\n", msg);
	fprintf(stderr, "%s.\n", strerror(error));

	exit(EXIT_FAILURE);
}/* errExit(const char * const msg, int error) */
