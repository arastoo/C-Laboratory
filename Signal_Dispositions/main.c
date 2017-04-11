#ifndef		_GNU_SOURCE
#define		_GNU_SOURCE
#endif
#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<errno.h>
#include	<signal.h>
#include	<string.h>


void sigInteruptHandler(int);


int
main(void)
{
	int		index;
	void (*oldHandler) (int);
	
	oldHandler = signal(SIGINT, sigInteruptHandler);
	if(SIG_ERR == oldHandler)
		exit(EXIT_FAILURE);

	for(index = 0; ; index++)
	{
		printf("\n%d\n", index);
		sleep(3);
	}/* for(index; ; index++) */
	 
	exit(EXIT_SUCCESS);
}/* main(void) */

void
sigInteruptHandler(int sig)
{
	printf("\nSignal %s received!\n", strsignal(sig));
}/* sigInteruptHandler(int sig) */
