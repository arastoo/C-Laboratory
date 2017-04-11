#ifndef		_XOPEN_SOURCE
#define		_XOPEN_SOURCE	600
#endif
#include	<unistd.h>
#include	<signal.h>
#include	<string.h>
#include	"general.h"


static	char	*str2;
static	int		handled = 0;


static void 
handler(int sig)
{
	crypt(str2, "xx");
	handled++;
}/* handler(int sig) */

int 
main(int argc, char * argv[])
{
	char	*cr1;
	int		callNum, mismatch;
	struct	sigaction	sa;

	if(3 != argc)
		usageError("%s str1 str2\n", argv[0]);

	str2 = argv[2];
	cr1 = strdup(crypt(argv[1], "xx"));

	if(NULL == cr1) 
		errExit("strdup");

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handler;
	if(-1 == sigaction(SIGINT, &sa, NULL))
		errExit("sigaction");

	for(callNum = 1, mismatch = 0; ; callNum++)
	{
		if(0 != strcmp(crypt(argv[1], "xx"), cr1))
		{
			mismatch++;
			printf("Mismatch on call %d (mismatch=%d handled=%d)\n",
					callNum, mismatch, handled);
		}/* if(0 != strcmp(crypt(argv[1], "xx"), cr1)) */
	}/* for(callNum = 1, mismatch = 0; ; callNum++) */
}/* main(int argc, char * argv[]) */
