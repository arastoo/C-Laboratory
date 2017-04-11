#include	<stdio.h>
#include	<stdlib.h>
#include	<signal.h>
#include	<errno.h>
#include	<string.h>
#include	<unistd.h>


void sigHandler(int);


int
main(void)
{
	int err_number;
	struct sigaction action;
	action.sa_handler = sigHandler;

	errno = 0;
	if(-1 == sigemptyset(&action.sa_mask))
	{
		err_number = errno;
		fprintf(stderr, "Error: %s\n", strerror(err_number));
		exit(EXIT_FAILURE);
	}/* if(-1 == sigemptyset(&action.sa_mask)) */

	action.sa_flags = 0;

	if(-1 == sigaction(SIGINT, &action, NULL))
	{
		err_number = errno;
		fprintf(stderr, "Error: %s\n", strerror(err_number));
		exit(EXIT_FAILURE);

	}/* if(-1 == sigaction(SIGINT, &action, NULL)) */

	raise(SIGINT);

	for(;;)
		pause();

	exit(EXIT_SUCCESS);
	
}/* main(void) */

void
sigHandler(int sig)
{
	printf("\n%s\n", strsignal(sig));
}/* sigHandler(int sig) */
