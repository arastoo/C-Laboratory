#include	<signal.h>
#include	"general.h"


void sigHandler(int);


int 
main(int argc, char * argv[])
{
	struct	sigaction	act;

	if(-1 == sigemptyset(&act.sa_mask))
		errExit("sigemptyset");
	
	act.sa_flags = 0;
	act.sa_handler = sigHandler;

	if(-1 == sigaction(SIGALRM, &act, NULL))
		errExit("sigaction");

	alarm(30);
	pause();

	exit(EXIT_SUCCESS);

}/* main(int argc, char * argv[]) */

void
sigHandler(int sig)
{
	/*
	 * UNSAFE
	 */
	printf("%s\n", strsignal(sig));
}/* sigHandler(int sig) */
