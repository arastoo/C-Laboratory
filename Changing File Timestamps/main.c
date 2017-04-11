#ifndef		_GNU_SOURCE
#define		_GNU_SOURCE
#endif
#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<getopt.h>
#include	<limits.h>
#include	<errno.h>
#include	<string.h>
#include	"timestamps_lib.h"
#include	"help_lib.h"
#include	"error_lib.h"


typedef enum {FALSE, TRUE} BOOLEAN;


int
main(int argc, char * argv[])
{
	const	char *	const	short_option = ":hp:dc";
	struct	option			long_option[] = {
		{"help",	no_argument,		0,	'h'},
		{"path",	required_argument,	0,	'p'},
		{"display",	no_argument,		0,	'd'},
		{"change",	no_argument,		0,	'c'},
		{0,					  0,		0,	  0}
	};
	char	path[PATH_MAX];
	int		option;
	int		OF_PATH		= FALSE;
	int		OF_DISPLAY	= FALSE;
	int		OF_CHANGE	= FALSE;

	if(argc <= 1 || argc >= 6)
	{
		printHelp();
		exit(EXIT_FAILURE);
	}/* if(argc <= 1 || argc >= 5) */

	while(-1 != (option = getopt_long(argc, argv, short_option, long_option, NULL)))
	{
		switch(option)
		{
			case 'h':
				printHelp();
				break;
			case 'p':
				strncpy(path, optarg, PATH_MAX - 1);
				OF_PATH = TRUE;
				continue;
			case 'd':
				OF_DISPLAY = TRUE;
				continue;
			case 'c':
				OF_CHANGE = TRUE;
				continue;
			case ':':
				printHelp();
				errExit("Option need an argument.", 0);
			case '?':
				printHelp();
				errExit("Unrecognized option", 0);
		}/* switch(option) */
	}/* while(-1 == (option = getopt_long(argc, argv, short_option, long_option, NULL))) */

	if(OF_PATH & OF_DISPLAY)	displayStatus(path);
	if(OF_PATH & OF_CHANGE)		changingTimestamps(path);

	exit(EXIT_SUCCESS);
}/* main(int argc, char * argv[]) */
