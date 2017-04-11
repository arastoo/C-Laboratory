#define		_DEFAULT_SOURCE || (_XOPEN_SOURCE >= 500) && ! (_POSIX_C_SOURCE >= 200112L)


#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<errno.h>
#include	<mcheck.h>


#include	"Trace_Program_break_lib.h"


#define		MAX_ALLOCS	1000000


typedef enum {OFF, ON} OP_FLAG;


int
main(int argc, char * argv[])
{
	int		opt;
	int		index;
	char *	next_string;
	char *	ptr[MAX_ALLOCS];
	long	int		number_allocation;
	long	int		block_size;
	long	int		free_step;
	long	int		start_range_free;
	long	int		end_range_free;
	const	char *	const	opt_string = ":hn:s:S:r:";

	OP_FLAG NUMBER_OF_BLOCKS	= OFF;
	OP_FLAG SIZE_OF_BLOCKS		= OFF;
	OP_FLAG FREE_STEP			= OFF;
	OP_FLAG RANGE_FREE			= OFF;

	/*
	 * Turn on tracing of memory allocation calls.
	 */
	mtrace();

	while(-1 != (opt = getopt(argc, argv, opt_string)))
	{
		switch(opt)
		{
			case 'n':
				convertToInt(optarg, &number_allocation);
				NUMBER_OF_BLOCKS = ON;
				continue;

			case 's':
				convertToInt(optarg, &block_size);
				SIZE_OF_BLOCKS = ON;
				continue;

			case 'S':
				convertToInt(optarg, &free_step);
				FREE_STEP = ON;
				continue;

			case 'r':
				if(-1 == stringParser(optarg, ",", &next_string)) 
					errExit("Error occurred in stringParser function", -1);
				convertToInt(optarg, &start_range_free);
				convertToInt(next_string, &end_range_free);
				RANGE_FREE = ON;
				continue;

			case 'h':
				printHelp(stdout);
				exit(EXIT_SUCCESS);
				break;

			case ':':
				fprintf(stdout, "Option needs an argument.\n");
				printHelp(stdout);
				errExit(NULL, -1);
				break;

			case '?':
				fprintf(stderr, "Unrecognize option %c.\n", optopt);
				exit(EXIT_SUCCESS);
				break;
		}/* switch(opt) */
	}/* while(-1 != (opt = getopt(argc, argv, opt_string))) */

	if(!(NUMBER_OF_BLOCKS && SIZE_OF_BLOCKS))
	{
		fprintf(stderr, "-n And -s option must be delcared.\n");
		printHelp(stderr);
		errExit(NULL, -1);
	}/* if(!(NUMBER_OF_BLOCKS && SIZE_OF_BLOCKS)) */

	if(!FREE_STEP) free_step = 1;
	if(!RANGE_FREE) 
	{
		start_range_free	= 1;
		end_range_free		= number_allocation; 
	}/* if(!RANGE_FREE) */

	if(MAX_ALLOCS < number_allocation)
		errExit("Number of blocks for allocation are greater than\n"
				"maximum valid size [MAX = 1000000].", -1);

	if(start_range_free > number_allocation)
		errExit("Range of blocks that we wish to free are greater than\n"
				"number of blocks to allocated.", -1);
	
	printf("Initial program break:\t%10p\n", sbrk(0));
	printf("Allocating %ld * %ld bytes\n", number_allocation, block_size);

	for(index = 0; index < number_allocation; index++)
	{
		errno = 0;
		ptr[index] = (char *)malloc(block_size);
		if(NULL == ptr[index] && 0 != errno)
		{
			errExit("malloc library function", errno);
		}/* if(NULL == ptr[index] && 0 != errno) */
	}/* for(index = 0; index < number_allocation; index++) */

	printf("Program break is now:\t%10p\n", sbrk(0));
	printf("Freeing blocks from %ld to %ld in steps of %ld.\n",
			start_range_free, end_range_free, free_step);

	for(index = start_range_free - 1; index < end_range_free; index += free_step)
	{
		free(ptr[index]);
	}/* for(index = 0; index< end_range_free; index += free_step) */

	printf("After free(), program break is:\t %10p\n", sbrk(0));

	/*
	 * Turn of tracing of memory allocation calls.
	 */
	muntrace();

	exit(EXIT_SUCCESS);
}/* main(int argc, char * argv[]) */
