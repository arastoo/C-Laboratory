#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include	<string.h>
#include	"disSigSet.h"


void
printSigset(FILE * of, const char * prefix, const sigset_t * sigset)
{
	int sig, cnt;

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
		fprintf(of, "%s<empty signal set>\n", prefix);
}/* printSigset(FILE *of, const char * prefix, const sigset_t * sigset) */

int 
printSigMask(FILE * of, const char * msg)
{
	sigset_t currMask;

	if(NULL != msg)
		fprintf(of, "%s", msg);

	if(sigprocmask(SIG_BLOCK, NULL, &currMask) == -1)
		return -1;

	printSigset(of, "\t\t", &currMask);

	return 0;
}/* printSigMask(FILE * of, const char * msg) */

int
printPendingSigs(FILE *of, const char *msg)
{
	sigset_t pendingSigs;

	if(NULL != msg)
		fprintf(of, "%s", msg);

	if(sigpending(&pendingSigs) == -1)
		return -1;

	printSigset(of, "\t\t", &pendingSigs);

	return 0;
}/* printPendingSigs(FILE *of, const char *msg) */
