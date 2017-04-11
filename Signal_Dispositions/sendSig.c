#ifndef		_GNU_SOURCE
#define		_GNU_SOURCE
#endif
#include	<stdio.h>
#include	<stdlib.h>
#include	<errno.h>
#include	<signal.h>
#include	<string.h>


int 
main(int argc, char * argv[])
{
	int err_number = 0;

	if(2 != argc)
	{
		fprintf(stderr, "%s <pid>", program_invocation_short_name);
		exit(EXIT_FAILURE);
	}/* if(2 != argc) */

	errno = 0;
	if(-1 == kill(atoi(argv[1]), SIGTERM))
	{
		err_number = errno;
		fprintf(stderr, "Error: %s\n", strerror(err_number));
		exit(EXIT_FAILURE);
	}/* if(-1 == kill(argv[1], SIGTERM)) */

	printf("\nDone\n");

	exit(EXIT_SUCCESS);
}/* main(int argc, char * argv[]) */
