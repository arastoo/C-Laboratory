#include	<stdio.h>
#include	<stdlib.h>
#include	<sys/time.h>
#include	<time.h>
#include	<errno.h>


int 
main(void)
{
	int				errnum;
	struct timeval	time;

	if(-1 == gettimeofday(&time, NULL))
	{
		fprintf(stderr, "Error\n");
		exit(EXIT_FAILURE);
	}/* if(-1 == gettimeofday(&time, NULL)) */

	fprintf(stdout, "%lld\t\t%lld\n", time.tv_sec, time.tv_usec);
	fprintf(stdout, "Calendar:\t%s\n", ctime(&time.tv_sec));


	exit(EXIT_SUCCESS);
}/* main(void) */
