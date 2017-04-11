#ifndef		_GNU_SOURCE
#define		_GNU_SOURCE
#endif
#include	<stdlib.h>
#include	<stdio.h>
#include	<errno.h>
#include	<string.h>
#include	<signal.h>
#include	<unistd.h>
#include	"signal_functions.h"


static int sigCnt[NSIG];
static volatile sig_atomic_t gotSigint = 0;


static void 
handler(int sig)
{
	if(SIGINT == sig)
	{
		gotSigint = 1;
	}/* if(SIGINT == sig) */
	else
	{
		sigCnt[sig]++;
	}/* else */

}/* handler(int sig) */

int 
main(int argc, char * argv[])
{
	int		n;
	int		numSecs;
	int		error_number;
	sigset_t	pendingMask;
	sigset_t	blockingMask;
	sigset_t	emptyMask;

	printf("%s: PID is %ld\n", program_invocation_short_name, (long) getpid());

	for(n = 1; n < NSIG; n++)
		(void) signal(n, handler);

	if(1 < argc)
	{
		numSecs = atoi(argv[1]);

		sigfillset(&blockingMask);
		errno = 0;
		if(-1 == sigprocmask(SIG_SETMASK, &blockingMask, NULL))
		{
			error_number = errno;
			fprintf(stderr, "Error: %s.\n", strerror(error_number));
			exit(EXIT_FAILURE);
		}/* if(-1 == sigprocmask(SIG_SETMASK, &blockingMask, NULL)) */

		printf("%s: sleeping for %d seconds\n", 
				program_invocation_short_name, numSecs);
		sleep(numSecs);

		errno = 0;
		if(-1 == sigpending(&pendingMask))
		{
			error_number = errno;
			fprintf(stderr, "Error: %s.\n", strerror(error_number));
			exit(EXIT_FAILURE);
		}/* if(-1 == sigpending(&pendingMask)) */

		printf("%s: pending signals are: \n", 
				program_invocation_short_name);	
		printSigset(stdout, "\t\t", &pendingMask);
		
		sigemptyset(&emptyMask);
		errno = 0;
		if(-1 == sigprocmask(SIG_SETMASK, &emptyMask, NULL))
		{
			error_number = errno;
			fprintf(stderr, "Error: %s.\n", strerror(error_number));
			exit(EXIT_FAILURE);
		}/* if(-1 == sigprocmask(SIG_SETMASK, &emptyMask, NULL)) */
	}/* if(1 < argc) */

	while(!gotSigint)
		continue;

	for(n = 1; n < NSIG; n++)
		if(sigCnt[n] != 0)
			printf("%s: signal %d caught %d time %s\n",
					program_invocation_short_name, n, sigCnt[n],
					(sigCnt[n] == 1) ? "" : "s");

	exit(EXIT_SUCCESS);
}/* main(int argc, char * argv[]) */
