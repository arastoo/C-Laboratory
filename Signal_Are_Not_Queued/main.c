#ifndef		_GNU_SOURCE
#define		_GNU_SOURCE
#endif
#include	<stdlib.h>
#include	<stdio.h>
#include	<errno.h>
#include	<string.h>
#include	<signal.h>


int 
main(int argc, char * argv[])
{
	int		number_signals;
	int		error_number;
	int		sig;
	int		index;
	pid_t	pid;


	if(argc < 4 || strcmp(argv[1], "--help") == 0)
	{
		fprintf(stdout, "%s pid num-sig sig_num [sig-num-2]", 
				program_invocation_short_name);
		exit(EXIT_SUCCESS);
	}/* if(argc < 4 || strcmp(argv[1], "--help") == 0) */
	
	pid				= atol(argv[1]);
	number_signals	= atoi(argv[2]);
	sig				= atoi(argv[3]);

	printf("%s: sending signal %d to process %ld %d times\n", 
			program_invocation_short_name, sig, (long) pid, number_signals);

	for(index = 0; index < number_signals; index++)
	{
		errno = 0;
		if(-1 == kill(pid, sig))
		{
			error_number = errno;
			fprintf(stderr, "Error: %s.\n", strerror(error_number));
			exit(EXIT_FAILURE);
		}/* if(-1 == kill(pid, sig)) */
	}/* for(index = 0; index < number_signals; index++) */

	if(5 == argc)
	{
		errno = 0;
		if(-1 == kill(pid, atoi(argv[4])))
		{
			error_number = errno;
			fprintf(stderr, "Error: %s.\n", strerror(error_number));
			exit(EXIT_FAILURE);
		}/* if(-1 == kill(pid, atoi(argv[4]))) */
	}/* if(5 == argc) */

	printf("%s: exiting\n", program_invocation_short_name);
	exit(EXIT_SUCCESS);

}/* main(int argc, char * argv[]) */
