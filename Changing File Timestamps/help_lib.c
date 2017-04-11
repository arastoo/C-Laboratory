#ifndef		_GNU_SOURCE
#define		_GNU_SOURCE
#endif
#include	<stdio.h>
#include	<errno.h>
#include	"help_lib.h"


void
printHelp(void)
{
	printf("%s [option [argument]]\n"
			"\t-h\t--help\t\t\tPrint this help.\n"
			"\t-p\t--path\t\t\tSpecify path. option need an argument\n"
			"\t-d\t--display\t\tDisplay file status\n"
			"\t-c\t--change\t\tChange file timestamp\n"
			, program_invocation_short_name);
}/* printHelp(void) */
