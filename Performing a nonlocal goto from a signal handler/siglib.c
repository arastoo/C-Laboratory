#ifndef		_GNU_SOURCE
#define		_GNU_SOURCE
#endif
#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<errno.h>
#include	<signal.h>
#include	"siglib.h"


/*
 * NOTE: All of the following functions employ fprintf(), which
 * is not async-signal-safe. As such these functions are also not
 * async-signal-safe (i.e., beware of indiscriminately calling
 * them from signal handlers).
 */

/*
 * Print list of signals within a signal set.
 */
static void
printSigset(FILE * of, const char * prefix, const sigset_t * sigset)
{
	int		sig;
	int		cnt;

	cnt = 0;
	for(sig = 1; sig < NSIG; sig++)
	{
		if(sigismember(sigset, sig))
		{
			cnt++;
			fprintf(of, "%s%d (%s)\n", prefix, sig, strsignal(sig));
		}/* if(sigismember(sigset, sig)) */
	}/* for(sig = 1; sig < NSIG; sig++) */

	if(0 == cnt)
	{
		fprintf(of, "%s<empty signal set>\n", prefix);
	}/* if(0 == cnt) */

}/* printSigset(FILE * of, const char * prefix, const sigset_t * sigset) */

int 
printSigMask(FILE *of, const char * msg)
{
	sigset_t currMask;

	if(NULL != msg)
	{
		fprintf(of, "%s", msg);
	}/* if(NULL != msg) */

	if(-1 == sigprocmask(SIG_BLOCK, NULL, &currMask))
	{
		return -1;
	}/* if(-1 == sigprocmask(SIG_BLOCK, NULL, &currMask)) */

	printSigset(of, "\t\t", &currMask);

	return 0;
}/* printSigMask(FILE *of, const char * msg) */

int
printPendingSigs(FILE * of, const char * msg)
{
	sigset_t	pendingSigs;

	if(NULL != msg)
	{
		fprintf(of, "%s", msg);
	}/* if(NULL != msg) */

	if(-1 == sigpending(&pendingSigs))
	{
		return -1;
	}/* if(-1 == sigpending(&pendingSigs)) */

	printSigset(of, "\t\t", &pendingSigs);

	return 0;
}/* printPendingSigs(FILE * of, const char * msg) */
