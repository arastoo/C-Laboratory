#define		_XOPEN_SOURCE

#include	<stdio.h>
#include	<stdlib.h>
#include	<sys/stat.h>
#include	<fcntl.h>
#include	<unistd.h>
#include	<errno.h>
#include	<string.h>



void errExit(int);


int 
main(int argc, char * argv[])
{
	int		fd;
	int		opt;
	int		err_number;
	int		F_SLEEP	= -1;
	int		oflags  = O_WRONLY | O_CREAT;
	char *	f_path  = NULL;
	const char * const	optstr = ":f:s";
	mode_t	mode = S_IRUSR | S_IWUSR;


	if(2 > argc) fprintf(stderr, "Program needs at least one argument\n");

	while(-1 != (opt = getopt(argc, argv, optstr)))
	{
		switch(opt)
		{
			case 'f':
				if(NULL != optarg)
				{
					f_path = optarg;
				}/* if(NULL != optarg) */
				continue;

			case 's':
				F_SLEEP = 1;
				continue;

			case ':':
				fprintf(stderr, "Option need an Argument\n");
				exit(EXIT_FAILURE);

			case '?':
				fprintf(stderr, "Unrecognized option\n");
				exit(EXIT_FAILURE);
		}/* switch(opt)*/
	}/* while(-1 != (opt = getopt(argc, argv, optstr))) */

	if(NULL != f_path)
	{
		errno = 0;
		fd = open(f_path, O_WRONLY);
		if(-1 != fd)
		{
			printf("[pid] %ld] File \"%s\" already exists\n", 
					(long) getpid(), f_path);

			errno = 0;
			if(-1 == close(fd))
			{
				err_number = errno;
				errExit(err_number);
			}/* if(-1 == close(fd)) */
		}/* if(-1 != fd) */
		else 
		{
			err_number = errno;
			if(err_number != ENOENT)
			{
				errExit(err_number);
			}/* if(err_number != ENOENT) */
			else 
			{
				printf("[PID %ld] File \"%s\" doesn't exist yet\n", 
						(long) getpid(), f_path);
				
				if(1 == F_SLEEP)
				{
					sleep(10);
					printf("[PID %ld] Done sleeping\n", (long) getpid());
				}/* if(F_SLEEP) */

				errno = 0;
				fd = open(f_path, oflags, mode);
				if(-1 == fd)
				{
					err_number = errno;
					errExit(err_number);
				}/* if(-1 == fd) */

				printf("[PID %ld] Created file \"%s\" exclusively\n",
						(long) getpid(), f_path);
			}/* else */
		}/* else */
	}/* if(NULL != f_path) */

	exit(EXIT_SUCCESS);
}/* main(int argc, char * argv[])*/


void 
errExit(int err)
{
	fprintf(stderr, "%s\n", strerror(err));
	exit(EXIT_FAILURE);
}/* errExit(int err) */
