#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<errno.h>
#include	<string.h>


void errExit(int);


int 
main(void)
{
	extern	char etext, edata, end;
	int *	heap_start		= NULL;
	int *	heap_end		= NULL;
	int *	heap_indicator	= NULL;
	int	*	return_value	= NULL;
	int		index;

	printf("----------------------------------------\n\n");
	printf("End of text segment:\t\t\t%10p\n", &etext);
	printf("End of initialized data segment:\t%10p\n", &edata);
	printf("End of uninitialized data segment:\t%10p\n", &end);
	printf("\n----------------------------------------\n\n");

	errno = 0;
	heap_start		= (int *)sbrk(0);
	if((int *)-1 == heap_start) errExit(errno);
	
	heap_indicator	= heap_start;
	heap_end		= heap_start;

	for(index = 0; index < 10; index++) heap_end++;

	printf("Size of int: %ld\n", sizeof(int));
	printf("Start of the heap segment:\t%10p\n", heap_start);
	printf("The indicator has to be set in\t%10p location, "
			"for allocating 10 integers.\n", heap_end);
	
	errno = 0;
	if(-1 == brk((void *) heap_end)) errExit(errno);
	errno = 0;
	return_value = (int *)sbrk(0);
	if((int *)-1 == return_value) errExit(errno);
	
	printf("Adjusts the program break to\t%10p\n", return_value);
	printf("\n----------------------------------------\n\n");
	
	for(heap_indicator, index = 0; heap_indicator < heap_end; heap_indicator++, index++)
		*heap_indicator = index;

	for(heap_indicator = heap_start; heap_indicator < heap_end; heap_indicator++)
		printf(" %d", *heap_indicator);

	printf("\n\n");
	
	exit(EXIT_SUCCESS);

}/* main(void) */


void 
errExit(int error_number)
{
	fputs(strerror(error_number), stderr);
	fputs("\n", stderr);

	exit(EXIT_FAILURE);
}/* errExit(int error_number) */
