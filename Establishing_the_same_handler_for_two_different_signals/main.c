#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<errno.h>
#include	<signal.h>


static void 
sigHandler(int sig)
{
	static int	count = 0;

	if(SIGINT == sig)
	{
		count++;
		printf("Caught SIGINT (%d)\n", count);
		return;
	}/* if(SIGINT == sig) */

	printf("Caught SIGQOUIT - that-s all folks!\n");
	exit(EXIT_SUCCESS);
}/* sigHandler(int sig) */


void
errExit(char * msg)
{
	fprintf(stderr, "%s\n", msg);
	exit(EXIT_FAILURE);
}/* errExit(char * msg) */


int 
main(int argc, char * argv[])
{
	if(signal(SIGINT, sigHandler) == SIG_ERR)
		errExit("signal");
	if(signal(SIGQUIT, sigHandler) == SIG_ERR)
		errExit("signal");
	
	for(;;)
		pause();
}/* main(int argc, char * argv[]) */
