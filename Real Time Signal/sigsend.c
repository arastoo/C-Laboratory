#ifndef		_POSIX_C_SOURCE
#define		_POSIX_C_SOURCE		199309
#endif
#include	<signal.h>
#include	"general.h"


int 
main(int argc, char * argv[])
{
	int		sig;
	int		num_sigs;
	int		sig_data;
	int		index;
	
	union	sigval	sv;

	if(4 > argc || strcmp(argv[1], "--help") == 0)
		usageError("%s pid sig-num data [num-sigs]\n", argv[0]);

	/*
	 * Display our PID and UID, so that they can be compared with the
	 * corresponding fields of the siginfo_t argument supplied to the 
	 * handler in the receiving process
	 */
	
	printf("%s: PID is %ld, UID is %ld\n", 
			argv[0], (long) getpid(), (long) getuid());

	sig			= atoi(argv[2]);
	sig_data	= atoi(argv[3]);
	num_sigs	= (4 > argc) ? atoi(argv[4]) : 1;

	for(index = 0; index < num_sigs; index++)
	{
		sv.sival_int = sig_data + index;
		if(-1 == sigqueue(atol(argv[1]), sig, sv))
			errExit("sigqueue %d", index);
	}/* for(index = 0; index < num_sigs; index++) */

	exit(EXIT_SUCCESS);

}/* main(int argc, char * argv[]) */
