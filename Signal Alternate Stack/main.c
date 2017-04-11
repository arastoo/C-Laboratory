#ifndef		_GNU_SOURCE
#define		_GNU_SOURCE
#endif
#include	<signal.h>
#include	"general.h"


static void sigsegvHandler(int);
static void overflowStack(int);


int
main(int argc, char * argv[])
{
	stack_t				sigstack;
	struct	sigaction	sa;
	int					j;

	printf("Top of standard stack is near %10p\n", (void *) &j);

	/*
	 * Allocate alternate stack and inform kernel of its existence
	 */

	sigstack.ss_sp = malloc(SIGSTKSZ);
	if(NULL == sigstack.ss_sp)
		errExit("malloc");

	sigstack.ss_size = SIGSTKSZ;
	sigstack.ss_flags = 0;
	if(-1 == sigaltstack(&sigstack, NULL))
		errExit("sigaltstack");

	printf("Alternate stack is at\t\t%10p-%p\n",
			sigstack.ss_sp, (char *) sbrk(0) - 1);

	/*
	 * Establish handler for SIGSEGV
	 */
	sa.sa_handler = sigsegvHandler;
	sigemptyset(&sa.sa_mask);
	/*
	 * Handler uses alternate stack
	 */
	sa.sa_flags = SA_ONSTACK;
	if(-1 == sigaction(SIGSEGV, &sa, NULL))
		errExit("sigaction");

	overflowStack(1);
}/* main(int argc, char * argv[]) */

static void
sigsegvHandler(int sig)
{
	/*
	 * UNSAFE: This handler uses non-async-signal-safe functions
	 * (printf(), strsignal(), fflush())
	 */

	int		x;

	printf("Caught signal %d (%s)\n", sig, strsignal(sig));
	printf("Top of handler stack near\t%10p\n", (void *) &x);
	fflush(NULL);

	_exit(EXIT_FAILURE);
}/* sigsegvHandler(int sig) */

static void
overflowStack(int callNum)
{
	/*
	 * A recursive function that overflows the stack
	 */

	char	a[100000];

	printf("Call %4d - top of stack near %10p\n", callNum, &a[0]);

	overflowStack(callNum + 1);
}/* overflowStack(int callNum) */
