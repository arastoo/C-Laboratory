#include	<signal.h>
#include	"general.h"


void handler(int);


int
main(void)
{
	sigset_t	prev_mask;
	sigset_t	int_mask;
	struct		sigaction	sa;

	sigemptyset(&int_mask);
	sigaddset(&int_mask, SIGINT);

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handler;

	if(-1 == sigaction(SIGINT, &sa, NULL))
		errExit("sigaction");

	/*
	 * Block SIGINT prior to executing critical section.
	 * (At this point we assume that SIGINT is not already blocked.)
	 */

	if(-1 == sigprocmask(SIG_BLOCK, &int_mask, &prev_mask))
		errExit("sigprocmask - SIG_BLOCK");

	/*
	 * Critical section: do some work here that must not be
	 * interrupted by the SIGINT handler.
	 */

	printf("Warning: Cricial section.\n");
	printf("Sleep 60sec.\n");
	sleep(60);

	/*
	 * End of critical Section - restore old mask to unblock SIGINT
	 */
	printf("Warning: End of critical section.\n");
	if(-1 == sigprocmask(SIG_SETMASK, &prev_mask, NULL))
		errExit("sigprocmask - SIG_SETMASK");

	printf("Simulate bug.\n");
	printf("Sleep 60sec.\n");
	sleep(60);
	printf("End sleep.\n");

	pause();
	
}/* main(void) */

void 
handler(int sig)
{
	/*
	 * UNSAFE: This handler use non-async-signal safe function, printf()
	 */

	printf("\n%s\n", strsignal(sig));
}/* handler(int sig) */
