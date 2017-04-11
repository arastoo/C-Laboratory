#ifndef		_GNU_SOURCE
#define		_GNU_SOURCE
#endif
#include	<string.h>
#include	<signal.h>
#include	"general.h"


static	volatile	int		handlerSleepTime;
static	volatile	int		sigCnt	= 0;
static	volatile	int		allDone	= 0;


static void siginfoHandler(int, siginfo_t *, void *);


int 
main(int argc, char * argv[])
{
	int			sig;
	struct		sigaction sa;
	sigset_t	prevMask;
	sigset_t	blockMask;

	if(1 < argc && 0 ==  strcmp(argv[1], "--help"))
		usageError("%s [block-time [handler-sleep-time]]\n", argv[0]);
	
	printf("%s: PID is %ld\n", argv[0], (long) getpid());

	handlerSleepTime = (2 < argc) ? atoi(argv[2]) : 1;

	sa.sa_sigaction = siginfoHandler;
	sa.sa_flags = SA_SIGINFO;
	sigfillset(&sa.sa_mask);

	for(sig = 1; sig < NSIG; sig++)
		if(sig != SIGTSTP && sig != SIGQUIT)
			sigaction(sig, &sa, NULL);

	if(1 < argc)
	{
		sigfillset(&blockMask);
		sigdelset(&blockMask, SIGINT);
		sigdelset(&blockMask, SIGTERM);

		if(-1 == sigprocmask(SIG_SETMASK, &blockMask, &prevMask))
			errExit("sigprocmask");

		printf("%s: signals blocked - sleeping %s seconds\n", argv[0], argv[1]);
		sleep(atoi(argv[1]));
		printf("%s: sleep complete\n", argv[0]);

		if(-1 == sigprocmask(SIG_SETMASK, &prevMask, NULL))
			errExit("sigprocmask");
	}/* if(1 < argc) */

	while(!allDone)
		pause();

}/* main(int argc, char * argv[]) */


static void 
siginfoHandler(int sig, siginfo_t * si, void * ucontext)
{
	/*
	 * UNSAFE: This handler uses non-async-signal-safe functions, printf()
	 */

	/*
	 * SIGINT or SIGTERM can be used to terminate program
	 */

	if(SIGINT == sig || SIGTERM == sig)
	{
		allDone = 1;
		return;
	}/* if(SIGINT == sig || SIGTERM == sig) */

	sigCnt++;

	printf("caught signal %d\n", sig);

	printf("\tsi_signo=%d, si_code=%d (%s), ", si->si_signo, si->si_code, 
			(si->si_code == SI_USER) ? "SI_USER" :
			(si->si_code == SI_QUEUE) ? "SI_QUEUE" : "other");
	printf("si_value=%d\n", si->si_value.sival_int);
	printf("\tsi_pid=%ld, si_uid=%ld\n", (long) si->si_pid, (long) si->si_uid);

	sleep(handlerSleepTime);
}/* siginfoHandler(int sig, siginfo_t * si, void * ucontext) */
