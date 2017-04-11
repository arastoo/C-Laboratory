#ifndef		_GNU_SOURCE
#define		_GNU_SOURCE
#endif

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<getopt.h>
#include	<string.h>

#include	"libm.h"
#include	"libue.h"

#define		MAX_SIZE	200


int
main(int argc, char * argv[])
{
	int		opt;
	int		OF_SRC = 0;
	int		OF_DES = 0;
	char	source[MAX_SIZE];
	char	destination[MAX_SIZE];

	struct option	long_options[] = {
		{"source",	required_argument,	0, 's'},
		{"mount",	required_argument,	0, 'm'},
		{"help",	no_argument,		0, 'h'},
		{0,			0, 0, 0}
	};
	const char * const short_options = ":s:m:h";

	memset(source, '\0', MAX_SIZE);
	memset(destination, '\0', MAX_SIZE);

	while(-1 != (opt = getopt_long(argc, argv,
					short_options, long_options, NULL)))
	{
		switch(opt)
		{
			case 's':
				strncpy(source, optarg, strlen(optarg) + 1);
				OF_SRC = 1;
				continue;

			case 'm':
				strncpy(destination, optarg, strlen(optarg) + 1);
				OF_DES = 1;
				continue;

			case 'h':
				usage();
				exit(EXIT_SUCCESS);
				break;

			case ':':
				errExit("Option needs an argument.", 0);

			case '?':
				errExit("Unrecognized option.", 0);
		}/* switch(opt) */
	}/* while(-1 != (opt = getopt_long(argc, argv, */

	if(OF_DES & OF_SRC)
	{
		mountFS(source, destination);
	}/* if(OF_DES & OF_SRC) */

	exit(EXIT_SUCCESS);
}/* main(int argc, char * argv[]) */
