#ifndef		_GNU_SOURCE
#define		_GNU_SOURCE
#endif


#ifndef		TRACE_PROGRAM_BREAK_LIB
#define		TRACE_PROGRAM_BREAK_LIB


#include	"Trace_Program_break_lib.h"


#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<errno.h>


typedef enum {FALSE = -1, TRUE = 1} BOOLEAN;


void 
errExit(char * msg, int err)
{
	if(NULL != msg)
		fprintf(stderr, "%s\n", msg);
	if(-1 != err)
		fprintf(stderr, "Error: %s\n", strerror(err));

	exit(EXIT_FAILURE);
}/* errExit(int err) */


void 
convertToInt(const char * const string, long int * integer)
{
	errno = 0;
	*integer = strtol(string, NULL, 10);
	if(0 != errno) errExit(NULL, errno);
}/* convertToInt(char * string, long int * integer) */


int
stringParser(const char * const string, char * token, char ** next)
{
	int		index;
	char *	str;
	char *	save_ptr;
	char *	save_token;

	for(index = 0, str = (char *)string; ; index++, str = NULL)
	{
		save_token = strtok_r(str, token, &save_ptr);
		
		if(NULL == save_token) break;
		
		*next = save_token;
	}/* for(index = 0, str = string; ; index++, str = NULL) */

	if(2 != index) return FALSE;
	
	return TRUE;
}/* stringParser(const char * const string, char * token, char ** next) */


void
printHelp(FILE * stream)
{

	fprintf(stream, "USAGE: %s [-n number] [-s size] [-S step] [-r range]\n\n"
			"\t-n\t\tSpecify the number of blocks to allocate.\n\n"
			"\t-s\t\tSpecify the size of blocks to allocate.\n\n"
			"\t-S\t\tSpecify the loop step unit to be used when freeing\n"
			"\t\t\tmemory blocks, If we specify 1 here (which is also the\n"
			"\t\t\tdefault if this argument os omitted), the the program\n"
			"\t\t\tfrees every memory block; if 2, then every second \n"
			"\t\t\tallocated block; and so on.\n\n"
			"\t-r\t\tSpecify the range of blocks that we wish to free\n"
			"\t\t\t, we specified the ranges look like this: -r start,end\n",
			program_invocation_short_name);

}/* printHelp(FILE * stream) */


#endif
