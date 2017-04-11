#define		_GNU_SOURCE

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<getopt.h>
#include	<string.h>


void printHelp(FILE * std, int exit_status);


int
main(int argc, char * argv[])
{
	int		opt;

	const char * const	short_opt			= ":hfic:";
	const struct		option	long_opt[]	= {
		{"help",		no_argument,		NULL,	'h'},
		{"full-name",	no_argument,		NULL,	'f'},
		{"info",		no_argument,		NULL,	'i'},
		{"contact",		required_argument,	NULL,	'c'},
		{0,				0,					   0,	 0}
	};

	const char * const full_name = "Arash Sadeghizadeh\n";
	const char * const info		 = "I do not know really who i am.\n"
									"But i know what really i want.\n" 
									"And it doesn't matter what others think.\n";
	const char * const email	 = "info@arashsadeghizadeh.me\n";
	const char * const linkedin  = "https://ir.linkedin.com/in/arashsadeghizadeh\n";

	if(2 > argc) printHelp(stderr, EXIT_FAILURE);

	while(-1 != (opt = getopt_long(argc, argv, short_opt, long_opt, NULL)))
	{
		switch(opt)
		{
			case 'h':
				printHelp(stdout, EXIT_SUCCESS);
				break;

			case 'f':
				fprintf(stdout, full_name);
				continue;

			case 'i':
				fprintf(stdout, info);
				continue;

			case 'c':
				if(NULL != optarg)
				{
					if(0 == (strncmp(optarg, "email", 9)))
						fprintf(stdout, email);
					else if (0 == (strncmp(optarg, "linkedin", 9)))
						fprintf(stdout, linkedin);
					else
						printHelp(stderr, EXIT_FAILURE);
				}/* if(NULL != optarg) */
				else printHelp(stderr, EXIT_FAILURE);
				break;
			case ':':
				fprintf(stdout, "Error: Wrong argument for option\n");
				printHelp(stderr, EXIT_FAILURE);
				break;
			case '?':
				fprintf(stdout, "Error: Unrecognized option\n");
				printHelp(stderr, EXIT_FAILURE);
				break;
		}/* switch(opt) */

		if(optind < argc)
		{
			fprintf(stderr, "Error: Unrecognized argument\n");
			for(; optind < argc; optind++)
				fprintf(stderr, "%s\n", argv[optind]);
			putc('\n', stdout);
		}/* if(optind > argc) */

		exit(EXIT_SUCCESS);
	}/* while(-1 == (opt = getopt_long(argc, argv, short_opt, long_opt, NULL))) */


}/* main(int argc, char * argv[]) */


void printHelp(FILE * std, int exit_status)
{
	const char * const msg = "\n"
							 "<Program Name> [OPTIONS]\n"
							 "  -h\t--help     \t\tShow this help\n"
							 "  -f\t--full-name\t\tShows my full name\n"
							 "  -i\t--info     \t\tShows my information\n"
							 "  -c\t--contact  \t\tThis option requires an argument, "
							 "you can choose one of the email or linkedin\n";

	fprintf(std, msg);

	exit(exit_status);
}/* void printHelp(FILE * std) */
