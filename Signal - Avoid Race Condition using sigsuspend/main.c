#ifndef		_POSIX_C_SOURCE
#define		_POSIX_C_SOURCE  200809 
#endif
#include	<string.h>
#include	<signal.h>
#include	<time.h>
#include	"general.h"
#include	"signal_functions.h"


static	volatile	sig_atomic_t	gotSigquit = 0;


static void handler(int);


int 
main(int argc, char * argv[])
{
	int			loopNum;
	time_t		startTime;
	sigset_t	origMask;
	sigset_t	blockMask;
	struct		sigaction	sa;

	printSigMask(stdout, "Initial signal mask is:\n");

	sigemptyset(&blockMask);
	sigaddset(&blockMask, SIGINT);
	sigaddset(&blockMask, SIGQUIT);
	
	if(-1 == sigprocmask(SIG_BLOCK, &blockMask, &origMask))
		errExit("sigprocmask - SIG_BLOCK");

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handler;

	if(-1 == sigaction(SIGINT, &sa, NULL))
		errExit("sigaction");

	if(-1 == sigaction(SIGQUIT, &sa, NULL))
		errExit("sigaction");

	for(loopNum = 1; !gotSigquit; loopNum++)
	{
		printf("=== LOOP %d\n", loopNum);

		/*
		 * Simulate a critical section by delaying a few seconds
		 */

		printSigMask(stdout, "Starting critical section, signal mask is:\n");
		
		for(startTime = time(NULL); time(NULL) < startTime + 4; )
			continue;

		printPendingSigs(stdout, "Before sigsuspend() - pending signals:\n");

		if(-1 == sigsuspend(&origMask) && EINTR != errno)
			errExit("sigsuspend");

	}/* for(loopNum = 1; !gotSigquit; loopNum++) */
	
	if(-1 == sigprocmask(SIG_SETMASK, &origMask, NULL))
		errExit("sigprocmask - SIG_SETMASK");

	printSigMask(stdout, "=== Exited loop\nRestored signal mask to:\n");

	/*
	 * Do other processing ...
	 */

	exit(EXIT_SUCCESS);

}/* main(int argc, char * argv[]) */

static void 
handler(int sig)
{
	/* UNSAFE */
	printf("Caught signal %d (%s)\n", sig, strsignal(sig));

	if(SIGQUIT == sig)
		gotSigquit = 1;
}/* handler(int sig) */
